#ifndef _JSF_STUB_H_
#define _JSF_STUB_H_

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <iostream>
#include <string>
#include <map>
#include <libgen.h>
#include <msgpack.hpp>
#include "bytestream.h"
#include "manager.h"
#include "registresp_jdregist.h"

#ifdef __JSF_CONFIG_ACCESS__
#include "c2cplatform/component/config_access/jsf_config_access.h"
#include "c2cplatform/component/config_access/config_access.h"
#elif defined __CONFIG_ACCESS__
#include "c2cplatform/component/config_access/config_access.h"
#include "xaccess.h"
#endif

//#include "serialize.h"
extern "C" {
#include    "tcplib.h"
#include     "udplib.h"
}

const uint32_t MAX_BUF_LEN = 1 * 1024 * 1024;

using namespace std;

namespace jsf {
typedef enum {
    ERR_NOERR = 0, ERR_INIT = 0xF0000000,  //-0x10000000,
    ERR_SER_PARSE = 0xEFFFFFFF,  //-0x10000001, //服务端解码错误
    ERR_CONNECT = 0xEFFFFFFE,  //-0x10000002,
    ERR_SEND = 0xEFFFFFFD,  //-0x10000003,
    ERR_RECV = 0xEFFFFFFC,  //-0x10000004,
    ERR_PARSE = 0xEFFFFFFB,  //-0x10000005
    ERR_SERNAME = 0xEFFFFFFA,  //-0x10000006,
    ERR_CMDID = 0xEFFFFFF9,  //-0x10000007,
    ERR_TOOBIG = 0xEFFFFFF8,
    ERR_PACKHEAD = 0xEFFFFFF7,
    ERR_PACKBODY = 0xEFFFFFF6,
    ERR_UNPACKHEAD = 0xEFFFFFF5,
    ERR_UNPACKBODY = 0xEFFFFFF4,
    ERR_INITREGIST = 0xEFFFFFF3,
} enuErrCode;

typedef enum {
    PRO_TCP = 1,
} protocol_type_t;

typedef enum {
    JSF = 1, HTTP = 9,
} protocol_type;

typedef enum {
    JSON = 5, MSGPACK = 10,
} codec_type;

typedef enum {
    REQ = 1, REP = 2,
} msg_type;

typedef enum {
    TIMEOUT = 1,        //value is int请求超时时间
    CALLBACKINSID = 5,    //value is string回调函数对应的id
    JSFVERSION = 7,        //value is short请求的协议版本
    LANGUAGE = 8,       //vakye is byte客户端语言
    CUSTOM_MOCK = 251,
} headext_key;

typedef enum {
    INT = 1, STRING = 2, UINT8_T = 3, SHORT = 4,
} headext_valuetype;

class NilType {
};

static void IntToString(const uint32_t dwInt, std::string& strString) {
    char szBuf[50] = { 0 };
    snprintf(szBuf, sizeof(szBuf), "%u", dwInt);
    strString = szBuf;
}

template<uint32_t _iCmd, typename IN, typename OUT = NilType>
struct Protocol {
    static const uint32_t iCmd = _iCmd;
    typedef IN in;
    typedef OUT out;
};

class CBaseAsyncStub {
public:
    virtual void AsyncResp() = 0;
    virtual void AsyncTimeOut() = 0;
    virtual ~CBaseAsyncStub() {
    }
};

class CJsfWaiter {
public:
    // 在调用了AsyncrInvoke将请求发送出去之后,将Interface和套接字注册到Waiter中,后续统一监听处理
    void Regist(int iFD, CBaseAsyncStub *pInf) {
        m_mapInf[iFD] = pInf;
    }

    // 统一等待所有监听的套接字是否可读, 最长等待时长为iTimeoutMs毫秒
    void Wait(int iTimeoutMs = 3000) {
        // 初始化最大超时时间
        struct timeval timeout;
        struct timeval *prTimeout = &timeout;
        ;
        prTimeout->tv_sec = iTimeoutMs / 1000;
        prTimeout->tv_usec = (iTimeoutMs % 1000) * 1000;
        while (1) {
            // 初始化需要监听的套接字
            fd_set fds;
            FD_ZERO(&fds);
            int iMaxFd = -1;
            for (std::map<int, CBaseAsyncStub *>::iterator it =
                    m_mapInf.begin(); it != m_mapInf.end(); it++) {
                if (it->first >= 0 && it->second != NULL) {
                    iMaxFd = (it->first > iMaxFd) ? it->first : iMaxFd;
                    FD_SET((unsigned int ) it->first, &fds);
                }
            }
            // 如果已经没Fd需要处理,则退出循环
            if (-1 == iMaxFd)
                break;

            // 继续等待剩余的时间窗,select返回之后,会修改prTimeout为剩余的时间窗,使得中等待时间为初始设定值
            int iRet = select(iMaxFd + 1, &fds, NULL, NULL, prTimeout);
            // 超时没有应答
            if (iRet <= 0) {
                break;
            }

            for (std::map<int, CBaseAsyncStub *>::iterator it =
                    m_mapInf.begin(); it != m_mapInf.end();) {
                // 只是监听套接字合法,且Interface指针不等于NULL,已经处理过的Interface会将指针置为NULL
                if (FD_ISSET(it->first, &fds)) {
                    if (it->second) {
                        it->second->AsyncResp();
                    }
                    m_mapInf.erase(it++);
                    continue;
                }
                ++it;
            }
        }
        if (m_mapInf.empty()) {
            return;
        }

        for (std::map<int, CBaseAsyncStub *>::iterator it = m_mapInf.begin();
                it != m_mapInf.end(); ++it) {
            if (it->second) {
                it->second->AsyncTimeOut();
            }
        }
    }
private:
    std::map<int, CBaseAsyncStub *> m_mapInf;
};

class PkgHeadExt {
public:
    PkgHeadExt() :
            iClassType(1) {
    }
public:
    uint8_t iClassType;
    std::string strValue;
};

class PkgHead {
public:
    uint8_t uProtocolType;
    uint8_t uCodecType;
    uint8_t uMsgType;
    uint8_t uCompressType;
    uint32_t dwMsgId;
    uint8_t uExtSize;
    std::map<uint8_t, PkgHeadExt> mapExt;

public:
    PkgHead() :
            uProtocolType(JSF), uCodecType(MSGPACK), uMsgType(REQ), uCompressType(
                    0), dwMsgId(0), uExtSize(0) {
    }

