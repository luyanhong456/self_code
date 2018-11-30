#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("pls input shmid\n");
		return 0;
	}

	int id = atoi(argv[1]);
	int iRet = shmctl(id, IPC_RMID, NULL);
	printf("iRet=%d\n", iRet);
	return 0;
}
