
#include <string>
#include <map>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "tcplib.h"
#include "urlencode.h"
#include "config_client_set.h"
//#include "c2cplatform/component/configcenter/config_client_set.h"

#include "tlog.h"

#include "http_client.h"

#include <curl/curl.h>

#define  HEADER_LINE_END    "\r\n"
#define  HEADER_END         "\r\n\r\n"

using namespace std;
using namespace boss;

CHttpClient::CHttpClient( const char* szSvrName, const char* szSetName, uint32_t dwRouteKey, const char* szLocalAddr, uint32_t dwTimeOutMs ) {
    m_iFd           =   -1;
    m_wSvrPort      = 0;
    m_dwTimeOutMs   = dwTimeOutMs;
    m_dwMethod      = 0;
    m_dwStatus      = 0;
    m_isLongConnect = false;
	m_isAutoTry		= false;
    m_isSetSSLFlag  = false;
    m_szRecvBuff    = new char[MAX_RECVBUFF_SIZE];

    if ( szLocalAddr == NULL || strlen( szLocalAddr ) == 0 ) {
        char szIp[32] = { 0 };
        if ( GetLocalIp( szIp, sizeof( szIp ) ) < 0 ) {
            strncpy( szIp, "127.0.0.1", sizeof( szIp ) );
        }
        m_sLocalAddr = szIp;
    }
    else {
        m_sLocalAddr = szLocalAddr;
    }

    GetConfigSvc( szSvrName, szSetName, dwRouteKey );
}



void CHttpClient::SetReqHeader(const char* szField, const char* szValue)
{
    if(!szField || strlen(szField) == 0)
    {
        return;
    }

    m_mapReqHdr[szField] = szValue;
}

void CHttpClient::SetReqData(const char* szName, int iValue)
{
    if(!szName || strlen(szName) == 0)
    {
        return;
    }

    if(!m_ssReqBody.str().empty())
    {
        m_ssReqBody << "&";
    }

    m_ssReqBody << UrlEncodeString(szName) << "=" << iValue;
}

void CHttpClient::SetReqData(const char* szName, uint32_t dwValue)
{
    if(!szName || strlen(szName) == 0)
    {
        return;
    }

    if(!m_ssReqBody.str().empty())
    {
        m_ssReqBody << "&";
    }

    m_ssReqBody << UrlEncodeString(szName) << "=" << dwValue;
}

void CHttpClient::SetReqData(const char* szName, uint64_t qwValue)
{
    if(!szName || strlen(szName) == 0)
    {
        return;
    }

    if(!m_ssReqBody.str().empty())
    {
        m_ssReqBody << "&";
    }

    m_ssReqBody << UrlEncodeString(szName) << "=" << qwValue;
}

void CHttpClient::SetReqData(const char* szName, const char* szValue)
{
    if(!szName || strlen(szName) == 0)
    {
        return;
    }

    if(!m_ssReqBody.str().empty())
    {
        m_ssReqBody << "&";
    }
    m_ssReqBody << UrlEncodeString(szName) << "=" << UrlEncodeString(szValue);

}

void CHttpClient::SetReqData(const char* szName, const std::string& sValue)
{
    if(!szName || strlen(szName) == 0)
    {
        return;
    }

    if(!m_ssReqBody.str().empty())
    {
        m_ssReqBody << "&";
    }
    m_ssReqBody << UrlEncodeString(szName) << "=" << UrlEncodeString(sValue);
}