    void Reset() {
        uProtocolType = 0;
        uCodecType = 0;
        uMsgType = 0;
        uCompressType = 0;
        dwMsgId = 0;
        uExtSize = 0;
        mapExt.clear();
    }

    bool Serialize(CByteStreamNetwork& bs) {
        bs & uProtocolType;
        bs & uCodecType;
        bs & uMsgType;
        bs & uCompressType;
        bs & dwMsgId;

        uint32_t dwValue = 0;
        if (bs.isStoring()) {
            uExtSize = mapExt.size();
            if (uExtSize) {
                bs & uExtSize;
                for (std::map<uint8_t, PkgHeadExt>::iterator it =
                        mapExt.begin(); it != mapExt.end(); it++) {
                    uint8_t iKeyTmp = it->first;
                    bs & iKeyTmp;
                    bs & it->second.iClassType;
                    if (it->second.iClassType == INT) {
                        dwValue = strtoul(it->second.strValue.c_str(), NULL,
                                10);
                        bs & dwValue;
                    } else if (it->second.iClassType == SHORT) {
                        short shValue = atoi(it->second.strValue.c_str());
                        bs & shValue;
                    } else if (it->second.iClassType == UINT8_T) {
                        uint8_t uValue = atoi(it->second.strValue.c_str());
                        bs & uValue;
                    }else {
                        bs & it->second.strValue;
                    }
                }
            }
        } else {
            bs & uExtSize;
            if (!bs.isGood()) {
                return true;
            }

            uint8_t iKey = 0;
            PkgHeadExt oExt;
            for (int i = 0; i < uExtSize; i++) {
                bs & iKey;
                bs & oExt.iClassType;
                if (oExt.iClassType == INT) {
                    bs & dwValue;
                    IntToString(dwValue, oExt.strValue);
                } else if (oExt.iClassType == SHORT) {
                    short wValue = 0;
                    bs & wValue;
                    IntToString(wValue, oExt.strValue);
                } else if (oExt.iClassType == UINT8_T) {
                    uint8_t uValue = 0;
                    bs & uValue;
                    IntToString(uValue, oExt.strValue);
                } else {
                    bs & oExt.strValue;
                }
                mapExt.insert(std::pair<uint8_t, PkgHeadExt>(iKey, oExt));
            }
        }

        return bs.isGood();
    }
};

#ifdef __CONFIG_ACCESS__
class config
{
public:
    std::string interface;
    std::string alias;
    std::string token;

    std::string& getInterface() {return trim(interface);}
    std::string& getAlias() {return trim(alias);}
    std::string& getToken() {return trim(token);}

    template<class AR>
    AR& serialize(AR& ar)
    {
        _SERIALIZE_(ar, interface);
        _SERIALIZE_(ar, alias);
        _SERIALIZE_(ar, token);
        return ar;
    }
private:
    // 删除字符串前后空白
    string& trim(string& str)
    {
        str = str.substr(str.find_first_not_of("\t \n"));
        str = str.substr(0, str.find_last_not_of("\t \n") + 1);
        return str;
    }
};
#endif

template<typename T>
class CJsfStub: public CBaseAsyncStub {
public:

#ifdef __JSF_CONFIG_ACCESS__
    CJsfStub(const char* svcName) :
            m_iTimeOutMs(3000), m_iTimeOut(3), m_iFd(-1), m_iRemotePort(0), m_isLongConnect(
            false), m_bInit(false), m_iSetId(0), m_eProtocol(PRO_TCP), m_pInnerBuf(
            NULL), m_dwBufLen(MAX_BUF_LEN), m_isAutoChooseIp(true), m_iResult(0),
            m_pResp(NULL), m_dwOnlineDebug(0), m_sSvcName(svcName), m_uSetId(0), m_uIp(0),
            m_isModuleInovke(true), m_mockFlag(false), m_iMockPort(0), m_jsfVersion(0) {
    	Init();
    }
#elif defined __CONFIG_ACCESS__
    CJsfStub(const char* svcName) :
    m_iTimeOutMs(3000),m_iTimeOut(3),m_iFd(-1), m_iRemotePort(0),
    m_isLongConnect(false), m_bInit(false), m_iSetId(0), m_eProtocol(PRO_TCP), m_pInnerBuf(NULL), m_dwBufLen(MAX_BUF_LEN),
    m_isAutoChooseIp(true),m_iResult(0), m_pResp(NULL),m_dwOnlineDebug(0), m_sSvcName(svcName), m_isModuleInovke(true), m_jsfVersion(0)
    {
        Init();
    }
#endif

    /*
     * 通过配置中心获取服务端地址
     */
    CJsfStub() :
            m_iTimeOutMs(3000), m_iTimeOut(3), m_iFd(-1), m_iRemotePort(0), m_isLongConnect(
            false), m_bInit(false), m_iSetId(0), m_eProtocol(PRO_TCP), m_pInnerBuf(
            NULL), m_dwBufLen(MAX_BUF_LEN), m_isAutoChooseIp(true), m_iResult(
                    0), m_pResp(NULL), m_dwOnlineDebug(0), m_uIp(0), m_isModuleInovke(true), m_mockFlag(false), m_iMockPort(0), m_jsfVersion(0) {
        Init();
    }

    /*
     * 指定服务端地址
     */
    CJsfStub(const char* szIp, int iPort) :
            m_iTimeOutMs(3000), m_iTimeOut(3), m_iFd(-1), m_iRemotePort(0), m_isLongConnect(
            false), m_bInit(false), m_iSetId(0), m_eProtocol(PRO_TCP), m_pInnerBuf(
            NULL), m_dwBufLen(MAX_BUF_LEN), m_isAutoChooseIp(false), m_iResult(
                    0), m_pResp(NULL), m_dwOnlineDebug(0), m_uIp(0), m_isModuleInovke(true), m_mockFlag(false), m_iMockPort(0), m_jsfVersion(0) {
        if (szIp != NULL) {
            m_sRemoteIp = szIp;
            m_uIp = inet_addr(szIp);
        }
        m_iRemotePort = iPort;
        Init();
    }

