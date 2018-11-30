#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>

void get_Next(std::string sSub, int next[])
{
	int iLen = sSub.length();
	
	int i=0, j=-1;
	next[0]= -1;
	
	printf("sSub:%s, iLen=%d\n", sSub.c_str(), iLen);//0-iLen
	
	while(i<iLen-1)
	{
		//printf("i=%d, j=%d, sSub[%d]=%c, sSub[%d]=%c\t", i, j, i, sSub[i], j, sSub[j]);
		if(j==-1 || sSub[i] == sSub[j])
		{
			++i;
			++j;
			next[i] = j;
			//printf("next[%d] = %d\n", i, j);
		}
		else
		{
			j = next[j];
			//printf("i=%d, j=%d \n", i, j);
		}
		//printf("\n");
	}
}

void get_Next_val(std::string sSub, int next[])
{	
	if(next == NULL)
	{
		printf("next is null\n");
		return ;
	}
	
	int iLen = sSub.length();
	printf("sSub:%s, iLen=%d\n", sSub.c_str(), iLen);//0-iLen
	
	int i=0, j=-1;
	next[0] = -1;
	
	while(i<iLen-1)
	{
		//printf("i=%d, j=%d, sSub[%d]=%c, sSub[%d]=%c\t", i, j, i, sSub[i], j, sSub[j]);
		if(j==-1 || sSub[i] == sSub[j])
		{
			++i;
			++j;
			if(sSub[i] != sSub[j])
			{
				next[i] = j;
			}
			else
			{
				next[i] = next[j];
			}
			//printf("next[%d] = %d\n", i, j);
		}
		else
		{
			j = next[j];
			//printf("i=%d, j=%d \n", i, j);
		}
		//printf("\n");
	}
}


//返回匹配成功的位置,-1表示没有匹配成功
int KMP(std::string str1, std::string str2)
{
	int i=0, j=0;
	int iLen1 = str1.length();
	int iLen2 = str2.length();
	
	printf("str1=%s, len1=%d, str2=%s, len2=%d\n", str1.c_str(), str1.length(), str2.c_str(), str2.length());
	
	//next[]
	int* next = new int[iLen2];
	for(int i=0; i<iLen2; i++)
	{
		next[i] = 0;
	}

	get_Next(str2, next);

	for(int i=0; i<iLen2; i++)
	{
		printf("next[%d]=%d\t", i, next[i]);
	}
	printf("\n");

	int iLoop=0;
	while(i<iLen1 && j<iLen2)
	{
		iLoop++;
		if(j==-1 || str1[i] == str2[j])
		{
			++i;
			++j;
		}
		else
		{
			j = next[j];
		}
	}
	printf("----------KMP, iLoop=%d---------------\n", iLoop);
	delete []next;
	
	if(j>=iLen2)
	{
		return i-iLen2;
	}
	
	return -1;
}

//char *StrStr(const char *src, const char *sub)
int StrStr(const char *src, const char *sub)
{
	const char *bp;
	const char *sp;

	if(src==NULL || sub==NULL)
	{
		return -1;
	}

	int i=0, iLoop=0;
	//暴力匹配O(n*m)
	while(*src)
	{
		bp = src;//主串
		sp = sub;//子串

		do
		{
			iLoop++;
			if(!*sp)
			{
				printf("StrStr:i=%d, iLoop=%d\n", i, iLoop);
				//return src;
				return i;
			}
			
		}while(*bp++ == *sp++);
		src += 1;
		i++;
		
		printf("StrStr:i=%d, iLoop=%d\n", i, iLoop);
		iLoop=0;
	}
	return -1;
}

char * CommonString(std::string &str1, std::string &str2)
{
	
	std::string shortstr, longstr;

	if(str1.empty() || str2.empty())
	{
		return NULL;
	}

	if(str1.length() < str2.length())
	{
		shortstr = str1;
		longstr = str2;
	}
	else
	{
		shortstr = str2;
		longstr = str1;
	}
	
	int i=0, j=0;
	int shortlen=0, longlen=0;
	shortlen = shortstr.length();
	longlen = longstr.length();
	
	printf("shortstr=%s, shortlen=%d, longstr=%s, longlen=%d\n", shortstr.c_str(), shortlen, longstr.c_str(), longlen);

	char *substr;
	substr = (char*)malloc(sizeof(char)*(shortlen+1));

	if(StrStr(longstr.c_str(), shortstr.c_str()) != -1)
	{
		shortstr.copy(substr, shortlen, 0);
		return substr;
	}
	
	for(i=shortlen-1; i>0; i--)
	{
		for(j=0; j<=shortlen-i; j++)
		{
			memcpy(substr, &shortstr[j], i);//将短字符串的一部分复制到substr
			substr[i] = '\0';//其长度逐渐减小
			#if 0
			if(strstr(longstr, substr) != NULL)//如果在longstr中能找到substr则返回substr
			{
				return substr;
			}
			#endif
			
			int pos = -1;
			pos = StrStr(longstr.c_str(), substr);
			printf("i=%d, j=%d, pos=%d, substr=%s\n", i, j, pos, substr);
			if(pos>=0)
			{
				printf("CommonString:pos=%d, substr=%s, longstr[%d]=%c, longstr[%d]=%c\n", pos, substr, i, longstr[i],pos-1, longstr[pos-1]);

				if(pos==0 && longstr[i]==',')
				{
					return substr;
				}

				if(pos != 0 && longstr[pos-1] == ',' && (longstr[pos+i-1] == ',' || longstr[pos+i] == '\0') )
				{
					return substr;
				}
			}
		}
	}

	return NULL;
}

