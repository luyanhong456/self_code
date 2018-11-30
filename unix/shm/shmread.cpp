#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <map>
#include <errno.h>

#include <fstream>

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
/*
class CShmIndexCfg
{
public:
	CShmIndexCfg(){}
	~CShmIndexCfg(){}
	template<class AR> AR & serialize(AR &ar)
	{
		_SERIALIZE_(ar, mapShmIndexCfg);
		return ar; 
	}	
public:
	std::map<std::string, uint32_t> mapShmIndexCfg;
};
*/


int main(int argc, char** argv)
{
	int shm_id;
	key_t key;
	People *p;
	std::string sPathName = ".";

	key = ftok(sPathName.c_str(), 0);
	if(key == -1)
	{
		perror("ftok error");
		return -1;
	}
	printf("key=%d\n", key) ;
	
	int i=0;
	std::string sFileName="test_i";
	
	FILE * fpFile = fopen( sFileName.c_str(), "a+" );
	
	while(1)
	{
		++i;
		shm_id = shmget(key, 0, 0666);
		p = (People *)shmat(shm_id, NULL, 0);

		++i;
		fprintf(fpFile, "%d\n", i);

		//if(i==10000) break;
	}

	fclose( fpFile );

	#if 0
	shm_id = shmget(key, 0, 0666);

	if(shm_id == -1)
	{
		perror("shmget error");
		printf("errno[%d], errmsg[%s]\n", errno, strerror(errno));//errno[2], errmsg[No such file or directory]
		return -1;
	}
	
	printf("shm_id=%d\n", shm_id) ;
	
	p = (People *)shmat(shm_id, NULL, 0);
	
	printf("errno[%d], errmsg[%s]\n", errno, strerror(errno));//errno[2], errmsg[No such file or directory]
	
	for(i = 0; i<3; i++)
	{
		printf( "name:%s\n",(*(p+i)).name );
		printf( "age %d\n",(*(p+i)).age );
	}
	#endif
	
	#if 0
	if(shmdt(p) == -1)
	{
		perror("detach error");
		return -1;
	}
	#endif
	
	return 0 ;
}

