/**************************************************************************
* Copyright (C), 1998-2050, Tencent.
*
* 模块名称 :
* 文 件 名 : curl_util_vars.h
* 功能描述 :
*
* 其它说明 :
*
* 修改历史 :
*   日  期               修改人                修改内容
*   2014-4-5            frankhuang            创建文件
*
**************************************************************************/


#ifndef _HTTP_UTIL_VARS_H_
#define _HTTP_UTIL_VARS_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <stdint.h>

using namespace std;

#define PRINT_ENDL(b) ((b)?"\n":"")

/*
 * HTTP请求
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
     * 设置超时时间
     */
    inline void SetTimeout(uint32_t dwTimeout)
    {
        this->dwTimeout = dwTimeout;
    }

    /**
     * 获取超时时间
     */
    inline const uint32_t& GetTimeout()const
    {
        return this->dwTimeout;
    }

    /**
     *  设置主机地址
     */
    inline void SetHost(const string& sHost)
    {
        this->sHost = sHost;
    }

    /*
     * 获取host信息
     */
    inline const string& GetHost()const
    {
        return this->sHost;
    }

    /**
     * 设置代理服务器地址
     */
    inline void SetProxyAddr(const string& sProxyAddr)
    {
        this->sProxyAddr = sProxyAddr;
    }

    /**
     * 获取代理服务器地址
     */
    inline const string& GetProxyAddr()const
    {
        return this->sProxyAddr;
    }

    /**
     * 设置代理服务器端口
     */
    inline void SetProxyPort(uint16_t wPort)
    {
        this->wProxyPort = wPort;
    }

    /**
     * 获取代理服务器端口
     */
    inline const uint16_t& GetProxyPort()const
    {
        return this->wProxyPort;
    }

    /**
     * 设置http请求post参数
     */ 
    inline void SetPostData(const string& sPostData)
    {
        this->sPostData = sPostData;
    }

    /**
     * 获取post参数
     */
    inline const string& GetPostData()const
    {
        return this->sPostData;
    }

    /**
     * 设置cookie信息
     */
    inline void SetCookie(const string& sCookie)
    {
        this->sCookie = sCookie;
    }
    
    /**
     * 获取cooke
     */
    inline const string& GetCookie()const
    {
        return this->sCookie;
    }

    /**
     * 设置http头
     */
    inline void SetHeader(const string& sHeader)
    {
        vecHeader.push_back(sHeader);
    }
    
    /**
     * 获取http报文头
     */
    inline const vector<string>& GetHeader()const
    {
        return this->vecHeader;
    }

    /**
     * 输出字符串类型的参数
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
    string   sHost; //host地址
    string   sProxyAddr; //代理地址
    uint16_t wProxyPort; //代理端口
    string   sPostData; //连接参数
    string   sCookie; //cookie参数
    vector<string> vecHeader; //http header数据
};

/**
 * HTTP 响应
 */
class CHttpResponse
{
public:
    CHttpResponse():dwHttpCode(0),sHttpContent("")
    {}
    ~CHttpResponse(){}
public:
    /**
     * 设置http code
     */
    inline void SetHttpCode(uint32_t dwHttpCode)
    {
        this->dwHttpCode = dwHttpCode;
    }

    /**
     * 获取httpcode
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
     * 设置http报文
     */
    inline void SetHttpContent(const string& sContent)
    {
        this->sHttpContent = sContent;
    }

    /**
     * 获取http报文
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
     * 输出字符串类型的参数
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
    string   sHttpContent; //http 报文
};

#endif /* CURL_UTIL_VARS_H_ */