int CHttpClient::BuildReq(std::string& sReq)
{
    int iRet = CheckReq();
    if(iRet != 0)
    {
        return iRet;
    }

    sReq.clear();

    // URI
    if(m_dwMethod == HTTPMETHOD_POST)
    {
        sReq.append("POST ");
    }
    else
    {
        sReq.append("GET ");
    }
    sReq.append(m_sUri).append(" HTTP/1.1").append(HEADER_LINE_END);
	//sReq.append(m_sUri).append(" HTTP/1.0").append(HEADER_LINE_END);

    // 协议头部
    // HTTP/1.1 要求设置Host头部
    std::map<std::string, std::string>::iterator iter = m_mapReqHdr.find("Host");
    if(iter == m_mapReqHdr.end() || iter->second.empty())
    {
        sReq.append("Host").append(": ").append(m_sSvrAddr);
        if(m_wSvrPort != 80)
        {
            char szPort[16] = {0};
            snprintf(szPort, sizeof(szPort), "%u", m_wSvrPort);
            sReq.append(":").append(szPort);
        }
        sReq.append(HEADER_LINE_END);
    }

    // 基本头部默认值
    if(m_mapReqHdr.find("Connection") == m_mapReqHdr.end()) SetReqHeader("Connection", "keep-alive");
    if(m_mapReqHdr.find("Cache-Control") == m_mapReqHdr.end()) SetReqHeader("Cache-Control", "no-cache");
    if(m_mapReqHdr.find("Accept") == m_mapReqHdr.end()) SetReqHeader("Accept", "application/json, text/javascript");
    if(m_mapReqHdr.find("Accept-Charset") == m_mapReqHdr.end()) SetReqHeader("Accept-Charset", "gbk");

    for(std::map<std::string, std::string>::iterator iter = m_mapReqHdr.begin();
            iter != m_mapReqHdr.end(); iter++)
    {
        sReq.append(iter->first).append(": ").append(iter->second).append(HEADER_LINE_END);
    }

    // 补充协议头部
    if(m_dwMethod == HTTPMETHOD_POST)
    {
        sReq.append("Content-Type: application/x-www-form-urlencoded").append(HEADER_LINE_END);
        char szContentLen[16] = {0};
        snprintf(szContentLen, sizeof(szContentLen)-1, "%zu", m_ssReqBody.str().size());
        sReq.append("Content-Length: ").append(szContentLen).append(HEADER_LINE_END);
    }
    sReq.append(HEADER_LINE_END);

    // POST数据
    if(m_dwMethod == HTTPMETHOD_POST)
    {
        sReq.append(m_ssReqBody.str());
    }
	TLOG_MSG("sReq[%s]", sReq.c_str());
    return 0;
}

