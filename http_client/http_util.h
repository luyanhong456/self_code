/**************************************************************************
* Copyright (C), 1998-2050, Tencent.
*
* 模块名称 :
* 文 件 名 : curl_util.h
* 功能描述 :
*
* 其它说明 :
*
* 修改历史 :
*   日  期               修改人                修改内容
*   2014-4-5            frankhuang            创建文件
*
**************************************************************************/


#ifndef CURL_UTIL_H_
#define CURL_UTIL_H_

#include "http_util_struct.h"

#ifndef MAX_CULR_WRITE_BUFFER_SIZE
#define MAX_CULR_WRITE_BUFFER_SIZE 10*1024*1024
#endif

#define MAX_MSG_SIZE 1024  //错误消息的最大长度

/**
 * 基于curl库的http访问协议api封装
 *
 */
class CHttpUtil
{
public:
    CHttpUtil();
    ~CHttpUtil();
public:
    /**
     * 初始化
     */
    int Init();

    /**
     * 发送http数据，接受http响应
     */
    int HttpCall(const CHttpRequest& rRequest, CHttpResponse& rReponse);

    /**
     * 获取最后一次出错信息
     */
    char* GetLastError();
private:
    /**
     * callback回调函数
     */
    static int WriteData(char* pszData,
                         uint32_t dwSize,
                         uint32_t dwMemBits,
                         std::string* psWriterData);
private:
    char m_sMsg[MAX_MSG_SIZE+4];
};


#endif /* CURL_UTIL_H_ */

/****************************************************************
 使用方法：
 1.安装curl的开发库
 2.编译
     添加头文件
     #include "http_util.h"
      链接： -lhttp_util -lcurl

  例子：
#include <iostream>
#include "http_util.h"


int main(int argc,char** argv)
{
    CHttpRequest oRequest;
    CHttpResponse oResponse;
    CHttpUtil ohttp;

    //设置请求
    oRequest.SetHost("http://crm.man.jd.com/rest/customer/level/get");
    oRequest.SetProxyAddr("172.27.0.162");
    oRequest.SetProxyPort(9080);
    oRequest.SetPostData("{\"venderId\":10026}");

    int iRet = ohttp.HttpCall(oRequest, oResponse);
    if(iRet)
    {
        printf("call curl failed! ret = %u\n", iRet);
    }
    else
    {
        printf("call http sucess! httpcode=%u package:%s\n",
               oResponse.GetHttpCode(), oResponse.GetHttpContent().c_str());
    }

    return 0;
}

 ****************************************************************/
