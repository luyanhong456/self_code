#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>   
#include <sys/socket.h>   
#include <sys/un.h>   
#include <stdlib.h>
#include <stddef.h>
#include "tlog.h"
#define name "./lu.socket"
using namespace std;
using namespace boss;
int main(void)   
{   
    int iConnFd=0, iRet=0, iLen=0;     
    struct sockaddr_un s_addr; 
    std::string sLogPath="./c_unix"; 
    TLOG->Init(sLogPath.c_str());
	
    //调用socket函数创建通信所需的套接字
    iConnFd = socket(AF_UNIX, SOCK_STREAM, 0);   
    if(iConnFd < 0)   
    {   
        TLOG_ERR("cannot create socket\n");   
        return 0;   
    }
	
    s_addr.sun_family = AF_UNIX;   
    strncpy(s_addr.sun_path, name, sizeof(s_addr.sun_path));   
    iLen = offsetof(struct sockaddr_un, sun_path) + strlen(s_addr.sun_path);
	
    //调用connect函数来连接服务器
    iRet= connect(iConnFd, (struct sockaddr*)&s_addr, iLen);   
    if(iRet == -1)   
    {   
        TLOG_ERR("cannot connect to the server\n");   
        close(iConnFd);   
        return 0;   
    }
	
    char snd_buf[1024]={0};   
    memset(snd_buf, 0, sizeof(snd_buf));   
    strncpy(snd_buf,"the message from client.", 24);
	
    //通过write函数向服务器发生信息 
    write(iConnFd, snd_buf, sizeof(snd_buf));
    close(iConnFd);  
    return 0;   
}