int CHttpClient::Invoke()
{
	
    int iRetry = 0;
    int iRet = 0;
    do
    {
		
        int iSuccess = 0;
        // 构建HTTP请求
        std::string sReq;
        iRet = BuildReq(sReq);
        if(iRet != 0)
        {
            return iRet;
        }

        TLOG_DBG("-----http req data:[\n%s\n]", sReq.c_str());

        // 连接服务器
        if ( m_iFd < 0 )
        {
            m_iFd = nTcpAsynConnectMs(const_cast<char *>(m_sSvrAddr.c_str()), m_wSvrPort, m_dwTimeOutMs);
            if(m_iFd < 0)
            {
                TLOG_MSG("connect svr:[%s][%u] fail, ret:[%d]", m_sSvrAddr.c_str(), m_wSvrPort, m_iFd);
                iSuccess = -1;
                iRet = HTTPERROR_CONNECT;
            }
        }

        if(iSuccess == 0 )
        {
            // 发送请求
            iRet = nTcpWriten(m_iFd, sReq.c_str(), sReq.size());
            if(iRet < 0)
            {	iSuccess = -2;
                iRet = HTTPERROR_SEND;
            }
        }

        if(iSuccess == 0)
        {
            // 接收并解析响应
            memset(m_szRecvBuff, 0, MAX_RECVBUFF_SIZE);

            // 1. read head
            int iHeadLength=0;
            int iRead=0;
            int iHeadTag=0;
            const int HEAD_END_TAG_LENG=4;
            while((iHeadTag<HEAD_END_TAG_LENG)&&(1==(iRead=nTcpReadnMs(m_iFd, m_szRecvBuff+iHeadLength, 1, m_dwTimeOutMs))))
            {
                ++iHeadLength;
                if(m_szRecvBuff[iHeadLength-1]=='\r' || m_szRecvBuff[iHeadLength-1]=='\n')
                {
                    ++iHeadTag;
                }
                else
                {
                    iHeadTag=0;
                    continue;
                }
            };
            if((1 != iRead) || (4 != iHeadTag))
            {
                TLOG_ERR("iread:[%d], iheadtag:[%d]", iRead, iHeadTag);
				iSuccess = -5;
                //return -1;
            }

            // read head finish. parse head
            
            iRet=ParseRespHead(m_szRecvBuff, iHeadLength+1);
            if(0 != iRet)
            {
                TLOG_ERR("parse resp fail. iRet:[%d]", iRet);
				iSuccess = -6;
				//return iRet;
            }
            // read resp data
            iRead=nTcpReadnMs(m_iFd, m_szRecvBuff+iHeadLength, GetRespLength(), m_dwTimeOutMs);
            if(iRead <= 0)
            {
                iSuccess = -3;
                iRet = HTTPERROR_RECEIVE;
                TLOG_MSG("read resp body fail. iRead:[%d], bodylength:[%u]", iRead, GetRespLength());
                //return 0;
            }
            TLOG_DBG("-----iRead:[%d], http respone data:[\n%s\n]. dataleng:[%d]", iRead, m_szRecvBuff,iHeadLength+iRead);
            if(iHeadLength+iRead > 0)
            { 
                iRet = ParseResp(m_szRecvBuff, iHeadLength+iRead);
                if(iRet < 0)
                {
                    TLOG_ERR("parse resp ret:[%d]", iRet);
                    iSuccess = -4;
                    iRet = HTTPERROR_PARSERESP;
                }
                // 执行成功
                iRet = 0;
            }

        }

        TLOG_DBG("iRet:[%d],iSuccess[%d],m_isLongConnect[%d],m_isAutoTry[%d]"
                 , iRet,iSuccess,m_isLongConnect,m_isAutoTry);
        // 长连接发包失败重连一次
        if ( iSuccess < 0 && m_isLongConnect&&m_isAutoTry ) {
            TLOG_ERR( "SendReq or RecvResp FAIL, ret:[%d]  retry:[%d]", iRet, iRetry + 1 );
            Close();
            continue;
        }

        break;
    }while(++iRetry <=1);

    // 关闭服务器连接
    if((0 != iRet) || ((!m_isLongConnect) && (m_iFd >= 0)))
    {
        Close();
    }

    return iRet;
}

int CHttpClient::CurlCall(const std::string &sUrl, const std::string &sCookie)
{
    //1. check param
    if(sUrl.empty())
    {
        TLOG_ERR("requre url is empty:[%s]", sUrl.c_str());
        return HTTPERROR_CURL_PARM;
    }

    CURL *curl = NULL;

    //2. init curl
    curl = curl_easy_init();
    if (!curl)
    {
        TLOG_ERR("fail to init curl");
        return HTTPERROR_CURL_INIT;
    }

    //    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);    // open detail info

    //3. set host
    //if ( m_sSvrAddr.empty( ) ) {
    //	TLOG_ERR( "svc addr:[%s] url:[%s]", m_sSvrAddr.c_str( ), sUrl.c_str( ) );

    //	if ( curl ) {
    //		curl_easy_cleanup( curl );       //always cleanup
    //		curl = NULL;
    //	}
    //	return HTTPERROR_CURL_INIT;
    //}

    if ( !m_sSvrAddr.empty( ) ) {
        char szProxy[64] = { 0 };
        snprintf( szProxy, sizeof( szProxy ), "%s:%u", m_sSvrAddr.c_str( ), m_wSvrPort );
        curl_easy_setopt( curl, CURLOPT_PROXY, szProxy );
        TLOG_DBG( "http proxy:[%s]", szProxy );
    }

    //4. set url & cookie
    curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());       // set url

    if(!sCookie.empty())
    {
        curl_easy_setopt(curl, CURLOPT_COOKIE, sCookie.c_str());       // set url
    }

    //5. set timeout
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL,1);     // 超时设置到毫秒，需要设置 CURLOPT_NOSIGNAL,屏蔽信号
    curl_easy_setopt(curl,CURLOPT_TIMEOUT_MS, m_dwTimeOutMs);

    //6. set follow location 1L
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    //7. set callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CHttpClient::WriteData);

    //8. set data buffer
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &m_sRespBody);

    if (m_isSetSSLFlag){
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }

    //9. perform the requre
    TLOG_DBG("http req. url:[%s]\ncookie:[%s]", sUrl.c_str(), sCookie.c_str());
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK)
    {
        TLOG_ERR("curl_easy_perform() fail, errmsg:[%s]", curl_easy_strerror(res));

        if(curl)
        {
            curl_easy_cleanup(curl);   //always cleanup
            curl = NULL;
        }
        return HTTPERROR_CURL_PERF;
    }
    TLOG_DBG("http response, content:[%s]", m_sRespBody.c_str());

    //8. get response code
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &m_dwStatus);

    //9. release curl
    if(curl != NULL)
    {
        curl_easy_cleanup(curl);   //always cleanup
        curl = NULL;
    }

    curl_global_cleanup();
    return 0;
}