char * CommonString_KMP(std::string &str1, std::string &str2)
{
	printf("==================CommonString_KMP=====================\n");
	std::string shortstr, longstr;

	if(str1.empty() || str2.empty())
	{
		return NULL;
	}

	if(str1.length() < str2.length())
	{
		shortstr = str1;
		longstr = str2;
	}
	else
	{
		shortstr = str2;
		longstr = str1;
	}
	
	int i=0, j=0;
	int shortlen=0, longlen=0;
	shortlen = shortstr.length();
	longlen = longstr.length();
	
	printf("shortstr=%s, shortlen=%d, longstr=%s, longlen=%d\n", shortstr.c_str(), shortlen, longstr.c_str(), longlen);

	char *substr;
	substr = (char*)malloc(sizeof(char)*(shortlen+1));

	if(StrStr(longstr.c_str(), shortstr.c_str()) != -1)
	{
		shortstr.copy(substr, shortlen, 0);
		return substr;
	}
	
	for(i=shortlen-1; i>0; i--)
	{
		for(j=0; j<=shortlen-i; j++)
		{
			memcpy(substr, &shortstr[j], i);//将短字符串的一部分复制到substr
			substr[i] = '\0';//其长度逐渐减小
			#if 0
			if(strstr(longstr, substr) != NULL)//如果在longstr中能找到substr则返回substr
			{
				return substr;
			}
			#endif
			
			int pos = -1;
			pos = KMP(longstr, substr);
			printf("i=%d, j=%d, pos=%d, substr=%s\n", i, j, pos, substr);
			if(pos>=0)
			{
				printf("CommonString_KMP:pos=%d, substr=%s, longstr[%d]=%c, longstr[%d]=%c\n", pos, substr, i, longstr[i],pos-1, longstr[pos-1]);

				if(pos==0 && longstr[i]==',')
				{
					return substr;
				}

				if(pos != 0 && longstr[pos-1] == ',' && (longstr[pos+i-1] == ',' || longstr[pos+i] == '\0') )
				{
					return substr;
				}
			}
		}
	}

	return NULL;
}



int main(int argc, char **argv)
{
	#if 0
	char *str1 = (char *)malloc(256);
	char *str2 = (char *)malloc(256);

	char *commonstr = NULL;
	gets(str1);
	gets(str2);
	#endif
	
	if(argc <3)
	{
		printf("pls input str1 & str2\n");
		return 0;
	}

	char *commonstr = NULL;
	
	std::string sStr1, sStr2;

	sStr1 = argv[1];
	sStr2 = argv[2];

	printf("str1=%s, len1=%d, str2=%s, len2=%d\n", sStr1.c_str(), sStr1.length(), sStr2.c_str(), sStr2.length());

	#if 0
	int pos = 0;
	pos = KMP(sStr1, sStr2);
	
	printf("pos=%d\n", pos);

	if(pos != -1)
	{
		
		std::string sCom;
		sCom = sStr1.substr(pos, sStr2.length());
		printf("pos=%d, sCom:%s\n", pos, sCom.c_str());
	}
	else
	{
		printf("not found\n");
	}
	#endif
	
	commonstr = CommonString(sStr1, sStr2);
	printf("commonstr=%s\n", commonstr);
	
	
	commonstr = CommonString_KMP(sStr1, sStr2);
	printf("KMP, commonstr=%s\n", commonstr);
	//char p[] = "12345";
	//char q[] = "34";

	//char *r = StrStr(p, q);
	//printf("r:%s\n", r);

	return 0;
}



#if 0
char *CommonString(char *str1, char *str2)
{
	int i=0, j=0;
	char *shortstr, *longstr;
	char *substr;

	if(NULL==str1 || NULL==str2)
	{
		return NULL;
	}

	if(strlen(str1)<=strlen(str2))
	{
		shortstr = str1;
		longstr = str2;
	}
	else
	{
		shortstr = str2;
		longstr = str1;
	}

	printf("shortstr=%s, longstr=%s\n", shortstr, longstr);

	if(strstr(longstr, shortstr) != NULL)
	{
		return shortstr;
	}

	substr = (char*)malloc(sizeof(char)*(strlen(shortstr)+1));

	for(i=strlen(shortstr)-1; i>0; i--)
	{
		for(j=0; j<=strlen(shortstr)-i; j++)
		{
			memcpy(substr, &shortstr[j], i);//将短字符串的一部分复制到substr
			substr[i] = '\0';//其长度逐渐减小
			#if 0
			if(strstr(longstr, substr) != NULL)//如果在longstr中能找到substr则返回substr
			{
				return substr;
			}
			#endif
			int pos = 0;
			pos = StrStr(longstr, substr);
			if(pos>=0)
			{
				printf("pos=%d\n", pos);

				if(pos==0 && longstr[i]==',')
				{
					return substr;
				}

				if(pos != 0 && longstr[pos-1] == ',' && longstr[i] == ',')
				{
					return substr;
				}
			}
		}
	}

	return NULL;
}
#endif
