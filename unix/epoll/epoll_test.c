#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>   
#include <stddef.h>
#include <sys/types.h>  

int main(void)
{
	epoll_event *ev;
	int maxevents = 20;
	int listener=1;
	int client;
	int kdpfd = epoll_create(100);
	int nfds = epoll_wait(kdpfd, ev, maxevents, -1);
	for (int n=0; n<nfds; ++n)
	{
		if(ev[n].data.fd == listener)
		{
			//如果是主socker的事件的话，则表示有新的连接进入，进行新的连接处理
			
			client = accept(listener, (struct sockaddr*)&local, &addrlen);
			if(client < 0)
			{
				perror("accept");
				continue;
			}
			//setnonblocking(client);//将新连接置于非阻塞模式
			ev[n].events = EPOLLIN | EPOLLET;//并且将新连接也加入epoll的监听队列
			evv.data.fd = client;
			if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, client, &ev[n])<0)
			{
				//设置好event之后，将这个新的event通过epoll_ctl加入到epoll的监听队列中，
				//通过EPOLL_CTL_ADD来添加一个新的epoll事件，通过EPOLL_CTL_DEL来减少一个epoll事件
				//通过EPOLL_CTL_MOD来改变有一个事件的监听方式
				fprintf(stderr, "epollerinserttionerror:fd=%d\n",client );
				return -1;
			}

		}
		else if (ev[n].events & EPOLLIN)
		{
			//如果是已经连接的用户，并且收到数据，那么进行读入
			int sockfd_r;
			if ((sockfd_r= ev[n].data.fd) < 0) continue;
			read(sockfd_r, buffer, maxSIZE);
			//修改sockfd_r上要处理的事件为EPOLLOUT;
			ev[n].data.fd = sockfd_r;
			ev[n].events = EPOLLOUT | EPOLLET;
			epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd_r, &ev[n]);
		}
		else if (ev[n].events & EPOLLOUT)
		{
			//如果有数据发送
			int sockfd_w = ev[n].data.fd;
			write(sockfd_w, buffer, sizeof(buffer));
			//修改sockfd_w上要处理的事件为EPOLLIN
			ev[n].data.fd = sockfd_w;
			ev[n].events = EPOLLIN | EPOLLET;
			epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd_w, &ev[n]);
		}
		//do_use_fd(e[n].data.fd);		
	}
	close();
	return 0;
}