int CHttpClient::CurlCallMulti( const std::vector<std::string> &vUrl ) {
    // 初始化一个multi curl 对象 //
    CURLM * curl_m = curl_multi_init( );
    if ( !curl_m ) {
        TLOG_ERR( "fail to init curl_m" );
        return HTTPERROR_CURL_INIT;
    }

    const int num = vUrl.size( );

    CURL * curl_array[num];

    for ( int i = 0; i < num; i++ ) {
        string sUrl = vUrl.at( i );
        curl_array[i] = NULL;

        if ( sUrl.empty( ) ) {
            TLOG_ERR( "requre url is empty:[%s]", sUrl.c_str( ) );
            continue;
        }

        curl_array[i] = curl_easy_init( );
        if ( !curl_array[i] ) {
            TLOG_ERR( "fail to init curl:[%s]", sUrl.c_str( ) );
            continue;
        }

        if ( !m_sSvrAddr.empty( ) ) {
            char szProxy[64] = { 0 };
            snprintf( szProxy, sizeof( szProxy ), "%s:%u", m_sSvrAddr.c_str( ), m_wSvrPort );
            curl_easy_setopt( curl_array[i], CURLOPT_PROXY, szProxy );
        }

        //4. set url
        curl_easy_setopt( curl_array[i], CURLOPT_URL, sUrl.c_str( ) );       // set url

        //5. set timeout
        curl_easy_setopt( curl_array[i], CURLOPT_NOSIGNAL, 1 );     // 超时设置到毫秒，需要设置 CURLOPT_NOSIGNAL,屏蔽信号
        curl_easy_setopt( curl_array[i], CURLOPT_TIMEOUT_MS, m_dwTimeOutMs );

        //6. set follow location 1L
        curl_easy_setopt( curl_array[i], CURLOPT_FOLLOWLOCATION, 1L );

        //7. set callback
        curl_easy_setopt( curl_array[i], CURLOPT_WRITEFUNCTION, CHttpClient::WriteData );

        //8. set data buffer
        curl_easy_setopt( curl_array[i], CURLOPT_WRITEDATA, &m_sRespBody );

        curl_multi_add_handle( curl_m, curl_array[i] );
    }

    struct timeval tv;
    tv.tv_sec = m_dwTimeOutMs / 1000;
    tv.tv_usec = ( m_dwTimeOutMs % 1000 ) * 1000;

    int running_handles;
    while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( curl_m, &running_handles ) );

    while ( running_handles ) {
        int max_fd = -1;
        fd_set fd_read;
        fd_set fd_write;
        fd_set fd_except;

        FD_ZERO( &fd_read );
        FD_ZERO( &fd_write );
        FD_ZERO( &fd_except );

        // 获取multi curl需要监听的文件描述符集合 fd_set //
        curl_multi_fdset( curl_m, &fd_read, &fd_write, &fd_except, &max_fd );
        if ( -1 == max_fd ) {
            TLOG_ERR( "curl_multi_fdset fail. max_fd=-1" );
            return -1;
        }

        //int retcode = select( max_fd + 1, &fd_read, &fd_write, &fd_except, &tv );
        //if ( retcode < 0 ) {
        //TLOG_ERR( "select error. max_fd:%d, retcode:%d timeout:%u ms", max_fd, retcode, m_dwTimeOutMs );
        //	break;
        //}

        // select监听到事件，调用curl_multi_perform通知curl执行相应的操作 //
        while ( CURLM_CALL_MULTI_PERFORM == curl_multi_perform( curl_m, &running_handles ) );
    }

    for ( int i = 0; i < num; i++ ) {
        if ( curl_array[i] ) {
            curl_multi_remove_handle( curl_m, curl_array[i] );
        }
    }
    for ( int i = 0; i < num; i++ ) {
        if ( curl_array[i] ) {
            curl_easy_cleanup( curl_array[i] );
        }
    }

    if ( curl_m != NULL ) {
        curl_multi_cleanup( curl_m );
    }

    curl_global_cleanup( );
    return 0;
}

