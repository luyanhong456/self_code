#include <stdio.h>    
#include <sys/types.h>    
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include <string>
#include <sys/un.h>  
#include <stdlib.h>  
#include <stddef.h>
#include <sys/un.h> 
#include "tlog.h"  
#define BUFFER_SIZE 1024
using namespace std;
using namespace boss;

int main(int argc, char **rgv)
{
	int dwClientSockFd = 0, dwLen=0;
	struct sockaddr_in s_addr; // 服务器端网络地址结构体     
    char buf[BUFFER_SIZE];  // 数据传送的缓冲区 

	std::string sLogPath = "./epoll_logs";
	TLOG->Init(sLogPath.c_str());
	TLOG->SetConsoleShow(1);
	
	memset(&s_addr,0,sizeof(s_addr)); // 数据初始化--清零     
    s_addr.sin_family = AF_INET; // 设置为IP通信     
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");// 服务器IP地址     
    s_addr.sin_port = htons(8000); // 服务器端口号     

	dwClientSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(dwClientSockFd<0)     
    {     
        TLOG_ERR("client socket creation failed.");     
        exit(1);  
    } 

	int iRet = 0;
	iRet = connect(dwClientSockFd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr));

	if(iRet < 0)
	{
		TLOG_ERR("connect to server failed.");     
        exit(1); 
	}

	while(1)
	{
		printf("Please input the message:");    
        scanf("%s", buf);

		write(dwClientSockFd, buf, sizeof(buf));
		
		// 接收服务器端信息
        dwLen = read(dwClientSockFd, buf, sizeof(buf)); 
        printf("client receive from server:%s\n",buf); 

		if(dwLen<0)  
        {  
            TLOG_ERR("client receive from server failed.");  
            continue;  
        }    
	}


	close(dwClientSockFd); 
	
	return 0;
}