    void SetTimeOut(int iTimeOut) {
        m_iTimeOutMs = iTimeOut * 1000;
        m_iTimeOut = iTimeOut;
    }

    void SetTimeOutMs(int iTimeOut) {
        m_iTimeOutMs = iTimeOut;
        if (iTimeOut < 1000) {
            m_iTimeOut = 1;
        } else {
            m_iTimeOut = iTimeOut / 1000;
        }
    }

    void SetProtocol(protocol_type_t pto_type) {
        m_eProtocol = pto_type;
        Close();
    }

    void SetAddr(const char * szIp, int iPort) {
        m_sRemoteIp = szIp;
    	m_uIp = inet_addr(szIp);
        m_iRemotePort = iPort;
        m_isAutoChooseIp = false;
        Close();
    }

    void SetLongConnect() {
        m_isLongConnect = true;
    }

    void SetSendBufSize(const uint32_t dwSize) {
        m_dwBufLen = dwSize;
    }
#ifdef __JSF_CONFIG_ACCESS__
    void SetSvcName(const char *svcName) {
        m_sSvcName = svcName;
    }
    void SetSetId(uint32_t setId) {
        m_uSetId = setId;
        m_iSetId = setId;
    }
#elif defined __CONFIG_ACCESS__
    void SetSvcName(const char *svcName) {
        m_sSvcName = svcName;
    }
#endif
    void setModuleInvokeFlag(bool isModuleInovke) {
    	m_isModuleInovke = isModuleInovke;
    }
    // 获取异步返回码
    int GetResult() {
        return m_iResult;
    }

    int Invoke(const typename T::in & oReq, typename T::out & oResp) {
        if (Init() < 0) {
            return ERR_INIT;
        }

        if (InitService(oReq)) {
            m_iResult = ERR_INITREGIST;
            return m_iResult;
        }

        //获取并发调用的起始时间
        gettimeofday(&m_stTimeStart, NULL);
        m_iResult = SendReq(const_cast<typename T::in &>(oReq));
        if (m_iResult >= 0) {
            m_iResult = Receive(oResp);
        }
        gettimeofday(&m_stTimeEnd, NULL);

        if (m_isModuleInovke) {
#ifdef __JSF_CONFIG_ACCESS__
        	ModuleInvokeReport(oReq.method);
#elif defined __CONFIG_ACCESS__
        	ModuleInvokeReport(oReq);
#endif
        }

        return m_iResult;
    }

    int AsyncInvoke(const typename T::in & oReq, typename T::out & oResp,
            CJsfWaiter *pProtocolWaiter) {
        if (Init() < 0 || !pProtocolWaiter) {
            return ERR_INIT;
        }
        m_pResp = &oResp;
        m_iResult = 0;

        if (InitService(oReq)) {
            m_iResult = ERR_INITREGIST;
            return m_iResult;
        }

        gettimeofday(&m_stTimeStart, NULL);
        // 发送请求成功,则注册到waiter中,且将错误信息置为timeout
        m_iResult = SendReq(const_cast<typename T::in &>(oReq));

        if (m_iResult < 0) {
            gettimeofday(&m_stTimeEnd, NULL);
            return m_iResult;
        }
        pProtocolWaiter->Regist(m_iFd, this);
        m_methodName = oReq.method;
        return 0;
    }

    virtual void AsyncTimeOut() {
        m_iResult = ERR_RECV;
        snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                "addr:%s@%u,set=%d,wait timeout", m_sRemoteIp.c_str(),
                m_iRemotePort, m_iSetId);
        gettimeofday(&m_stTimeEnd, NULL);
    }

    // 异步处理应答
    virtual void AsyncResp() {
        snprintf(m_szErrMsg, sizeof(m_szErrMsg), "success");
        m_iResult = Receive(*m_pResp);
        gettimeofday(&m_stTimeEnd, NULL);
        if (m_isModuleInovke) {
#ifdef __JSF_CONFIG_ACCESS__
        	ModuleInvokeReport(m_methodName);
#endif
        }
        return;
    }

    const char * GetErrMsg() {
        if (m_iResult == 0) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "addr:%s@%u,set=%d,time=%uus,success", m_sRemoteIp.c_str(),
                    m_iRemotePort, m_iSetId, this->GetTimeCostUs());
        }
        return m_szErrMsg;
    }

    const char * GetStackMsg() {
        return m_szStackMsg.c_str();
    }

    //ms
    uint32_t GetTimeCost() {
        return GetTimeCostUs() / 1000;
    }

    //us
    uint32_t GetTimeCostUs() {
        return (m_stTimeEnd.tv_sec - m_stTimeStart.tv_sec) * 1000000
                + (m_stTimeEnd.tv_usec - m_stTimeStart.tv_usec);
    }

    //取服务器ip地址
    const char * GetSvrAddr() {
        return m_sRemoteIp.c_str();
    }
    //取服务器端口
    int GetSvrPort() {
        return m_iRemotePort;
    }

    const char * GetSvcName() {
        return m_sSvcName.c_str();
    }

    uint32_t GetSvcCmd() {
        return T::iCmd;
    }

    const char * GetSrcName() {
        return m_sSrcName.c_str();
    }

    const char * GetLocalAddr() {
        return m_sLocalIp.c_str();
    }

    int GetSetId() {
        return m_iSetId;
    }

    uint64_t GetTraceId() {
        return m_ddwTraceId;
    }

    virtual ~CJsfStub() {
        Close();

        if (m_pInnerBuf) {
            delete[] m_pInnerBuf;
            m_pInnerBuf = NULL;
        }
    }

    /**
     * 设置需要的服务端日至级别，大于此级别的会被收集回来
     */
    void SetOnlineDebug(uint32_t dwOnlineDebug = 4) {
        m_dwOnlineDebug = dwOnlineDebug;
    }

    const PkgHead & GetHead() {
        return m_oHead;
    }

    void SetCallerName(const std::string &strCallerName) {
        m_sCallerName = strCallerName;
    }

    void SetMockAddr(std::string mockIp, int mockPort) {
    	m_sMockIp = mockIp;
    	m_iMockPort = mockPort;
    	m_mockFlag = true;
    }

    void SetJsfVersion(short jsfVersion) {
        m_jsfVersion = jsfVersion;
    }

    void SetTraceId(uint64_t traceId) {
        m_ddwTraceId = traceId;
    }

