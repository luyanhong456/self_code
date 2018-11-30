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
int main(int argc, char ** argv)
{
	int iListenFd=0, iConFd=0, iRet=0, iLen=0;
	char buf[1024]={0};
	struct sockaddr_un c_addr;
	struct sockaddr_un s_addr;
	std::string sLogPath="./s_unix";
	TLOG->Init(sLogPath.c_str());
	
	iListenFd=socket(PF_UNIX,SOCK_STREAM,0);
	if(iListenFd < 0)
	{
		TLOG_ERR("cannot create socket\n");
		return 0;
	}
	
	s_addr.sun_family = AF_UNIX;
	strncpy(s_addr.sun_path,name, sizeof(s_addr.sun_path));
	unlink(name);
	iRet = bind(iListenFd, (struct sockaddr*)&s_addr, sizeof(s_addr));
	if(iRet == -1)
	{
		TLOG_ERR("cannot bind serverv socket \n");
		return 0;
	}
	iRet = listen(iListenFd, 1);
	if (iRet == -1)
	{
		TLOG_ERR("cannot listen the client connect request\n");
		close(iListenFd);
		unlink(name);
		return 0;
	}
	
	iLen = offsetof(struct sockaddr_un, sun_path) + strlen(c_addr.sun_path);
	iConFd = accept(iListenFd, (struct sockaddr*)&c_addr, (socklen_t*)&iLen);
	if(iConFd <0)
	{
		TLOG_ERR("cannot accept client connect request\n");
		close(iListenFd);
		unlink(name);
		return 0;
	}
	printf("\n====== info ======\n");
	memset(buf,0,1024);
	read(iConFd,buf,sizeof(buf));
	printf("Message from client :%s\n",buf);
	close(iConFd);
	close(iListenFd);
	unlink(name);
	return 0; 

}
