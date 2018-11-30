#include "tlog.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <sys/msg.h>
#include "tlog.h"

#define MAX_TEXT 512
#define BUFSIZE 1024

using namespace std;
using namespace boss;

struct msg_st
{
	long int msg_type;
	char text[MAX_TEXT];
};
 int main()
 {
 	char log_path[]="/home/luyanhong/test/log";
 	TLOG->SetConsoleShow(1);
 	int running =1;
 	struct msg_st data;
 	char buffer[BUFSIZE];
 	int msgid = -1;
 	msgid = msgget((key_t)1888, 0666 | IPC_CREAT);
 	TLOG_MSG("receive msgid=%d",msgid);
 	TLOG->SetConsoleShow(0);
 	TLOG->Init(log_path);
 	if(msgid == -1)
 	{
 		TLOG_ERR("msgget failed with error: %d\n",errno );
 		exit(1);
 	}
 	while(running)
 	{
 		printf("Send text: ");
 		fgets(buffer,BUFSIZE,stdin);
 		data.msg_type == 1;
 		strcpy(data.text,buffer);
 		if(msgsnd(msgid, (void*)&data, MAX_TEXT,0)== -1)
 		{
 			TLOG_ERR("msgsnd failed!!\n");
 			continue;
 		}
 		if(strncmp(buffer,"exit",4) == 0)
 			running = 0;
 		sleep(1);
 	}
 	return 0;
 }