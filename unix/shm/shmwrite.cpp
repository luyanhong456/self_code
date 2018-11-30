#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
using namespace std;
class People
{
public:
		People():age(0){}
		~People(){}
	
public:
	char name[64];
	int age;

};


int main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	People *p;
	
	std::string sPathName = ".";
	key = ftok(sPathName.c_str(),0);
	if(key==-1)
	{
		perror("ftok error");
		return -1;
	}
	printf("key=%d\n", key) ;

#if 1
	shm_id=shmget(key, 0, 0);

	if(shm_id != -1)
	{
		int iRet = shmctl(shm_id, IPC_RMID, NULL);
		printf("shmctl, iRet[%d], shm_id[%d]\n", iRet, shm_id) ;
		printf("errno[%d], errmsg[%s]\n", errno, strerror(errno));
	}
#endif

	shm_id=shmget(key, 64, IPC_CREAT|IPC_EXCL|0666);
	if(shm_id == -1)
	{
		perror("shmget error");
		printf("errno[%d], errmsg[%s]\n", errno, strerror(errno));
		return -1;
	}
	
	printf("shm_id=%d\n", shm_id) ;
	
	p=(People*)shmat(shm_id, NULL, 0);
	std::string sTmp = "test";
	
	for(i=0; i<3; i++)
	{
		
		strncpy((p+i)->name, sTmp.c_str(), sTmp.length());
		printf("%s\n", sTmp.c_str());
		(p+i)->age=0+i;
	}
	
	//shmdt(p);

	return 0 ;
}

