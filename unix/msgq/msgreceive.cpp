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


struct stMsgBuf
{
	long    mtype;
	//char 	str[8192 - 8];
	char 	str[32768];

};


int main(void)
{
	TLOG->SetConsoleShow(1);
	char log_path[]="test.log";
	
	int msgid = -1;
	
	msgid = msgget(0x15070201, IPC_CREAT | 0666);
	
	TLOG_MSG("receive msgid=%d", msgid);
	TLOG->SetConsoleShow(0);
	
 	TLOG->Init(log_path);
	
	if (msgid == -1)
	{
		TLOG_ERR("msgget failed with error:%d\n", errno );
		exit(1);
	}

	stMsgBuf stBuf;

	size_t nRecSize = msgrcv(msgid, &stBuf, sizeof(stBuf) - sizeof(long), 0, IPC_NOWAIT);
	TLOG_MSG("nRecSize[%d]", nRecSize);
	
	if (nRecSize == ENOMSG)
	{
		TLOG_ERR("ENOMSG");
		return -1;
	}

	if (nRecSize <= 0  || nRecSize == E2BIG || nRecSize == EACCES )
	{
		TLOG_ERR("nRecSize:[%d]", nRecSize);
		return -1;
	}

	std::string sReq = stBuf.str;

	
	
	#if 0
	CDataRpt oDataRpt;
	try
	{
		sReq >> oDataRpt;
	}
	catch (...)
	{
		TLOG_ERR("sReq >> oDataRpt Failed sReq:[%s],nRecSize:[%d]", sReq.c_str(), nRecSize);
		std::string sJsoAnaFalFile;
		sJsoAnaFalFile.assign(g_dat_path_err).append(json_analyse_false);

		FILE * file = fopen(sJsoAnaFalFile.c_str(), "a");
		fprintf(file, "{\"cData\":%s}\n", sReq.c_str());
		TLOG_ERR("Write to [%s]:[{\"cData\":%s}\n]", sJsoAnaFalFile.c_str(), sReq.c_str());
		fclose(file);
	}
	#endif

	TLOG_MSG("sReq[%s]", sReq.c_str());

		

	return 0;

}


#if 0
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
#endif

