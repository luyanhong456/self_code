#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <sys/types.h>
#include <sys/msg.h>

#include "tlog.h"
#define BUFSIZE 1024

using namespace std;
using namespace boss;
struct msg_st
{
	long int msg_type;
	char text[BUFSIZE];
};

int main(void)
{
	TLOG->SetConsoleShow(1);
	char log_path[]="/home/luyanhong/test/log";
	int running = 1;
	int msgid = -1;
	struct  msg_st data;
	long int msgtype = 0;
	msgid = msgget((key_t)88, 0666|IPC_CREAT);
	TLOG_MSG("receive msgid=%d",msgid);
	TLOG->SetConsoleShow(0);
 	TLOG->Init(log_path);
	if (msgid == -1)
	{
		TLOG_ERR("msgget failed with error:%d\n",errno );
		exit(1);
	}
	//msgrcv 从队列中获取消息，直到遇到end消息为止
	while (running)
	{
	
		if(msgrcv(msgid,(void*)&data,BUFSIZE,msgtype,0) == -1)
		{
			TLOG_ERR("msgrcv failed with error:%d\n",errno);
			continue;
		}
		printf("Receive text:%s",data.text );
		//遇到end结束
		if(strncmp(data.text,"exit",4) == 0)
			running=0;
	}
	//删除消息队列
	if( msgctl(msgid,IPC_RMID,0) == -1)
	{
		TLOG_ERR("msgctl(IPC_RMID) failed!!\n");
		exit(1);
	}

	return 0;

}
