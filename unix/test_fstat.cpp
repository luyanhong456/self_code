#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


int getModeType(int iStMode)
{
	int iRet = 0;
	#if 0
	iRet = S_ISLINGK(iStMode);      //判断是否位符号链接  
	if(iRet)
	{
		printf("st_mode[%d], is S_ISLINGK\n", iStMode);
		return 0;
	}
	#endif
	
	iRet = S_ISREG(iStMode);        //是否为一般文件 
	if(iRet)
	{
		printf("st_mode[%d], is S_ISREG\n", iStMode);
		return 0;
	}
	
	iRet = S_ISDIR(iStMode);       //是否为目录
	if(iRet)
	{
		printf("st_mode[%d], is S_ISDIR\n", iStMode);
		return 0;
	}
	
	iRet = S_ISCHR(iStMode);        //是否位字符装置文件 
	if(iRet)
	{
		printf("st_mode[%d], is S_ISCHR\n", iStMode);
		return 0;
	}
	
	//S_ISBLK(s3e);           //是否先进先出  
	iRet = S_ISSOCK(iStMode);       //是否为socket
	if(iRet)
	{
		printf("st_mode[%d], is S_ISSOCK\n", iStMode);
		return 0;
	}
	
}

#if 0
int main(int argc, char **argv)
{
	int iFd = 0;
	
	iFd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(iFd < 0)   
    {   
        printf("cannot create socket\n");   
        return 0;   
    }
	
	struct stat buf;
	int iRet = 0;
	iRet = fstat(iFd, &buf);
	printf("fstat: iRet[%d], iFd[%d],  errno[%d], errmsg[%s]\n", iRet, iFd, errno, strerror(errno));

	if(iRet != 0)
	{
		
		close(iFd);
		return 0;
	}

	
	printf("fstat buf, buf.st_size[%d], st_ino[%d], st_mode[%d]\n", buf.st_size, buf.st_ino, buf.st_mode);

	if(buf.st_mode & S_IFSOCK)
	{
		printf("S_IFSOCK\n");
	}
	getModeType(buf.st_mode);
	
	int iError = 0;
	int iLen = 0;
	errno = 0;
    iRet = getsockopt(iFd, SOL_SOCKET, SO_ERROR, &iError, (socklen_t *)&iLen);
	
	printf("getsockopt: iRet[%d], iFd[%d], errno[%d], errmsg[%s], iError[%d]\n", iRet, iFd, errno, strerror(errno), iError);

	close(iFd);
	return 0;
}
#endif

#if 1
int main(int argc, char **argv)
{
	int iFd = 0;
	if(argc <2)
	{
		printf("pls input fd\n");
		return 0;
	}
	
	iFd = atoi(argv[1]);
	
	struct stat buf;
	int iRet = 0;
	iRet = fstat(iFd, &buf);
	printf("iRet[%d], errno[%d], errmsg[%s]\n", iRet, errno, strerror(errno));

	if(iRet != 0)
	{
		return 0;
	}

	
	printf("fstat buf, buf.st_size[%d], st_ino[%d], st_mode[%d]\n", buf.st_size, buf.st_ino, buf.st_mode);

	if(buf.st_mode & S_IFSOCK)
	{
		printf("S_IFSOCK\n");
	}
	getModeType(buf.st_mode);
	
	int iError = 0;
	int iLen = 0;
	errno = 0;
    iRet = getsockopt(iFd, SOL_SOCKET, SO_ERROR, &iError, (socklen_t *)&iLen);
	
	printf("iRet[%d], errno[%d], errmsg[%s], iError[%d]\n", iRet, errno, strerror(errno), iError);
	
	return 0;
}
#endif
