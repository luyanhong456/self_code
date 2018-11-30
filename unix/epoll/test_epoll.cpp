#include <string>
#include <stdint.h>
#include <stdio.h>
#include <sys/epoll.h>

int main(int argc, char **argv)
{
	int evt = 4;
	int i=0;
	i=evt & (EPOLLERR | EPOLLHUP);
	//EPOLLERR:8
	//EPOLLHUP:16
	printf("i[%d], EPOLLERR | EPOLLHUP=%d\n", i, EPOLLERR | EPOLLHUP);
	return 0;
}