protected:
    int SendReq(typename T::in & oReq) {
        PkgHead & cHead = m_oHead;
        cHead.Reset();
        cHead.uProtocolType = JSF;
        cHead.uCodecType = MSGPACK;
        cHead.uMsgType = REQ;
        cHead.uCompressType = 0;
        cHead.dwMsgId = 0;

        PkgHeadExt oExtTimeOut;
        oExtTimeOut.iClassType = INT;
        IntToString(m_iTimeOutMs, oExtTimeOut.strValue);
        cHead.mapExt.insert(pair<uint8_t, PkgHeadExt>(TIMEOUT, oExtTimeOut));

        short jsfVersion = 0;
        if (m_jsfVersion >= 1200) {
            jsfVersion = m_jsfVersion;
        }
        else {
#if _JSF_VERSION_ >= 1200
            jsfVersion = _JSF_VERSION_;
#endif
        }
        if (jsfVersion >= 1200) {
            PkgHeadExt oExtVersion;
            oExtVersion.iClassType = SHORT;
            std::stringstream ssVersion;
            ssVersion << jsfVersion;
            oExtVersion.strValue = ssVersion.str();
            cHead.mapExt.insert(pair<uint8_t, PkgHeadExt>(JSFVERSION, oExtVersion));
            if (jsfVersion >= 1500) {
                PkgHeadExt oExtLanguage;
                oExtLanguage.iClassType = UINT8_T;
                IntToString(1, oExtLanguage.strValue);
                cHead.mapExt.insert(pair<uint8_t, PkgHeadExt>(LANGUAGE, oExtLanguage));
            }
        }

        if (m_mockFlag) {
            PkgHeadExt oExtHeadMock;
            oExtHeadMock.iClassType = STRING;
            std::stringstream sAddr;
            sAddr << m_sRemoteIp << ":" << m_iRemotePort;
            oExtHeadMock.strValue = sAddr.str();
            cHead.mapExt.insert(pair<uint8_t, PkgHeadExt>(CUSTOM_MOCK, oExtHeadMock));
            m_sRemoteIp = m_sMockIp;
            m_iRemotePort = m_iMockPort;
        }

        // pkg
        uint16_t iMagicNum = 0xADCF;
        uint32_t dwPkgLength = 0;
        uint16_t iHeadLength = 0;

        memset(m_pInnerBuf, 0x0, m_dwBufLen);

        CByteStreamNetwork bs(m_pInnerBuf, m_dwBufLen);
        bs.isStoring(true);
        bs & iMagicNum;
        bs & dwPkgLength;
        bs & iHeadLength;
        if (!bs.isGood()) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "addr:%s@%u,packhead(magic) fail", m_sRemoteIp.c_str(),
                    m_iRemotePort);
            return ERR_PACKHEAD;
        }

        uint32_t dwHeadST = bs.getWrittenLength();
        // head serialize
        if (!cHead.Serialize(bs)) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "addr:%s@%u,packhead(serialize) fail", m_sRemoteIp.c_str(),
                    m_iRemotePort);
            return ERR_PACKHEAD;
        }
        uint32_t dwHeadET = bs.getWrittenLength();

        // fill head length
        uint16_t* pHeadLength = reinterpret_cast<uint16_t*>(m_pInnerBuf + 6);
        *pHeadLength = htons(dwHeadET - dwHeadST);

#ifdef __CONFIG_ACCESS__
        if (!m_jsfCfg.interface.empty()) {
            oReq.service = m_jsfCfg.getInterface();
        }
        if (!m_jsfCfg.alias.empty()) {
            oReq.alias = m_jsfCfg.getAlias();
        }
        if (!m_jsfCfg.token.empty()) {
            oReq.attachments[".token"] = m_jsfCfg.getToken();
        }
