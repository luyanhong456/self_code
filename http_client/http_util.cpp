/**************************************************************************
* Copyright (C), 1998-2050, Tencent.
*
* 模块名称 :
* 文 件 名 : http_util.cpp
* 功能描述 :
*
* 其它说明 :
*
* 修改历史 :
*   日  期               修改人                修改内容
*   2014-4-8            frankhuang            创建文件
*
**************************************************************************/

#ifndef __CURL_CURL_H
    #include <curl/curl.h>
#endif

#ifndef __CURL_EASY_H
    #include <curl/easy.h>
#endif

#include "http_util.h"
#include <string.h>




CHttpUtil::CHttpUtil()
{
    memset(m_sMsg, 0, MAX_MSG_SIZE+4);
}

CHttpUtil::~CHttpUtil()
{

}


/**
 * 初始化
 */
int CHttpUtil::Init()
{
    return 0;
}

/**
 * http 调用
 */
int CHttpUtil::HttpCall(const CHttpRequest& rRequest, CHttpResponse& rReponse)
{
    CURL* curl = curl_easy_init();
    curl_slist *http_headers = NULL;

    //host
    if(!rRequest.GetHost().empty())
    {
        curl_easy_setopt(curl, CURLOPT_URL, rRequest.GetHost().c_str());
    }

    //http header
    if(!rRequest.GetHeader().empty())
    {

        for(vector<string>::const_iterator it = rRequest.GetHeader().begin();
                        it != rRequest.GetHeader().end(); ++it)
        {
            if(it->empty())
            {
                continue;
            }

            http_headers = curl_slist_append(http_headers, it->c_str());
        }


        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
    }

    //proxy
    if(!rRequest.GetProxyAddr().empty())
    {
        curl_easy_setopt(curl, CURLOPT_PROXY, rRequest.GetProxyAddr().c_str());
        curl_easy_setopt(curl, CURLOPT_PROXYPORT, rRequest.GetProxyPort());
    }

    //request param
    if(!rRequest.GetPostData().empty())
    {
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rRequest.GetPostData().c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, rRequest.GetPostData().size());
    }

    //timeout
    if(rRequest.GetTimeout())
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, rRequest.GetTimeout());
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    }

    //cookies
    if(!rRequest.GetCookie().empty())
    {
        curl_easy_setopt(curl, CURLOPT_COOKIE, rRequest.GetCookie().c_str());
    }

    //call back
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CHttpUtil::WriteData);

    //data buffer
    //string sHttpContent = "";
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&rReponse.GetHttpContent());
    //rReponse.SetHttpContent(sHttpContent);

    //send http
    CURLcode ret = curl_easy_perform(curl);
    if(ret != CURLE_OK)
    {
        snprintf(m_sMsg,MAX_MSG_SIZE, "call curl_easy_perform failed!errno:%u errmsg:%s",
                ret,curl_easy_strerror(ret));
        curl_easy_cleanup(curl);
        curl_slist_free_all(http_headers);
        curl_global_cleanup();
        return ret;
    }

    //get httpcode
    ret = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &(rReponse.GetHttpCode()));
    if(ret != CURLE_OK)
    {
        snprintf(m_sMsg,MAX_MSG_SIZE, "call curl_easy_perform failed!errno:%u errmsg:%s",
                ret,curl_easy_strerror(ret));
        curl_easy_cleanup(curl);
        curl_slist_free_all(http_headers);
        curl_global_cleanup();
        return ret;
    }

    // release curl
    curl_easy_cleanup(curl);
    curl_slist_free_all(http_headers);
    curl_global_cleanup();
    return 0;
}

/**
 * 获取出错信息
 */
char* CHttpUtil::GetLastError()
{
    return m_sMsg;
}

/**
 * 写数据回调函数
 */
int CHttpUtil::WriteData(char* pszData,
                                uint32_t dwSize,
                                uint32_t dwMemBits,
                                std::string* psWriterData)
{
    if(pszData == NULL)
    {
        return 0;
    }
    if(psWriterData == NULL)
    {
        return 0;
    }

    int len = dwSize * dwMemBits;
    if ((psWriterData->size() + len) > MAX_CULR_WRITE_BUFFER_SIZE)
    {
        return 0;
    }

    psWriterData->append(pszData, len);
    return len;
}
