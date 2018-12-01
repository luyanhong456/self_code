#include <string>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include "facility.h"

int main(int argc, char **argv)
{	
	std::string sFileName = "file";

	FILE *pFile;
	pFile = fopen(sFileName.c_str(), "w");

	if(pFile == NULL)
	{
		printf("fopen failed %s\n", sFileName.c_str());
		return -1;
	}

	while(1)
	{
		
		std::string sTime;
		sTime = boss::util::timestr();
		
		fprintf(pFile, "%s\n", sTime.c_str());		
		fflush (pFile);    
		
		//printf("sTime:%s\n", sTime.c_str());
		sleep(5);
	}
		
	fclose(pFile);
	
	return 0;
}