#endif
        // body serialize
        oReq.pack(bs);

        if (!bs.isGood()) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "addr:%s@%u,body too big",
                    m_sRemoteIp.c_str(), m_iRemotePort);
            return ERR_TOOBIG;
        }

        // fill pkg length
        uint32_t* pPkgLength = reinterpret_cast<uint32_t*>(m_pInnerBuf + 2);
        *pPkgLength = htonl(bs.getWrittenLength() - 2);

        //printf("sbuf.size=%u, head.size=%d\n", bs.getWrittenLength(), *pHeadLength);

        enuErrCode iRet = ERR_NOERR;

        do {
            if (m_iFd < 0) {
                if (this->GetProtocol() == PRO_TCP) {
                    m_iFd = nTcpAsynConnectMs(
                            const_cast<char *>(m_sRemoteIp.c_str()),
                            m_iRemotePort, m_iTimeOutMs);
                }

                if (m_iFd < 0) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "addr:%s@%u,set=%d,connect[%d] timeout,name:[%s],timeout:[%d][error:%s]",
                            m_sRemoteIp.c_str(), m_iRemotePort, m_iSetId,
                            this->GetProtocol(), oReq.service.c_str(),
                            m_iTimeOutMs, strerror(errno));
                    iRet = ERR_CONNECT;
                    break;
                }
            }

            if ((size_t) nTcpWriten(m_iFd, m_pInnerBuf, bs.getWrittenLength())
                    != bs.getWrittenLength()) {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                        "addr:%s@%u,set=%d, write[%u] fail [error:%s]",
                        m_sRemoteIp.c_str(), m_iRemotePort, m_iSetId,
                        bs.getWrittenLength(), strerror(errno));
                iRet = ERR_SEND;
                break;
            }
        } while (0);

        if (iRet) {
            Close();
        }

        return iRet;
    }

    int Receive(typename T::out & oResp) {
        PkgHead & cHead = m_oHead;
        cHead.Reset();
        enuErrCode iRet = ERR_NOERR;
        char * szBuf = NULL;

        do {
            int iNetPkgLen = 0;
            int iRead = 0;
            int iPkgLen = 0;
            char* pMark = NULL;

            if (this->GetProtocol() == PRO_TCP) {
                const int iLen = 6;
                char cLengthField[iLen];
                memset(cLengthField, 0, iLen);
                iRead = nTcpReadnMs(m_iFd, cLengthField, iLen, m_iTimeOutMs);
                iNetPkgLen = *(reinterpret_cast<uint32_t*>(cLengthField + 2));
                iPkgLen = ntohl(iNetPkgLen);

                if ((iRead < iLen) || (iPkgLen <= 0)
                        || (iPkgLen > 10 * 1024 * 1024)) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "addr:%s@%u,set=%d,receive head timeout[iRet:%d][len:%d][error:%s]",
                            m_sRemoteIp.c_str(), m_iRemotePort, m_iSetId, iRead,
                            iPkgLen, strerror(errno));
                    iRet = ERR_RECV;
                    break;
                }
                szBuf = new char[iPkgLen];
                bzero(szBuf, iPkgLen);
                *(int *) szBuf = iNetPkgLen;
                int iLeft = iPkgLen - sizeof(iNetPkgLen);
                if (iLeft < 0) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "addr:%s@%u,set=%d, receive head error[len:%d]",
                            m_sRemoteIp.c_str(), m_iRemotePort, m_iSetId,
                            iPkgLen);
                    iRet = ERR_RECV;
                    break;
                }

                // 从headlength field开始标记
                pMark = szBuf + sizeof(iNetPkgLen);
                iRead = nTcpReadnMs(m_iFd, pMark, iLeft, m_iTimeOutMs);
                if (iRead < iLeft) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "addr:%s@%u,set=%d,receive body timeout[error:%s]",
                            m_sRemoteIp.c_str(), m_iRemotePort, m_iSetId,
                            strerror(errno));
                    iRet = ERR_RECV;
                    break;
                }
            }

            try {
                // unpack head
                int iRespHeadLen = ntohs(*(reinterpret_cast<uint16_t*>(pMark)));
                //printf("iRespHeadLen=%d\n", iRespHeadLen);

                pMark += 2;

                CByteStreamNetwork bs(pMark, iRespHeadLen);
                bs.isStoring(false);

                if (!cHead.Serialize(bs)) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "addr:%s@%u,head read failed, iRespHeadLen(%d)",
                            m_sRemoteIp.c_str(), m_iRemotePort, iRespHeadLen);
                    iRet = ERR_UNPACKHEAD;
                    break;
                }

                // unpack obj
                int iBodyLen = iPkgLen - iRespHeadLen - 6;
                if (iBodyLen <= 0) {
                	snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "addr:%s@%u,resp head read failed, iPkgLen(%d), iRespHeadLen(%d)",
                            m_sRemoteIp.c_str(), m_iRemotePort, iPkgLen, iRespHeadLen);
                    iRet = ERR_UNPACKHEAD;
                    break;
                }
                oResp.unpack(bs.getRawBufCur(), iBodyLen);
            } catch (exception &bug) {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                        "addr:%s@%u,set=%d,parse pkg error.[error:%s].",
                        m_sRemoteIp.c_str(), m_iRemotePort, m_iSetId,
                        strerror(errno));
                m_szStackMsg = bug.what();
                iRet = ERR_PARSE;
                break;
            }
        } while (0);

        if (szBuf) {
            delete[] szBuf;
            szBuf = NULL;
        }

        if (!m_isLongConnect) {
            Close();
        }
        else if (iRet != 0) {
        	Close();
        }

        return iRet;
    }

    int Close() {
        if (m_iFd >= 0) {
            nTcpClose(m_iFd);
            m_iFd = -1;
        }

        return 0;
    }

    protocol_type_t GetProtocol() {
        return m_eProtocol;
    }

    int Init() {
        if (m_bInit) {
            return 0;
        }

        m_pInnerBuf = new char[m_dwBufLen];
        memset(m_pInnerBuf, 0x0, m_dwBufLen);

        memset(m_szErrMsg, 0x0, sizeof(m_szErrMsg));
        if (m_sLocalIp.empty()) {
            char szIp[20] = { 0 };
            if (GetLocalIp(szIp, sizeof(szIp)) < 0) {
                m_sLocalIp = "127.0.0.1";
            } else {
                m_sLocalIp = szIp;
            }
        }
        //if(InitCmdLine() < 0 || InitService() < 0)
        if (InitCmdLine() < 0) {
            return -1;
        }

        memset(&m_stTimeStart, 0x0, sizeof(m_stTimeStart));
        m_bInit = true;
        return 0;
    }

    int InitService(const typename T::in & oReq) {
        if (!m_isAutoChooseIp) {
            return 0;
        }

        if (m_iFd >= 0) {
            return 0;
        }

#ifdef __JSF_CONFIG_ACCESS__
        if (m_sSvcName != "") {
            // 按时间取模
            struct timespec now;
            int err = clock_gettime(CLOCK_MONOTONIC, &now);
            if (err) {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                        "clock_gettime() failed with %d .svcname:[%s]",
                        errno, m_sSvcName.c_str());
                return -1;
            }

            wq::config::jsf::JsfAddrInfo jsfAddrInfo;
            int ret = GetAddrBySvcName(m_sSvcName, now.tv_nsec, jsfAddrInfo);
            if (ret != 0) {
                return ret;
            }

            m_uIp = jsfAddrInfo.uIP;
            m_sRemoteIp = wq::config::jsf::JSF_CONFIG_ACCESS->GetStrIP(m_uIp);
            m_iRemotePort = jsfAddrInfo.wPort;
            const_cast<typename T::in &>(oReq).alias = jsfAddrInfo.sAlias;

            return 0;
        }