int CHttpClient::CheckReq()
{
    // 检查服务器地址
    if(m_sSvrAddr.empty() || m_wSvrPort == 0)
    {
        return HTTPERROR_SERVER;
    }

    // 检查HTTP请求头部
    if(m_dwMethod != HTTPMETHOD_GET
            && m_dwMethod != HTTPMETHOD_POST)
    {
        return HTTPERROR_METHOD;
    }

    if(m_sUri.empty())
    {
        return HTTPERROR_URI;
    }

    return 0;
}

std::string HexDump(const char * pStr,int iLen)
{
    string sRet;
    unsigned char cT;
    const static char szHex[]={'0','1','2','3','4','5','6','7',
        '8','9','A','B','C','D','E','F'};
    for ( int i = 0 ; i < iLen; ++i )
    {
        cT = pStr[i];   
        sRet.push_back(szHex[(cT&0xF0)>>4]);
        sRet.push_back(szHex[(cT&0x0F)]);
        if ( i < iLen - 1 )sRet.push_back(' ');
    }
    return sRet;
}


int CHttpClient::ParseRespHead(const char* szBuff, uint32_t dwBuffSize)
{
    if(!szBuff || dwBuffSize == 0)
    {
        return -1;
    }
	TLOG_MSG("====szBuff[%s]=====", HexDump(szBuff, dwBuffSize).c_str());
    // 检查HTTP头部是否完整
    const char *p = strstr(szBuff, HEADER_END);
    const char *pBuffEnd = szBuff + dwBuffSize;
    if(p == NULL)
    {
        TLOG_ERR("find head end fail.");
        return -1;
    }
    if(p == NULL || p+4 > pBuffEnd)
    {
        return -1;
    }

    // 获取状态码和状态信息
    const char *pHdr = szBuff;
    const char *pHdrEnd = strstr(pHdr, HEADER_LINE_END);
    if(pHdrEnd == NULL || pHdrEnd - pHdr < 4 || 0 != strncmp(pHdr, "HTTP", 4))
    {
        return -1;
    }
    p = pHdr + 4;
    while(p < pHdrEnd && *p != ' ') ++p;
    if (p >= pHdrEnd)
    {
        return -1;
    }
    while(p < pHdrEnd && *p == ' ') ++p;
    if(pHdrEnd - p < 3 || !isdigit(p[0]) || !isdigit(p[1]) || !isdigit(p[2]))
    {
        return -1;
    }
    m_dwStatus = 100 * (p[0]-'0') + 10 * (p[1]-'0') + (p[2]-'0');

    p += 3;
    while(p < pHdrEnd && *p == ' ') ++p;
    m_sMessage = string(p, pHdrEnd - p);

    // 解析HTTP头部
    pHdr = pHdrEnd + 2;
    while(pHdr < pBuffEnd)
    {
        while(*pHdr == ' ' || *pHdr == '\t') ++pHdr;

        if(!pHdr[0] || pHdr[0] == '\r' || pHdr[0] == '\n')
        {
            break;
        }

        pHdrEnd = strstr(pHdr, HEADER_LINE_END);
        if(pHdrEnd == NULL)
        {
            return -1;
        }

        p = strchr (pHdr, ':');
        if(p == NULL)
        {
            return -1;
        }

        std::string sField(pHdr, p - pHdr);
        ++p;
        while(p < pHdrEnd && *p == ' ') ++p;
        std::string sValue(p, pHdrEnd - p);
        m_mapRespHdr.insert(pair<string,string>(sField, sValue));
        pHdr = pHdrEnd + 2;
    }

    // 跳过头部结束符
    while((pHdr < pBuffEnd) && (*pHdr == '\r' || *pHdr == '\n')) ++pHdr;

    // get resp lenth
    // Content-Length
    std::map<std::string, std::string>::iterator iterHdr = m_mapRespHdr.find("Content-Length");
    if(iterHdr == m_mapRespHdr.end())
    {
        iterHdr = m_mapRespHdr.find("Content-length");
    }
    if(iterHdr == m_mapRespHdr.end())
    {
        iterHdr = m_mapRespHdr.find("content-length");
    }
    if(iterHdr != m_mapRespHdr.end())
    {
        m_dwRespLenth= strtoul(iterHdr->second.c_str(), NULL, 10);
    }

    return 0;
};// parse head

