
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
        // Ĭ�Ϲ��캯��
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

		// ���ڷ�������ַ�Ĺ��캯��
		CHttpClient( const char* szSvrAddr, uint16_t wSvrPort )
			: m_iFd(-1), m_sSvrAddr( szSvrAddr ), m_wSvrPort( wSvrPort )
			, m_dwTimeOutMs( 200 ), m_dwMethod( 0 ), m_dwStatus( 0 )
			, m_isLongConnect(false)
			, m_isAutoTry(false)
		{
			m_isSetSSLFlag = false;
			m_szRecvBuff = new char[MAX_RECVBUFF_SIZE];
		}

        // �����������ĵĹ��캯��
        CHttpClient(const char* szSvrName, const char* szSetName = ""
                  , uint32_t dwRouteKey = 0, const char* szLocalAddr = "", uint32_t dwTimeOutMs = 200);

		~CHttpClient( ) {
			if ( m_szRecvBuff ) {
				delete[] m_szRecvBuff;
				m_szRecvBuff = NULL;
			}
		}

        // ���÷�������ַ
        void SetSvrAddr(const std::string& sSvrAddr, uint16_t wSvrPort = 80, uint32_t dwTimeOutMs = 200)
        {
            m_sSvrAddr = sSvrAddr;
            m_wSvrPort = wSvrPort;
            m_dwTimeOutMs = dwTimeOutMs;
        }

        // ��ȡ��������ַ
        const char* GetSvrAddr()
        {
            return m_sSvrAddr.c_str();
        }

        // ��ȡ�������˿�
        uint16_t GetSvrPort()
        {
            return m_wSvrPort;
        }

        // ���õȴ���������Ӧʱ��
        void SetTimeOutMs(uint32_t dwTimeOutMs)
        {
            m_dwTimeOutMs = dwTimeOutMs;
        }

        // ����HTTP���󷽷�
        void SetReqMethod(uint32_t dwMethod)
        {
            m_dwMethod = dwMethod;
        }

        // ����HTTP����URI
        void SetReqUri(const std::string& sUri)
        {
            m_sUri  = sUri;
        }

		// ����HTTP����URI
        void SetHttpFlag(bool bFlag)
        {
           m_isSetSSLFlag = bFlag;
        }


        // ����HTTP����ͷ��
        void SetReqHeader(const char* szField, const char* szValue);

        // ��ȡHTTP����ͷ��
        const std::map<std::string, std::string>& GetReqHeader()
        {
            return m_mapReqHdr;
        }

        // ����POST����
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
        // ����POST BODY
        void SetReqBody(const std::string& sBody)
        {
            m_ssReqBody.str(sBody);
        }

        void SetLongConnect(bool isLongConnect, bool isAutoTry = false)
        {
            m_isLongConnect = isLongConnect;
			m_isAutoTry = isAutoTry;
        }		

        // ��ȡPOST BODY
        std::string GetReqBody()
        {
            return m_ssReqBody.str();
        }

        // ����HTTP����
        int BuildReq(std::string& sReq);

        // ����HTTP�������Ӧ
        int Invoke();
        
        // ͨ�� curl �ķ�ʽ���� http ����
        int CurlCall(const std::string &sUrl, const std::string &sCookie);

		// ͨ�� curl ���첽��ʽ���� http ����
		int CurlCallMulti( const std::vector<std::string> &vUrl );

        // ��ȡHTTPԭʼ��Ӧ
        const char* GetRespRaw()
        {
            return m_szRecvBuff;
        }

        // ��ȡHTTP��Ӧ״̬
        uint32_t GetRespStatus()
        {
            return m_dwStatus;
        }

        // ��ȡHTTP��Ӧͷ��
        const std::map<std::string, std::string>& GetRespHeader()
        {
            return m_mapRespHdr;
        }

        // ��ȡ��Ӧ����
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
        // ���HTTP����
        int CheckReq();

        // ����HTTP��Ӧ
        int ParseRespHead(const char* szBuff, uint32_t dwBuffSize);
        int ParseResp(const char* szBuff, uint32_t dwBuffSize);

        // ��ȡ�������ĵ�ַ
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

