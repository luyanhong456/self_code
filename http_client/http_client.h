
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <sstream>
#include <map>
#include <stdint.h>
#include <vector>

#define  MAX_RECVBUFF_SIZE  10*1024*1024

enum _HttpMethod
{
    HTTPMETHOD_GET  = 0,
    HTTPMETHOD_POST = 1,
};

enum _HttpError
{
    HTTPERROR_SERVER    = 0x01,
    HTTPERROR_METHOD    = 0x02,
    HTTPERROR_URI       = 0x03,
    HTTPERROR_HOST      = 0x04,
    HTTPERROR_CONNECT   = 0x05,
    HTTPERROR_SEND      = 0x06,
    HTTPERROR_RECEIVE   = 0x07,
    HTTPERROR_PARSERESP = 0x08,
    HTTPERROR_CURL_PARM = 0x09,
    HTTPERROR_CURL_INIT = 0x0A,
    HTTPERROR_CURL_PERF = 0x0B,
};

class CHttpClient
{
    public:
        // 默认构造函数
        CHttpClient():  m_iFd(-1),m_wSvrPort(0)
                   , m_dwTimeOutMs(200)
                   , m_dwMethod(0)
                   , m_dwStatus(0)
                   , m_dwRespLenth(0)    
                   , m_isLongConnect(false)
                   , m_isAutoTry(false)
    {
    		m_isSetSSLFlag = false;
			m_szRecvBuff = new char[MAX_RECVBUFF_SIZE];
		}

		// 基于服务器地址的构造函数
		CHttpClient( const char* szSvrAddr, uint16_t wSvrPort )
			: m_iFd(-1), m_sSvrAddr( szSvrAddr ), m_wSvrPort( wSvrPort )
			, m_dwTimeOutMs( 200 ), m_dwMethod( 0 ), m_dwStatus( 0 )
			, m_isLongConnect(false)
			, m_isAutoTry(false)
		{
			m_isSetSSLFlag = false;
			m_szRecvBuff = new char[MAX_RECVBUFF_SIZE];
		}

        // 基于配置中心的构造函数
        CHttpClient(const char* szSvrName, const char* szSetName = ""
                  , uint32_t dwRouteKey = 0, const char* szLocalAddr = "", uint32_t dwTimeOutMs = 200);

		~CHttpClient( ) {
			if ( m_szRecvBuff ) {
				delete[] m_szRecvBuff;
				m_szRecvBuff = NULL;
			}
		}

        // 设置服务器地址
        void SetSvrAddr(const std::string& sSvrAddr, uint16_t wSvrPort = 80, uint32_t dwTimeOutMs = 200)
        {
            m_sSvrAddr = sSvrAddr;
            m_wSvrPort = wSvrPort;
            m_dwTimeOutMs = dwTimeOutMs;
        }

        // 获取服务器地址
        const char* GetSvrAddr()
        {
            return m_sSvrAddr.c_str();
        }

        // 获取服务器端口
        uint16_t GetSvrPort()
        {
            return m_wSvrPort;
        }

        // 设置等待服务器响应时间
        void SetTimeOutMs(uint32_t dwTimeOutMs)
        {
            m_dwTimeOutMs = dwTimeOutMs;
        }

        // 设置HTTP请求方法
        void SetReqMethod(uint32_t dwMethod)
        {
            m_dwMethod = dwMethod;
        }

        // 设置HTTP请求URI
        void SetReqUri(const std::string& sUri)
        {
            m_sUri  = sUri;
        }

		// 设置HTTP请求URI
        void SetHttpFlag(bool bFlag)
        {
           m_isSetSSLFlag = bFlag;
        }


        // 设置HTTP请求头部
        void SetReqHeader(const char* szField, const char* szValue);

        // 获取HTTP请求头部
        const std::map<std::string, std::string>& GetReqHeader()
        {
            return m_mapReqHdr;
        }

        // 设置POST数据
        void SetReqData(const char* szName, int iValue);
        void SetReqData(const char* szName, uint32_t dwValue);
        void SetReqData(const char* szName, uint64_t qwValue);
        void SetReqData(const char* szName, const char* szValue);
        void SetReqData(const char* szName, const std::string& sValue);

        void Reset()
        {
           m_mapReqHdr.clear();
           m_ssReqBody.clear();
           m_ssReqBody.str("");
           m_mapRespHdr.clear();
           m_sRespBody="";
           m_dwRespLenth=0;
        }
        // 设置POST BODY
        void SetReqBody(const std::string& sBody)
        {
            m_ssReqBody.str(sBody);
        }

        void SetLongConnect(bool isLongConnect, bool isAutoTry = false)
        {
            m_isLongConnect = isLongConnect;
			m_isAutoTry = isAutoTry;
        }		

        // 获取POST BODY
        std::string GetReqBody()
        {
            return m_ssReqBody.str();
        }

        // 构建HTTP请求
        int BuildReq(std::string& sReq);

        // 处理HTTP请求和响应
        int Invoke();
        
        // 通过 curl 的方式处理 http 请求
        int CurlCall(const std::string &sUrl, const std::string &sCookie);

		// 通过 curl 的异步方式处理 http 请求
		int CurlCallMulti( const std::vector<std::string> &vUrl );

        // 获取HTTP原始响应
        const char* GetRespRaw()
        {
            return m_szRecvBuff;
        }

        // 获取HTTP响应状态
        uint32_t GetRespStatus()
        {
            return m_dwStatus;
        }

        // 获取HTTP响应头部
        const std::map<std::string, std::string>& GetRespHeader()
        {
            return m_mapRespHdr;
        }

        // 获取响应数据
        const std::string& GetRespData()
        {
            return m_sRespBody;
        }
        
        const uint32_t &GetRespLength()
        {
            return m_dwRespLenth;
        }
		int Close();

    private:
        // 检查HTTP请求
        int CheckReq();

        // 解析HTTP响应
        int ParseRespHead(const char* szBuff, uint32_t dwBuffSize);
        int ParseResp(const char* szBuff, uint32_t dwBuffSize);

        // 获取配置中心地址
        int GetConfigSvc(const char* szSvrName, const char* szSetName, uint32_t dwRouteKey);
        
        // culr callback function
        static size_t WriteData(char *buffer, size_t size, size_t nmemb,  std::string  *userp);
    private:
		int         m_iFd;
        std::string m_sLocalAddr;
        std::string m_sSvrAddr;
        uint16_t    m_wSvrPort;
        uint32_t    m_dwTimeOutMs;

        uint32_t    m_dwMethod;
        std::string m_sUri;
        std::map<std::string, std::string> m_mapReqHdr;
        std::stringstream m_ssReqBody;
		char  *      m_szRecvBuff;
        uint32_t    m_dwStatus;
        std::string m_sMessage;
        std::map<std::string, std::string> m_mapRespHdr;
        std::string m_sRespBody;
        uint32_t    m_dwRespLenth;
		bool        m_isLongConnect;
		bool        m_isAutoTry;

		bool        m_isSetSSLFlag;
		
};

#endif  // #ifndef HTTP_CLIENT_H

