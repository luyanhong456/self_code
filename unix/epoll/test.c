#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/un.h>
#include <sys/types.h> 

#define maxevents 20
#define maxSIZE 1024
#define name "./lu.socket"
int main(void)
{
	epoll_event *events;
	epoll_event ev;
	struct sockaddr_un c_addr;
	struct sockaddr_un s_addr;
	int listenfd, connfd, len;
	char buf[1024]={0};

	int kdpfd=epoll_create(100);
	int nfds = epoll_wait(kdpfd, events, maxevents, -1);
	listenfd= socket(AF_UNIX,SOCK_STREAM,0);

	//ev.data.fd=listenfd;
	//ev.events=EPOLLIN | EPOLLET;
	//s_addr.sun_family=AF_UNIX;   
    //strncpy(s_addr.sun_path, name, sizeof(s_addr.sun_path));

	//epoll_ctl(kdpfd,EPOLL_CTL_ADD,listenfd,&ev);

	//len=offsetof(struct sockaddr_un, sun_path) + strlen(s_addr.sun_path);
	//bind(listenfd, (struct sockaddr*)&s_addr, len); 
	//listen(listenfd,1);

	for (int n=0; n<nfds; ++n)
	{
		if(events[n].data.fd == listenfd )
		{
			len = sizeof(c_addr);
			connfd = accept(listenfd, (struct sockaddr*)&c_addr, (socklen_t*)&len);
			ev.data.fd = connfd;
			ev.events=EPOLLIN|EPOLLET;
			epoll_ctl(n,EPOLL_CTL_ADD,connfd, &ev);
		}
		else if (events[n].events & EPOLLIN)//读入
		{
			int sockfd_r;
			printf("EPOLLIN:\n");
			sockfd_r=events[n].data.fd;
			read(sockfd_r, buf,maxSIZE);
			printf("read:%s\n",buf );
			ev.data.fd=sockfd_r;
			ev.events = EPOLLOUT|EPOLLET;
			epoll_ctl(n,EPOLL_CTL_MOD, sockfd_r, &ev);
		}
		else if (events[n].events & EPOLLOUT)
		{
			int sockfd_w;
			printf("EPOLLOUT:\n");
			sockfd_w=events[n].data.fd;
			write(sockfd_w, buf, 1024);
			ev.data.fd=sockfd_w;
			ev.events=EPOLLIN | EPOLLET;
			epoll_ctl(n,EPOLL_CTL_MOD, sockfd_w, &ev);
		}
		else
			continue;

	}

	return 0;
}