int CHttpClient::ParseResp(const char* szBuff, uint32_t dwBuffSize)
{
	TLOG_MSG("====szBuff[%s]=====", HexDump(szBuff, dwBuffSize).c_str());
    if(!szBuff || dwBuffSize == 0)
    {
        return -1;
    }
	
    // 检查HTTP头部是否完整
    const char *p = strstr(szBuff, HEADER_END);
    const char *pBuffEnd = szBuff + dwBuffSize;
    if(p == NULL )
    {
        TLOG_ERR("find end fail");
        return -1;
    }
    if(p == NULL || p+4 > pBuffEnd)
    {
        return -1;
    }

    // 获取状态码和状态信息
    const char *pHdr = szBuff;
    const char *pHdrEnd = strstr(pHdr, HEADER_LINE_END);
    if(pHdrEnd == NULL || pHdrEnd - pHdr < 4 || 0 != strncmp(pHdr, "HTTP", 4))
    {
        return -1;
    }
    p = pHdr + 4;
    while(p < pHdrEnd && *p != ' ') ++p;
    if (p >= pHdrEnd)
    {
        return -1;
    }
    while(p < pHdrEnd && *p == ' ') ++p;
    if(pHdrEnd - p < 3 || !isdigit(p[0]) || !isdigit(p[1]) || !isdigit(p[2]))
    {
        return -1;
    }
    m_dwStatus = 100 * (p[0]-'0') + 10 * (p[1]-'0') + (p[2]-'0');

    p += 3;
    while(p < pHdrEnd && *p == ' ') ++p;
    m_sMessage = string(p, pHdrEnd - p);

    // 解析HTTP头部
    pHdr = pHdrEnd + 2;
    while(pHdr < pBuffEnd)
    {
        while(*pHdr == ' ' || *pHdr == '\t') ++pHdr;

        if(!pHdr[0] || pHdr[0] == '\r' || pHdr[0] == '\n')
        {
            break;
        }

        pHdrEnd = strstr(pHdr, HEADER_LINE_END);
        if(pHdrEnd == NULL)
        {
            return -1;
        }

        p = strchr (pHdr, ':');
        if(p == NULL)
        {
            return -1;
        }

        std::string sField(pHdr, p - pHdr);
        ++p;
        while(p < pHdrEnd && *p == ' ') ++p;
        std::string sValue(p, pHdrEnd - p);
        m_mapRespHdr.insert(pair<string,string>(sField, sValue));
        pHdr = pHdrEnd + 2;
    }

	/**
	 * 跳过头部结束符, 头部结束符为CRLF两个字符
	 *
	 * part of Hypertext Transfer Protocol -- HTTP/1.1
     * RFC 2616 Fielding, et al.
	 *
	 *     Response   = Status-Line               ;
     *                  *(( general-header        ;
     *                   | response-header        ;
     *                   | entity-header ) CRLF)  ;
     *                  CRLF
     *                  [ message-body ]          ;
	 */
	for( unsigned int i=0; i < 2 && (pHdr < pBuffEnd); i++ ){
		if(*pHdr == '\r' || *pHdr == '\n'){
			++pHdr;
		}  
	}
	/**
    while((pHdr < pBuffEnd) && (*pHdr == '\r' || *pHdr == '\n')) {
		++pHdr;
	}
	*/

    // get resp lenth
    // Content-Length    
    std::map<std::string, std::string>::iterator iterHdr = m_mapRespHdr.find("Content-Length");
    if(iterHdr == m_mapRespHdr.end())
    {
        iterHdr = m_mapRespHdr.find("Content-length");
    }
    if(iterHdr == m_mapRespHdr.end())
    {
        iterHdr = m_mapRespHdr.find("content-length");
    }
    if(iterHdr != m_mapRespHdr.end())
    {
        m_dwRespLenth= strtoul(iterHdr->second.c_str(), NULL, 10);
    }

    // 解析响应数据
    if(pHdr >= pBuffEnd)
    {
        // 无响应数据
        return 0;
    }
    // 设置了头部Content-Length时验证数据长度
    if(m_dwRespLenth != uint32_t(pBuffEnd - pHdr))
    {
        return -1;
    }
    // 若设置了头部Transfer-Encoding: chunked
    iterHdr = m_mapRespHdr.find("Transfer-Encoding");
    if(iterHdr != m_mapRespHdr.end() && iterHdr->second.compare("chunked") == 0)
    {
        uint32_t dwChunkSize = 0;
        do
        {
            if(!pHdr[0] || pHdr[0] == '\r' || pHdr[0] == '\n')
                break;

            pHdrEnd = strstr(pHdr, HEADER_LINE_END);
            if(pHdrEnd == NULL)
            {
                return -1;
            }

            std::string sChunkSize = std::string(pHdr, pHdrEnd - pHdr);
            dwChunkSize = strtoul(sChunkSize.c_str(), NULL, 16);
            if(pHdrEnd + 2 + dwChunkSize > pBuffEnd)
            {
                return -1;
            }

            m_sRespBody.append(pHdrEnd+2, dwChunkSize);

            pHdr = pHdrEnd + 2;
        }while(dwChunkSize != 0 && pHdr < pBuffEnd);
    }
    else
    {
        m_sRespBody.assign(pHdr, pBuffEnd - pHdr);
    }

    // 暂不支持编码
    iterHdr = m_mapRespHdr.find("Content-Encoding");
    if(iterHdr != m_mapRespHdr.end())
    {
        return -1;
    }

    return 0;
}