#elif defined __CONFIG_ACCESS__
        if (m_sSvcName != "") {
            uint16_t uPort = 0;
            // 按时间取模
            struct timespec now;
            int err = clock_gettime(CLOCK_MONOTONIC, &now);
            if(err) {
                snprintf(m_szErrMsg, sizeof(m_szErrMsg), "clock_gettime() failed with %d .svcname:[%s]",
                        errno, m_sSvcName.c_str());
                return -1;
            }
            int ret = GetAddrBySvcName(m_sSvcName, 0, now.tv_nsec, m_uIp, uPort);
            if (ret != 0) {
                return -1;
            }
            m_sRemoteIp = wq::config::CONFIG_ACCESS->GetStrIP(m_uIp);
            m_iRemotePort = uPort;

            ret = GetJSfConfigItem(m_sSvcName, m_jsfCfg);
            if (ret != 0) {
                return -1;
            }

            return 0;
        }
#endif

        jsf::regist::CManager oManager;

        CRegistResp oRegistResp;
        int32_t iRet = oManager.GetAddrBySerNameAlias(oReq.service, oReq.alias,
                oRegistResp);
        if (iRet || oRegistResp.listList.empty()) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "get dest ip from regist_agent fail.sername:[%s],alias:[%s]",
                    oReq.service.c_str(), oReq.alias.c_str());
            return -1;
        }

        // 按时间取模
        struct timespec now;
        int err = clock_gettime(CLOCK_MONOTONIC, &now);
        if (err) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "clock_gettime() failed with %d .sername:[%s],alias:[%s]",
                    errno, oReq.service.c_str(), oReq.alias.c_str());
            return -1;
        }
        uint32_t dwMod = now.tv_nsec % oRegistResp.listList.size();
        std::list<CIPInfo>::iterator it = oRegistResp.listList.begin();
        for (; it != oRegistResp.listList.end(); it++) {
            if (dwMod) {
                dwMod--;
            } else {
                break;
            }
        }

        if (it != oRegistResp.listList.end()) {
            m_sRemoteIp = it->strIp;
            m_iRemotePort = it->wPort;
        } else {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "get dest ip from regist_agent is null.sername:[%s],alias:[%s]",
                    oReq.service.c_str(), oReq.alias.c_str());
            return -1;
        }

        return 0;
    }

    int InitCmdLine() {
        if (!m_sSrcName.empty())
            return 0;

        static char szSrcName[64] = { 0 };
        if (strlen(szSrcName) > 0) {
            m_sSrcName = szSrcName;
            return 0;
        }

        pid_t pid = getpid();
        snprintf(szSrcName, sizeof(szSrcName), "/proc/%d/cmdline", pid);
        int iFd = open(szSrcName, O_RDONLY);
        memset(szSrcName, 0x0, sizeof(szSrcName));
        if (iFd < 0) {
            close(iFd);
            return -1;
        }
        if (read(iFd, szSrcName, sizeof(szSrcName) - 1) < 0) {
            close(iFd);
            return -1;
        }
        m_sSrcName = szSrcName;       //basename(szSrcName);
        return close(iFd);
    }

