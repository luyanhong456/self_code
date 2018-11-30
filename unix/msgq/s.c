#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
#define MAX_TEXT 512
#define BUFSIZE 1024
struct msg_st
{
	long int msg_type;
	char text[MAX_TEXT];
};
 int main()
 {
 	int running =1;
 	struct msg_st data;
 	char buffer[BUFSIZE];
 	int msgid = -1;
 	msgid=msgget((key_t)88, 0666 | IPC_CREAT);
 	printf("send msgid=%d\n",msgid);
 	if(msgid == -1)
 	{
 		printf("msgget failed with error: %d\n",errno );
 		exit(1);
 	}
 	while(running)
 	{
 		printf("Enter some text: ");
 		fgets(buffer,BUFSIZE,stdin);
 		data.msg_type == 1;
 		strcpy(data.text,buffer);
 		if(msgsnd(msgid, (void*)&data, MAX_TEXT,0)== -1)
 		{
 			printf("msgsnd failed!!\n");
 			exit(1);
 		}
 		if(strncmp(buffer,"end",3) == 0)
 			running = 0;
 		sleep(1);
 	}
 	return 0;
 }