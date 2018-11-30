/**************************************************************************
* Copyright (C), 1998-2050, Tencent.
*
* ģ������ :
* �� �� �� : curl_util.h
* �������� :
*
* ����˵�� :
*
* �޸���ʷ :
*   ��  ��               �޸���                �޸�����
*   2014-4-5            frankhuang            �����ļ�
*
**************************************************************************/


#ifndef CURL_UTIL_H_
#define CURL_UTIL_H_

#include "http_util_struct.h"

#ifndef MAX_CULR_WRITE_BUFFER_SIZE
#define MAX_CULR_WRITE_BUFFER_SIZE 10*1024*1024
#endif

#define MAX_MSG_SIZE 1024  //������Ϣ����󳤶�

/**
 * ����curl���http����Э��api��װ
 *
 */
class CHttpUtil
{
public:
    CHttpUtil();
    ~CHttpUtil();
public:
    /**
     * ��ʼ��
     */
    int Init();

    /**
     * ����http���ݣ�����http��Ӧ
     */
    int HttpCall(const CHttpRequest& rRequest, CHttpResponse& rReponse);

    /**
     * ��ȡ���һ�γ�����Ϣ
     */
    char* GetLastError();
private:
    /**
     * callback�ص�����
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
 ʹ�÷�����
 1.��װcurl�Ŀ�����
 2.����
     ���ͷ�ļ�
     #include "http_util.h"
      ���ӣ� -lhttp_util -lcurl

  ���ӣ�
#include <iostream>
#include "http_util.h"


int main(int argc,char** argv)
{
    CHttpRequest oRequest;
    CHttpResponse oResponse;
    CHttpUtil ohttp;

    //��������
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