#ifdef __JSF_CONFIG_ACCESS__
    static const int MAX_SET_ID = 31;
    static const int ERR_CONF_ACCESS_SHM_NOT_FIND_SETID = -104;

    bool CheckJsfAddrInfo(const wq::config::jsf::JsfAddrInfo &jsfAddrInfo) {
        if (jsfAddrInfo.uIP == 0 || jsfAddrInfo.wPort == 0 ||
                jsfAddrInfo.sAlias == "" || jsfAddrInfo.sInterface == "") {
            return false;
        }
        return true;
    }

    int GetAddrByRandSetId(const std::string &sSvcName, uint32_t &maxSetId,
    		uint32_t uMod, wq::config::jsf::JsfAddrInfo &jsfAddrInfo, bool isUseful = true) {
    	int iRet = 0;
    	std::map<int, uint32_t> mapAddrCount;
    	uint32_t totalAddrCount = 0;
    	maxSetId = MAX_SET_ID;
    	int setId = 0;
    	for (; setId <= MAX_SET_ID; setId++) {
    		wq::config::jsf::JsfSetDataHead jsfSetDataHead;
            int iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5Head(sSvcName, setId, jsfSetDataHead);
            if (iRet == ERR_CONF_ACCESS_SHM_NOT_FIND_SETID) {
                break;
            }
            else if (iRet < 0) {
            	continue;
            }
            if (isUseful && jsfSetDataHead.dwSuccessRate == 0) {
            	continue;
            }
            if (jsfSetDataHead.dwAddrCount == 0) {
            	continue;
            }
            totalAddrCount += jsfSetDataHead.dwAddrCount;
            mapAddrCount.insert(std::make_pair(setId, jsfSetDataHead.dwAddrCount));
    	}
    	maxSetId = setId;
    	if (totalAddrCount == 0) {
    		return -1;
    	}

    	uint32_t findSetId = 32767;
    	uint32_t setRand = rand() % (totalAddrCount + 1);
    	std::map<int, uint32_t>::iterator it = mapAddrCount.begin();
    	uint32_t setSum = 0;
    	for (; it != mapAddrCount.end(); ++it) {
    		setSum += it->second;
    		if (setSum >= setRand) {
    			findSetId = it->first;
    			break;
    		}
    	}
    	if (findSetId > maxSetId) {
    		return -1;
    	}

        iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5AndMod(sSvcName, findSetId, uMod, jsfAddrInfo);
        if (iRet < 0) {
             snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                     "get l5 dest ip from config center fail. "
                     "line:%d, function:%s, ret:%d, setId:%d, Error:%s, svcname:[%s]",
                     __LINE__, __FUNCTION__, iRet, findSetId, wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
             return -1;
        }

        m_uSetId = findSetId;

    	return 0;
    }

    int GetValidAddrBySetID(const std::string& sSvcName,
            uint32_t setId, uint32_t uMod, wq::config::jsf::JsfAddrInfo &jsfAddrInfo) {
        int iRet = 0;
        iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5AndMod((const std::string &)m_sSvcName, setId, uMod, jsfAddrInfo);
        if (iRet == -1 || !CheckJsfAddrInfo(jsfAddrInfo)) {
            return 0;
        } else if (iRet < -1) {
             snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                     "get l5 dest ip from config center fail. "
                     "line:%d, function:%s, ret:%d, setId:%d, Error:%s, svcname:[%s]",
                     __LINE__, __FUNCTION__, iRet, setId, wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
             return -1;
        }

        wq::config::jsf::JsfSetDataHead jsfSetDataHead;
        iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5Head((const std::string &)m_sSvcName, setId, jsfSetDataHead);
        if (iRet < 0) {
             snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                     "get l5 head from config center fail. "
                      "line:%d, function:%s, ret:%d, setId:%d, Error:%s, svcname:[%s]",
                      __LINE__, __FUNCTION__, iRet, setId, wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
             return -1;
        }
        if (jsfSetDataHead.dwSuccessRate > (uint32_t)jsfSetDataHead.GetMinCrossSetLimit()) {
            return 1;
        }

        return -1;
    }

    int GetAddrBySetID(const std::string& sSvcName,
            uint32_t setId, uint32_t uMod, wq::config::jsf::JsfAddrInfo &jsfAddrInfo) {
        int iRet = 0;
        iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5AndMod((const std::string &)m_sSvcName, setId, uMod, jsfAddrInfo);
        if (iRet == -1 || !CheckJsfAddrInfo(jsfAddrInfo)) {
            return 0;
        } else if (iRet < 0) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                    "get l5 dest ip from config center fail. "
                     "line:%d, function:%s, ret:%d, setId:%d, Error:%s, svcname:[%s]",
                     __LINE__, __FUNCTION__, iRet, setId, wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
            return -1;
        } else {
            return 1;
        }
    }

    int GetFirstValidAddr(const std::string& sSvcName, uint32_t &maxSetId,
            uint32_t uMod, wq::config::jsf::JsfAddrInfo &jsfAddrInfo) {
        int iRet = 0;

        iRet = GetValidAddrBySetID(sSvcName, m_uSetId, uMod, jsfAddrInfo);
        if (iRet == 1) {
            return 0;
        }

        return GetAddrByRandSetId(sSvcName, maxSetId, uMod, jsfAddrInfo);
    }

    int GetFirstAddr(const std::string& sSvcName, uint32_t maxSetId,
            uint32_t uMod, wq::config::jsf::JsfAddrInfo &jsfAddrInfo) {
        int iRet = 0;

        iRet = GetAddrBySetID(sSvcName, m_uSetId, uMod, jsfAddrInfo);
        if (iRet == 1) {
            return 0;
        }

        return GetAddrByRandSetId(sSvcName, maxSetId, uMod, jsfAddrInfo, false);
    }

    int GetAddrBySvcName(const std::string& sSvcName, uint32_t uMod,
            wq::config::jsf::JsfAddrInfo &jsfAddrInfo) {
        int iRet = 0;
        if (m_sSvcName != "") {
            uint32_t uSetId = 0;
            wq::config::jsf::JsfSetDataHead jsfSetDataHead;
            iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5Head((const std::string &)m_sSvcName, uSetId, jsfSetDataHead);
            if (iRet != 0) {
                std::string errMsg = wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg();
                snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                        "get l5 head from config center fail. "
                        "line:%d, function:%s, ret:%d, setId:%d, Error:%s, svcname:[%s]",
                        __LINE__, __FUNCTION__, iRet, uSetId, errMsg.c_str(), m_sSvcName.c_str());
                return -1;
            }

            if (jsfSetDataHead.dwCallMode == 1) {  // set优先模式
                uint32_t maxSetId = MAX_SET_ID;
                iRet = GetFirstValidAddr(m_sSvcName, maxSetId, uMod, jsfAddrInfo);
                if(iRet != 0 && maxSetId >= 0) {
                    // 在Set优先的情况下，如果从set0到setmax都未能成功获取到可用率大于0的IP，则从头开始获取第一个IP。
                    iRet = GetFirstAddr(m_sSvcName, maxSetId, uMod, jsfAddrInfo);
                    if (iRet != 0) {
                        return iRet;
                    }
                } else {
                    return iRet;
                }
            }
            else if (jsfSetDataHead.dwCallMode == 3) {  // 按指定set
                iRet = GetAddrBySetID(sSvcName, m_uSetId, uMod, jsfAddrInfo);
                if (iRet == 1) {
                    return 0;
                } else {
                    return -1;
                }
            }
            else {
                iRet = wq::config::jsf::JSF_CONFIG_ACCESS->GetAddrL5AndMod((const std::string &)m_sSvcName, 0, uMod, jsfAddrInfo);
                if (iRet < 0) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "get l5 dest ip from config center fail. "
                            "line:%d, function:%s, ret:%d, Error:%s, svcname:[%s]",
                            __LINE__, __FUNCTION__, iRet, wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
                    return -1;
                }
                else if (!CheckJsfAddrInfo(jsfAddrInfo)) {
                    snprintf(m_szErrMsg, sizeof(m_szErrMsg),
                            "get l5 dest ip from config center fail (l5 ip is null). "
                            "line:%d, function:%s, ret:%d, Error:%s, svcname:[%s]",
                            __LINE__, __FUNCTION__, iRet, wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
                    return -1;
                }
                else if (iRet >= 0) {
                    m_uSetId = 0;
                    return 0;
                }
            }
        }
        return iRet;
    }

    // 上报
    int ModuleInvokeReport(const std::string & methodName) {
        if (m_sSvcName.empty()) {
            return 0;
        }
        int costTime = (m_stTimeEnd.tv_sec - m_stTimeStart.tv_sec) * 1000
                + (m_stTimeEnd.tv_usec - m_stTimeStart.tv_usec) / 1000;
        std::string jsfName = m_sSvcName + "#" + m_sSvcName + ":" + methodName;
        /*int ret = wq::config::jsf::JSF_CONFIG_ACCESS->ReportInvokeInfo(m_sSvcName, m_uSetId,
                m_uIp, m_iRemotePort, m_iResult, costTime, m_szErrMsg, m_sCallerName);*/
        wq::config::InvokeInfo oInvokeInfo;
        oInvokeInfo.iRet = m_iResult;
        oInvokeInfo.nInvokeBeginTime = m_stTimeStart.tv_sec * 1000000 + m_stTimeStart.tv_usec;
        oInvokeInfo.nInvokeEndTime = m_stTimeEnd.tv_sec * 1000000 + m_stTimeEnd.tv_usec;
        oInvokeInfo.nTraceId = m_ddwTraceId;
        oInvokeInfo.uCostTime = costTime;
        oInvokeInfo.sErrMsg = m_szErrMsg;
        if (!m_sCallerName.empty()) {
            oInvokeInfo.sCallerName = m_sCallerName;
        }
        else {
            oInvokeInfo.sCallerName = jsfName;
        }
        int ret = wq::config::jsf::JSF_CONFIG_ACCESS->ReportInvokeInfo(m_sSvcName, m_uSetId, m_uIp, m_iRemotePort, oInvokeInfo);
        if (ret != 0) {
            std::stringstream strErrMsg;
            strErrMsg << m_szErrMsg << " | " << "ReportInvokeInfo error."
                    << "svcname: [" << m_sSvcName.c_str() << "]," << "ip:"
                    << wq::config::jsf::JSF_CONFIG_ACCESS->GetStrIP(m_uIp) << "],"
            << "port:" << m_iRemotePort << "],"
            << "ret:" << ret << "],"
            << "error:[" << wq::config::jsf::JSF_CONFIG_ACCESS->GetErrMsg().c_str() << "]";
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "%s",
                    strErrMsg.str().c_str());
            return ret;
        }

        return 0;
    }

