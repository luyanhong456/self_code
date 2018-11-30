#include <stdio.h>     
#include <sys/types.h>     
#include <sys/socket.h>     
#include <netinet/in.h>     
#include <arpa/inet.h>   
#include <stdlib.h>  
#include <string>  
#include <sys/epoll.h>
#include <sys/un.h>
#include "tlog.h"  
#define BUFFER_SIZE 40  
#define MAX_EVENTS 10  
using namespace std;
using namespace boss; 

int main(int argc, char **argv)
{
	int iServerSockFd=0, iClientSockFd=0;// 服务器端套接字 客户端套接字 
    int iLen=0;
	
	struct sockaddr_in s_addr;   // 服务器网络地址结构体     
    struct sockaddr_in c_addr; // 客户端网络地址结构体         
    char buf[BUFFER_SIZE]={0};  // 数据的缓冲区

	bzero(&s_addr, sizeof(s_addr));   
    s_addr.sin_family = AF_INET;    //设置为AF_INET，即使用IP
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);// 服务器IP地址--允许连接到所有本地地址上     
    s_addr.sin_port = htons(8000); // 服务器端口号
    
	std::string sLogPath = "./epoll_log";
 	TLOG->Init(sLogPath.c_str());
	TLOG->SetConsoleShow(1);
	
	iServerSockFd = socket(AF_INET, SOCK_STREAM, 0);	
	if(iServerSockFd<0)     
    {       
        printf("socket failed\n");     
        return -1;     
    }  

	int iRet = 0;
	iRet = bind(iServerSockFd,(struct sockaddr *)&s_addr, sizeof(struct sockaddr));
	// 将套接字绑定到服务器的网络地址上  
    if(iRet< 0)     
    {     
        printf("bind failed\n");     
        return -1;     
    }  

	listen(iServerSockFd, 1); 
    int iEpollFd=0;
    iEpollFd = epoll_create(MAX_EVENTS);  
    if(iEpollFd==-1)  
    {  
        printf("epoll_create failed\n");  
        exit(1);  
    }  


	struct epoll_event ev;// epoll事件结构体  
    struct epoll_event events[MAX_EVENTS];// 事件监听队列  
    ev.events = EPOLLIN;  
    ev.data.fd = iServerSockFd; 

	// 向epoll注册iServerSockFd监听事件  
    if(epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iServerSockFd, &ev) == -1)  
    {  
        TLOG_ERR("epll_ctl:server_sockfd register failed.");  
        exit(1);  
    } 


	int nfds;
    // 循环接受客户端请求
	while(1)  
    {  
        // 等待事件发生  
        nfds=epoll_wait(iEpollFd, events, MAX_EVENTS,-1);  
        if(nfds==-1)  
        {  
            TLOG_ERR("start epoll_wait failed.");  
            continue;  
        } 
		printf("nfds[%d]\n", nfds);
		
        for(int i=0; i<nfds; ++i)  
        {  
            // 客户端有新的连接请求  
            if(events[i].data.fd == iServerSockFd)  
            {  
                // 等待客户端连接请求到达
                iLen = sizeof(c_addr);
                if((iClientSockFd = accept(iServerSockFd, (struct sockaddr *)&c_addr, (socklen_t*)&iLen))<0)  
                {     
                    TLOG_ERR("accept client_sockfd failed.");     
                    continue;  
                }  
				
                // 向epoll注册client_sockfd监听事件  
                ev.events = EPOLLIN | EPOLLET;  
                ev.data.fd = iClientSockFd;  
                if(epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iClientSockFd, &ev)==-1)  
                {  
                    TLOG_ERR("epoll_ctl:client_sockfd register failed.");  
                    continue;  
                }  
                //printf("accept client %s\n",inet_ntoa(c_addr.sin_addr));
                
                TLOG_MSG("connect from[%s], port[%d]",inet_ntop(AF_INET,&c_addr.sin_addr,buf, sizeof(buf)), ntohs(c_addr.sin_port)); 
            }  
            // 客户端有数据发送过来  
            else  
            {  
                iLen=read(iClientSockFd, buf, sizeof(buf));
                if(iLen < 0)  
                {  
                    TLOG_ERR("server receive from client failed.");  
                    continue;  
                }
				
                printf("server receive from client:%s\n", buf);
                strncpy(buf,"server have received your message.", 35);
				
                write(iClientSockFd,buf,sizeof(buf));  
            } 
        }
//       close(iClientSockFd);
    }
	 
	close(iClientSockFd);
    close(iServerSockFd);
	
	return 0;
}