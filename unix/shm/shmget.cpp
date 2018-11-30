#include <string>
#include <iostream>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char **argv)
{
	int c,id,oflag;
	char *ptr;
	size_t length;
	oflag = SVSHM_MODE|IPC_CREATE;
	while((c=Gotopt(argc, argv, "e")) != -1)
	{
		switch(c)
		{
			case 'e':
				oflag != IPC_EXCL;
				break;
		}
	}

	if(optind != argc -2)
	{
		cout<<"usage: shmget[-e] <pathname> <length>" <<endl;
		return -1;
	}

	legth = atoi();
	return 0;
}