int CHttpClient::GetConfigSvc(const char* szSvrName, const char* szSetName, uint32_t dwRouteKey)
{
    if(szSvrName == NULL || strlen(szSvrName) == 0 || CONFIG_SET->Initialize() != 0)
    {
        return -1;
    }

    int iSetId = 0;
    if((szSetName != NULL && strlen(szSetName) != 0)
            && (!m_sLocalAddr.empty()))
    {
        struct in_addr stAddr;
        inet_pton(AF_INET, m_sLocalAddr.c_str(), &stAddr);
        iSetId = CONFIG_SET->GetSetIdBySvcAndIp(szSetName, stAddr.s_addr);
    }
    if(iSetId < 0)
    {
        iSetId = 0;
    }

    const CInterAddr oAddr = CONFIG_SET->getServiceAddressBySet(szSvrName, iSetId, dwRouteKey);
    m_sSvrAddr = oAddr.getHost();
    m_wSvrPort = oAddr.getPort();
    if(m_sSvrAddr.empty() || m_wSvrPort == 0)
    {
        return -1;
    }

    return 0;
}


size_t CHttpClient::WriteData(char *buffer, size_t size, size_t nmemb, std::string *userp)
{
    if((NULL == buffer) || (NULL == userp))
    {
        return 0;
    }

    size_t realsize = size * nmemb;
    if(MAX_RECVBUFF_SIZE < realsize)
    {
        return 0;
    }
    userp->append((const char *)buffer, realsize);
    return realsize;
}
int CHttpClient::Close()
{
	nTcpClose(m_iFd);
     m_iFd = -1;
	 return 0;
}
