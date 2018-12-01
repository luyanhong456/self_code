
#ifndef _HTTP_UTIL_VARS_H_
#define _HTTP_UTIL_VARS_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <stdint.h>

using namespace std;

#define PRINT_ENDL(b) ((b)?"\n":"")

/*
 * HTTP����
 */
class CHttpRequest
{
public:
    CHttpRequest():dwTimeout(5),
                   sHost(""),
                   sProxyAddr(""),
                   wProxyPort(80),
                   sPostData(""),
                   sCookie("")
    {
        vecHeader.clear();
    }
    ~CHttpRequest(){}
public:
    /**
     * ���ó�ʱʱ��
     */
    inline void SetTimeout(uint32_t dwTimeout)
    {
        this->dwTimeout = dwTimeout;
    }

    /**
     * ��ȡ��ʱʱ��
     */
    inline const uint32_t& GetTimeout()const
    {
        return this->dwTimeout;
    }

    /**
     *  ����������ַ
     */
    inline void SetHost(const string& sHost)
    {
        this->sHost = sHost;
    }

    /*
     * ��ȡhost��Ϣ
     */
    inline const string& GetHost()const
    {
        return this->sHost;
    }

    /**
     * ���ô����������ַ
     */
    inline void SetProxyAddr(const string& sProxyAddr)
    {
        this->sProxyAddr = sProxyAddr;
    }

    /**
     * ��ȡ�����������ַ
     */
    inline const string& GetProxyAddr()const
    {
        return this->sProxyAddr;
    }

    /**
     * ���ô���������˿�
     */
    inline void SetProxyPort(uint16_t wPort)
    {
        this->wProxyPort = wPort;
    }

    /**
     * ��ȡ����������˿�
     */
    inline const uint16_t& GetProxyPort()const
    {
        return this->wProxyPort;
    }

    /**
     * ����http����post����
     */ 
    inline void SetPostData(const string& sPostData)
    {
        this->sPostData = sPostData;
    }

    /**
     * ��ȡpost����
     */
    inline const string& GetPostData()const
    {
        return this->sPostData;
    }

    /**
     * ����cookie��Ϣ
     */
    inline void SetCookie(const string& sCookie)
    {
        this->sCookie = sCookie;
    }
    
    /**
     * ��ȡcooke
     */
    inline const string& GetCookie()const
    {
        return this->sCookie;
    }

    /**
     * ����httpͷ
     */
    inline void SetHeader(const string& sHeader)
    {
        vecHeader.push_back(sHeader);
    }
    
    /**
     * ��ȡhttp����ͷ
     */
    inline const vector<string>& GetHeader()const
    {
        return this->vecHeader;
    }

    /**
     * ����ַ������͵Ĳ���
     */
    inline const string ToString(bool bIndent=true)const
    {
        stringstream oss;
        oss << PRINT_ENDL(bIndent) << "[" << PRINT_ENDL(bIndent);
        oss << " host:" << sHost << PRINT_ENDL(bIndent);
        oss << " proxy ip:" << sProxyAddr << PRINT_ENDL(bIndent);
        oss << " proxy port:" << wProxyPort << PRINT_ENDL(bIndent);
        oss << " timeout:" << dwTimeout << PRINT_ENDL(bIndent);
        oss << " post data:" << sPostData << PRINT_ENDL(bIndent);

        oss << " header:";
        for(vector<string>::const_iterator cit=vecHeader.begin();
                        cit != vecHeader.end();++cit)
        {
            if(cit!=vecHeader.begin())
            {
                oss <<";";
            }

            oss << *cit;

        }
        oss << PRINT_ENDL(bIndent);

        oss << " cookie:" << sCookie << PRINT_ENDL(bIndent);

        oss <<"]" << PRINT_ENDL(bIndent);

        return oss.str();
    }
private:
    uint32_t dwTimeout;
    string   sHost; //host��ַ
    string   sProxyAddr; //�����ַ
    uint16_t wProxyPort; //����˿�
    string   sPostData; //���Ӳ���
    string   sCookie; //cookie����
    vector<string> vecHeader; //http header����
};

/**
 * HTTP ��Ӧ
 */
class CHttpResponse
{
public:
    CHttpResponse():dwHttpCode(0),sHttpContent("")
    {}
    ~CHttpResponse(){}
public:
    /**
     * ����http code
     */
    inline void SetHttpCode(uint32_t dwHttpCode)
    {
        this->dwHttpCode = dwHttpCode;
    }

    /**
     * ��ȡhttpcode
     */
    inline const uint32_t& GetHttpCode()const
    {
        return this->dwHttpCode;
    }


   inline uint32_t& GetHttpCode()
   {
       return this->dwHttpCode;
   }


    /**
     * ����http����
     */
    inline void SetHttpContent(const string& sContent)
    {
        this->sHttpContent = sContent;
    }

    /**
     * ��ȡhttp����
     */
    inline const string& GetHttpContent()const
    {
        return this->sHttpContent;
    }

    inline string& GetHttpContent()
    {
        return this->sHttpContent;
    }

    /**
     * ����ַ������͵Ĳ���
     */
    inline const string ToString(bool bIndent=true)const
    {
        stringstream oss;
        oss << PRINT_ENDL(bIndent) << "[" << PRINT_ENDL(bIndent);
        oss << " http code:" << dwHttpCode << PRINT_ENDL(bIndent);
        oss << " http content:" << sHttpContent << PRINT_ENDL(bIndent);
        oss <<"]" << PRINT_ENDL(bIndent);
        return oss.str();
    }


private:
    uint32_t dwHttpCode;    //http code
    string   sHttpContent; //http ����
};

#endif /* CURL_UTIL_VARS_H_ */
