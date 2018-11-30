#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/msg.h>
#define BUFSIZE 1024
struct msg_st
{
	long int msg_type;
	char text[BUFSIZE];
};

int main()
{
	int running = 1;
	int msgid = -1;
	struct  msg_st data;
	long int msgtype = 0;
	msgid = msgget((key_t)88, 0666|IPC_CREAT);
	printf("\nreceive msgid=%d",msgid);
	if (msgid == -1)
	{
		printf("msgget failed with error:%d\n",errno );
		exit(1);
	}
	//msgrcv 从队列中获取消息，直到遇到end消息为止
	while (running)
	{
	
		if(msgrcv(msgid,(void*)&data,BUFSIZE,msgtype,0) == -1)
		{
			printf("msgrcv failed with error:%d\n",errno);
			exit(1);
		}
		printf("\nYou wrote:%s\n",data.text );
		//遇到end结束
		if(strncmp(data.text,"end",3) == 0)
			running=0;
	}
	//删除消息队列
	if( msgctl(msgid,IPC_RMID,0) == -1)
	{
		printf("msgctl(IPC_RMID) failed!!\n");
		exit(1);
	}

	return 0;

}