#elif defined __CONFIG_ACCESS__
    int GetAddrBySvcName(const std::string& sSvcName,
            uint32_t uSetId, uint32_t uMod, uint32_t& uIP, uint16_t& wPort)
    {
        int iRet = 0;

        iRet = wq::config::CONFIG_ACCESS->GetAddrL5AndMod(sSvcName, uSetId, uMod, uIP, wPort);
        if (iRet < 0) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "get dest ip from config center fail. ret:%d, Error:%s, svcname:[%s]",
                    iRet, wq::config::CONFIG_ACCESS->GetErrMsg().c_str(), m_sSvcName.c_str());
            return -1;
        }

        return 0;
    }

    int GetJSfConfigItem(const std::string& sSvcName, config &cfg) {
        int ret = 0;
        std::string strConfItem;
        ret = wq::config::CONFIG_ACCESS->GetConfItem(m_sSvcName, 0, strConfItem);
        if (ret != 0) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "get config item from config center fail. ret:%d, svcname:[%s]",
                    ret, m_sSvcName.c_str());
            return -1;
        }

        oad::util::XAccess oXAccess;
        oXAccess << strConfItem;
        int iRet = oXAccess.Query("/config", cfg);
        if (iRet != 0) {
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "parse jsf item fail.svcname:[%s],item:[%s]",
                    m_sSvcName.c_str(), strConfItem.c_str());
            return -1;
        }

        return 0;
    }

    int ModuleInvokeReport(const typename T::in &oReq) {
        // 上报kk
        if (m_sSvcName.empty()) {
            return 0;
        }
        int costTime = (m_stTimeEnd.tv_sec - m_stTimeStart.tv_sec) * 1000
        + (m_stTimeEnd.tv_usec - m_stTimeStart.tv_usec) / 1000;
        std::string jsfName = m_sSvcName + "#" + m_sSvcName + ":" + oReq.method;
        int ret = wq::config::CONFIG_ACCESS->ReportInvokeInfo(m_sSvcName, 0,
                m_uIp, m_iRemotePort, m_iResult, costTime, m_szErrMsg, m_sCallerName);
        if (ret != 0) {
            std::stringstream strErrMsg;
            strErrMsg << m_szErrMsg << " | "
            << "ReportInvokeInfo error."
            << "svcname: [" << m_sSvcName.c_str() << "],"
            << "ip:" << wq::config::CONFIG_ACCESS->GetStrIP(m_uIp) << "],"
            << "port:" << m_iRemotePort << "],"
            << "ret:" << ret << "],"
            << "error:[" << wq::config::CONFIG_ACCESS->GetErrMsg().c_str() << "]";
            snprintf(m_szErrMsg, sizeof(m_szErrMsg), "%s", strErrMsg.str().c_str());
        }
        return ret;
    }
#endif

private:
    int m_iTimeOutMs;
    int m_iTimeOut;
    struct timeval m_stTimeStart;
    struct timeval m_stTimeEnd;
    int m_iFd;
    std::string m_sServiceName;
    std::string m_sLocalIp;
    std::string m_sRemoteIp;
    int m_iRemotePort;
    std::string m_sSrcName;
    char m_szErrMsg[1024];
    std::string m_szStackMsg;
    bool m_isLongConnect;
    bool m_bInit;
    int m_iSetId;
    protocol_type_t m_eProtocol;
    static const uint32_t m_dwVersion = 1;  //版本号
    PkgHead m_oHead;
    char* m_pInnerBuf;
    uint32_t m_dwBufLen;
    bool m_isAutoChooseIp;
    std::string m_sCallerName;
private:
    int m_iResult;
    typename T::out * m_pResp;
    uint32_t m_dwOnlineDebug;
    std::string m_sSvcName;
#ifdef __JSF_CONFIG_ACCESS__
    uint32_t m_uSetId;
#elif defined __CONFIG_ACCESS__
    config m_jsfCfg;
#endif
    uint32_t m_uIp;
    bool m_isModuleInovke;
    bool m_mockFlag;
	std::string m_sMockIp;
	int m_iMockPort;
	std::string m_methodName;
	short m_jsfVersion;
	uint64_t m_ddwTraceId;
};
}
#endif

