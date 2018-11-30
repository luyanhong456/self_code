#include <string>
#include <stdio.h>
#include <vector>


int DumpData_1(std::vector<std::string> vStr1, std::vector<std::string> vStr2, int **c)
{
	#if 0
	if(c==NULL)
	{
		return -1;
	}

	int iSize1=0, iSize2=0;
	iSize1 = vStr1.size();
	iSize2 = vStr2.size();

	if(iSize1 == 0 && iSize2 == 0)
	{
		return -1;
	}
	int q=0, p=0;

	for(q=0; q<iSize2; ++q)
	{
		printf("%d\t", q);
	}
	printf("\n");
	
	for(q=0; q<iSize2; ++q)
	{
		if(q==0) printf(" \t");
		printf("%s\t", vStr2[q]);
	}
	printf("\n");
	

	//输出c数组情况
	
	for(q=0; q<iSize1+1; ++q)
	{
		if(q==0) printf("0 \t");
		for(p=0; p<iSize2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}

	printf("=======================================\n");

 #endif
	
	return 0;
}


//中文状态下utf8编码
int RecognizeCharacter(const std::string &sStr, std::vector<std::string> &v)
{
	if(sStr.empty())
	{
		return 0;
	}
	

	int nBytes = 0, nSymbol=0;
	unsigned char chr;
	std::string::size_type length = sStr.size();
	std::string sDest;
	printf("ClearUtf8Symbol: sStr[%s], length=%d\n", sStr.c_str(), length);
	
	for(std::string::size_type i = 0; i < length; ++i)
	{
		chr = sStr[i];

		if( nBytes == 0)
		{
		   //下面的if是判断下该中文utf8编码一共占了几个字节，最多占6位
			if(chr >= 0x80)
			{
				if(chr >= 0xC0)// 11000000 =< chr 2byte
				{
					nBytes = 2;
				}
				
				if(chr >= 0xE0)// 11100000 =< chr 3byte
				{
					nBytes = 3;
				}
				
				if(chr >= 0xF0)// 11110000 =< chr 4byte
				{
					nBytes = 4;
				}
				
				if(chr >= 0xF8)// 11111000 =< chr 5byte
				{
					nBytes = 5;
				}
				
				if(chr >= 0xFC && chr <= 0xFD)// 11111100 =< chr <= 11111101 6byte
				{
					nBytes = 6;
				}
			
			}
			else
			{
				nBytes = 1;
			}
		}

		std::string sTmp;
		nSymbol = nBytes;
		sTmp += chr;
		nBytes--;
		for(int t=1; t<nSymbol; ++t)
		{
			sTmp += sStr[++i];
			nBytes--;
		}
		
		//printf("sTmp[%s]\n", sTmp.c_str());
		v.push_back(sTmp);

	}

    return 0;
}

void print_str(std::vector<std::string> v)
{
	printf("str: ");
	for(int i=0; i<v.size(); i++)
	{
		printf("%s", v[i].c_str());
	}
	printf("\n");
	return;
}

int longest_common_substring(std::vector<std::string> vStr1, std::vector<std::string> vStr2)  
{ 
	//动态规划算法
	
    int i,j,k,len1,len2,max,x,y;  

	len1 = vStr1.size();
	len2 = vStr2.size();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//第0列都初始化为0
	}
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//第0行都初始化为0   
	}


	//输出c数组情况
	int q=0, p=0;
	#if 0
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}

	printf("=======================================\n");
	#endif
	
    max = -1;  
    for(i = 1; i < len1+1; i++)//str1
    {  
        for(j = 1; j < len2+1; j++)//str2 
        {  
        	#if 0
            if(vStr1[i-1]==vStr2[j-1])
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else
            {
            	c[i][j]=0;
            }
			#endif

			#if 0 //for 分割符,
			if(vStr1[i-1] == vStr2[j-1])
			{
				//c[i][j]=c[i-1][j-1]+1;
				if(c[i-1][j-1] != -1)
            	{
            		c[i][j]=c[i-1][j-1]+1;
            	}
            	else
            	{
            		c[i][j] = 0+1;
            	}
			}
			else if(vStr1[i-1] == "@")
			{
				c[i][0]=-1;
				for(int t=0; t<len2+1; t++)
				{
					c[i][t]=-1;
				}
				continue;
			}
			else
            {
            	c[i][j]=0;
            }
			#endif

			//去掉分割符判断最大子串
               
            if(c[i][j] >= max)  
            {  
                max = c[i][j];  
                x = i;  
                y = j;  
            }  
        }  
    } 
	printf("x=%d, y=%d, max=%d\n", x, y, max);
	
	//输出c数组情况
	//int q=0, p=0;
	printf("=====after, printf c_array msg:======\n");
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}
		
	
    //输出公共子串  
    //char s[1000];
	std::string s, sTmp;
    k = max;  
    i=x-1, j=y-1;  
    //s[k--]='\0';

	printf("i=%d, j=%d, max=%d\n", i, j, max);
    while(i>=0 && j>=0)  
    {  
        if(vStr1[i] == vStr2[j]) //从后往上查的
        {  
            //s[k--] = vStr1[i];
            sTmp = vStr1[i];
            sTmp.append(s);
			s = sTmp;
            i--;  
            j--;  
        }  
        else       //只要有一个不相等，就说明相等的公共字符断了，不连续了  
        {
            break;
		}
    }
	
    printf("Longest-Common-SuString:%s\n", s.c_str());   
	
    for(i = 0; i < len1+1; i++)         //释放动态申请的二维数组
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
} 

int longest_common_subsequence(std::vector<std::string> vStr1, std::vector<std::string> vStr2)  
{ 
	//动态规划算法
    int i,j,k,len1,len2,max,x,y;  

	len1 = vStr1.size();
	len2 = vStr2.size();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//第0列都初始化为0
	}
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//第0行都初始化为0   
	}

	std::vector<int> vPos;
	vPos.push_back(0);
    max = -1;  
    for(i = 1; i < len1+1; i++)//str1
    {	
    	if(vStr1[i-1] == "@")
		{
			vPos.push_back(i);	
			for(j=1; j<len2+1; j++)
			{
				c[i][j] = 0;
			}
			continue;
		}
		
        for(j = 1; j < len2+1; j++)//str2 
        {  
        	//subsequence
        	if(vStr1[i-1]==vStr2[j-1])
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else if(c[i][j-1] > c[i-1][j])
            {
            	c[i][j]=c[i][j-1];
            }
			else
			{
				c[i][j]=c[i-1][j];
			}
        }
    } 

	
	//输出c数组情况
	int q=0, p=0;
	
	for(int x=0; x<vPos.size(); ++x)
	{
		int d = vPos[x];
		printf("@=%d\n", d);
		//c[d][0] = -1;
	}
	
	
	printf("\n=====DumpData_1 after, vPos c_array msg:======\n");
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}
		
	#if 1
	//vPos一定是从小到大的
    //输出公共子序列，按分割符号输出
    
    std::string s,sTmp, sDest;  
	int iPos = vPos.size();
	printf("iPos=%d\n", iPos);
	
	int i1=0, i2=0;
	j = len2;

	for(int x=0; x<iPos; x++)//逆序处理
	{
		i1=0, i2=0, k=0, max=0;
		
		if(x+1 == iPos || vPos[x]==len1)//vPos[iSize-1]一定是最后一行的
		{
			break;
		}

		i1 = vPos[x];
		i2 = vPos[x+1];
		j = len2;
		
		max = i2-(i1+1);//i1和i2之间行，每一行都取一个
		printf("x=%d, i1=%d, i2=%d, j=%d, max=%d, len1=%d\n", x, i1, i2, j, max, len1);

		i=i2-1;//取分隔符的上一行
		
		if(i>len1) break;

		s = "";
		sTmp = "";
		
		while(i>i1 && j>0 && max>0)
		{
			
			printf("i=%d, j=%d, vStr1[i-1]=%s, vStr2[j-1]=%s \n", i, j, vStr1[i-1].c_str(), vStr2[j-1].c_str());
			if(vStr1[i-1] == vStr2[j-1] && c[i][j]==c[i-1][j-1]+1 )
			{	
				sTmp = vStr1[i-1];
	            sTmp.append(s);
				s = sTmp;
				
				--j;
				--i;
				--max;
				
			}else if(vStr1[i-1] != vStr2[j-1] && c[i-1][j] > c[i][j-1])//上>左
			{
				//--i;//往上
				break;
			}
			else
			{
				--j;//往左
			}
		}
		printf("max=%d, k=%d, s:%s\n\n", max, k, s.c_str());
		
		#if 1
		
		if(max==0)
		{
			sDest.append(s).append("@");
		}
		#endif
		
	}
	
    printf("Longest-Common-subsequence:%s\n", sDest.c_str());   
	#endif

	
#if 0
    //输出公共子序列
    std::string s,sTmp;  
	i = len1;
	j = len2;
	k = c[i][j];
	max = k;
	
	printf("i=%d, j=%d, k=%d\n", i, j, k);
	
	while(i>0 & j>0)
	{
		if(vStr1[i-1] == vStr2[j-1] && c[i][j]==c[i-1][j-1]+1)
		{
			sTmp = vStr1[i-1];
            sTmp.append(s);
			s = sTmp;
			--j;
			--i;
		}else if(vStr1[i-1] != vStr2[j-1] && c[i-1][j] > c[i][j-1])//上>左
		{
			--i;
		}
		else
		{
			--j;
		}
	}
	
    printf("Longest-Common-subsequence:%s\n", s.c_str());   
#endif
	
//===================================================================	
    for(i = 0; i < len1+1; i++)         //释放动态申请的二维数组
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
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
				//printf("StrStr:i=%d, iLoop=%d\n", i, iLoop);
				//return src;
				return i;
			}
			
		}while(*bp++ == *sp++);
		src += 1;
		i++;
		
		//printf("StrStr:i=%d, iLoop=%d\n", i, iLoop);
		iLoop=0;
	}
	return -1;
}

//StrStr有bug不适合中文字符串
int CommonString(std::string longstr, std::vector<std::string> &vStr1, std::vector<std::string> &vStr2)
{
	if(vStr1.empty() || vStr2.empty())
	{
		return 0;
	}
	
	std::vector<std::string> vShortstr, vLongstr;
	
	if(vStr1.size() < vStr2.size())
	{
		vShortstr = vStr1;
		vLongstr = vStr2;
	}
	else
	{
		vShortstr = vStr2;
		vLongstr = vStr1;
	}
	
	
	int i=0, j=0;
	int shortlen=0, longlen=0;
	shortlen = vShortstr.size();
	longlen = vLongstr.size();

	printf("shortlen=%d, longlen=%d\n", shortlen, longlen);

	std::string substr;
	
	#if 0
	if(StrStr(longstr.c_str(), shortstr.c_str()) != -1)
	{
		shortstr.copy(substr, shortlen, 0);
		return substr;
	}
	#endif
	
	
	for(i=shortlen-1; i>0; i--)//控制长度
	{
		for(j=0; j<shortlen-i; j++)//控制右边结束的位置
		{
			substr = "";
			for(int t=j; t<shortlen-i; ++t)
			{
				substr.append(vShortstr[t]);
			}
			
			//printf("i=%d, j=%d, substr=%s\n", i, j, substr.c_str());
			int pos = -1;
			pos = StrStr(longstr.c_str(), substr.c_str());
			
			#if 1
			
			if(pos != -1)
			{
				printf("pipeidao: i=%d, j=%d, pos=%d, substr=%s\n", i, j, pos, substr.c_str());
				//return pos;
			}
			#endif
			
			#if 0
			if(pos>=0)
			{
				int iTmp = substr.length();
				
				if(pos==0 && longstr[iTmp]==',')//pos=0表示是最左边找到，那就判断接下来的分割符
				{
					printf("CommonString_1: pos=%d, longstr[%d]==',', substr[%s]\n\n", pos, iTmp, substr.c_str());
					return pos;
				}

				if(pos!=0 && pos<longstr.length()-1 && longstr[pos-1] == ',' && longstr[pos+iTmp-1] == ',')//pos!=0 表示中间
				{
					printf("CommonString_2: pos=%d, longstr[%d]==',', substr[%s]\n\n", pos, pos-1, substr.c_str());
					return pos;
				}
				if(pos == longstr.length()-1 && longstr[pos-iTmp] == ',')//最后
				{
					printf("CommonString_3: pos=%d, substr[%s]\n\n", pos, substr.c_str());
					return pos;
				}
				
				printf("pos=%d, substr=%s, iTmp=%d, longstr[pos-1]=%c, longstr[pos+iTmp-1]=%c\n\n", pos, substr.c_str(), iTmp, longstr[pos-1], longstr[pos+iTmp-1]);
			}
			#endif
		}
	
	}

	return 0;
}

void get_Next_val(std::vector<std::string> vSub, int next[])
{	
	if(next == NULL)
	{
		printf("next is null\n");
		return ;
	}
	
	int iLen = vSub.size();
	
	int i=0, j=-1;
	next[0] = -1;
	
	while(i<iLen-1)
	{
		//printf("i=%d, j=%d, sSub[%d]=%c, sSub[%d]=%c\t", i, j, i, sSub[i], j, sSub[j]);
		if(j==-1 || vSub[i] == vSub[j])
		{
			++i;
			++j;
			if(vSub[i] != vSub[j])
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
int KMP(std::vector<std::string> vStr1, std::vector<std::string> vStr2)
{
	int i=0, j=0;
	int iLen1 = vStr1.size();
	int iLen2 = vStr2.size();
		
	//next[]
	int* next = new int[iLen2];
	for(int i=0; i<iLen2; i++)
	{
		next[i] = 0;
	}

	get_Next_val(vStr2, next);

#if 0
	for(int i=0; i<iLen2; i++)
	{
		printf("next[%d]=%d\t", i, next[i]);
	}
	printf("\n");
#endif

	int iLoop=0;
	while(i<iLen1 && j<iLen2)
	{
		iLoop++;
		if(j==-1 || vStr1[i] == vStr2[j])
		{
			++i;
			++j;
		}
		else
		{
			j = next[j];
		}
	}
	//printf("----------KMP, iLoop=%d---------------\n", iLoop);
	delete []next;
	
	if(j>=iLen2)
	{
		return i-iLen2;
	}
	
	return -1;
}

int CommonString_KMP(std::vector<std::string> &vStr1, std::vector<std::string> &vStr2)
{
	printf("==================CommonString_KMP=====================\n");
	
	int i=0, j=0;
	int shortlen=0, longlen=0;
	
	longlen = vStr1.size();
	shortlen = vStr2.size();

	std::vector<std::string> vSubstr;
	
	for(i=shortlen; i>0; i--)//控制长度
	{
		for(j=0; j<i; j++)//控制右边结束的位置
		{
			vSubstr.clear();
			for(int t=j; t<i; ++t)
			{
				vSubstr.push_back(vStr2[t]);
			}
			
			//print_str(vSubstr);
			
			int pos = -1;
			pos = KMP(vStr1, vSubstr);
			
		#if 1
			//printf("i=%d, j=%d, pos=%d, substr=%s\n", i, j, pos, substr.c_str());
			if(pos != -1)
			{
				printf("i=%d, j=%d, pos=%d\n", i, j, pos);
				print_str(vSubstr);
				//return pos;
			}
		#endif
			
		#if 0
			if(pos>=0)
			{
				int iTmp = substr.length();
				
				if(pos==0 && longstr[iTmp]==',')//pos=0表示是最左边找到，那就判断接下来的分割符
				{
					printf("CommonString_1: pos=%d, longstr[%d]==',', substr[%s]\n\n", pos, iTmp, substr.c_str());
					return pos;
				}

				if(pos!=0 && pos<longstr.length()-1 && longstr[pos-1] == ',' && longstr[pos+iTmp-1] == ',')//pos!=0 表示中间
				{
					printf("CommonString_2: pos=%d, longstr[%d]==',', substr[%s]\n\n", pos, pos-1, substr.c_str());
					return pos;
				}
				if(pos == longstr.length()-1 && longstr[pos-iTmp] == ',')//最后
				{
					printf("CommonString_3: pos=%d, substr[%s]\n\n", pos, substr.c_str());
					return pos;
				}
				
				printf("pos=%d, substr=%s, iTmp=%d, longstr[pos-1]=%c, longstr[pos+iTmp-1]=%c\n\n", pos, substr.c_str(), iTmp, longstr[pos-1], longstr[pos+iTmp-1]);
			}
		#endif
		}
	
	}

	return 0;
}



int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("pls input str\n");
		return 0;
	}


	std::string str1, str2;
	str1= argv[1];
	str2 = argv[2];
	printf("str1[%s], str2[%s]\n", str1.c_str(), str2.c_str());


	std::vector<std::string> vStr1;
	std::vector<std::string> vStr2;
	
	RecognizeCharacter(str1, vStr1);
	
	for(int i=0; i<vStr1.size(); ++i)
	{
		printf("vStr1[%d]:%s\t", i, vStr1[i].c_str());
	}
	printf("\n");

	
	RecognizeCharacter(str2, vStr2);
	
	for(int i=0; i<vStr2.size(); ++i)
	{
		printf("vStr2[%d]:%s\t", i, vStr2[i].c_str());
	}
	printf("\n");

	
	//lcs算法的公共子串
	//longest_common_substring(vStr1, vStr2);
	//printf("========longest_common_substring===========\n\n");


	//lcs最大公共子序列
	longest_common_subsequence(vStr1, vStr2);
	printf("========longest_common_subsequence===========\n\n");
#if 0
	//字符串的匹配
	CommonString(str1, vStr1, vStr2);
	printf("========CommonString===========\n");

	//用kmp算法做字符串的匹配
	CommonString_KMP(vStr1, vStr2);
	printf("========CommonString_KMP===========\n");
#endif

	return 0;
}





#if 0
int longest_common_subsequence(std::vector<std::string> vStr1, std::vector<std::string> vStr2)  
{ 
	//动态规划算法
    int i,j,k,len1,len2,max,x,y;  

	len1 = vStr1.size();
	len2 = vStr2.size();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//第0列都初始化为0
	}
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//第0行都初始化为0   
	}


	//输出c数组情况
	int q=0, p=0;
	#if 0
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}

	printf("=======================================\n");
	#endif

	std::vector<int> vPos;
    max = -1;  
    for(i = 1; i < len1+1; i++)//str1
    {  
        for(j = 1; j < len2+1; j++)//str2 
        {  
        	//subsequence
        	if(vStr1[i-1]==vStr2[j-1])
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else if(c[i][j-1] > c[i-1][j])
            {
            	c[i][j]=c[i][j-1];
            }
			else
			{
				c[i][j]=c[i-1][j];
			}
        }
		
		if(vStr1[i-1]=="@")
		{
			vPos.push_back(i);	
		}
    } 

	
	//输出c数组情况
	//int q=0, p=0;
	printf("\n=====after, printf c_array msg:======\n");
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
			//printf("%d\t", c[q][p]);
		}
		printf("\n");
	}

	for(int x=0; x<vPos.size(); ++x)
	{
		int d = vPos[x];
		printf("@=%d\n", d);
		
		for(int y=0; y<len2+1; ++y)
		{
			c[d][y] = -1;
		}
	}
	
	printf("\n=====DumpData_1 after, vPos c_array msg:======\n");
	#if 1
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
			//printf("%d\t", c[q][p]);
		}
		printf("\n");
	}
	#endif
	
	//DumpData_1(vStr1, vStr2, c);
	
	#if 1
	//vPos一定是从小到大的
    //输出公共子序列，按分割符号输出
    
    std::string s,sTmp, sDest;  
	//i = len1;
	//j = len2;
	//k = c[i][j];
	
	//i= 8;
	//j=12;
	//k=2;

	int iPos = vPos.size();
	printf("iPos=%d\n", iPos);
	int i1=0, i2=0;
	j = len2;

	for(int x=0; x<iPos; x++)
	{
		i1=0, i2=0, k=0, max=0;
		
		if(x+1 == iPos || vPos[x]==len1)//vPos[iSize-1]一定是最后一行的
		{
			break;
		}

		i1 = vPos[x];
		i2 = vPos[x+1];
		j = len2;
		
		max = i2-(i1+1);//i1和i2之间行，每一行都取一个
		printf("x=%d, i1=%d, i2=%d, j=%d, max=%d, len1=%d\n", x, i1, i2, j, max, len1);

		printf("(i2-1)-(i1+1)+1)=%d, (c[i2-1][j]-c[i1+1][j]+1)=%d\n", (i2-1)-(i1+1)+1, c[i2-1][j]-c[i1+1][j]+1 );
		//判断是否为命中的黑词
		//每一行的最大一个数字肯定能在这一行的最后一列体现出来
		//i2-(i1+1)==c[i2-1][j]-c[i1+1][j]-1 满足这个公式才能保证i1到i2(不包含i1和i2行)之间行是命中的黑词
		if( ((i2-1)-(i1+1)+1) != (c[i2-1][j]-c[i1+1][j]+1) )//这样的公式写发或许更好理解一点
		{
			continue;
		}
		//end结束判断
		
		i=i2;//如果i2=len1+1,vStr[len1+1-1]=vStr1[len1]或coredump，最后一行添加一个@分割符
		
		if(i>len1) break;

		s = "";
		sTmp = "";
		
		while(i>0 && j>0 && max>0)
		{
			if(vStr1[i-1] == vStr2[j-1] && (c[i-1][j-1] == -1 || c[i][j]==c[i-1][j-1]+1))
			{	
				
				printf("i=%d, j=%d, vStr1[i-1]=%s, vStr2[j-1]=%s,c[i-1][j-1]=%d, c[i][j]=%d, \n", i, j, vStr1[i-1].c_str(), vStr2[j-1].c_str(), c[i-1][j-1], c[i][j]);
				sTmp = vStr1[i-1];
	            sTmp.append(s);
				s = sTmp;
				
				--j;
				--i;
				--max;
				
			}else if(vStr1[i-1] != vStr2[j-1] && c[i-1][j] > c[i][j-1])//上>左
			{
				--i;//往上
			}
			else
			{
				--j;//往左
			}
		}

		printf("max=%d, k=%d, s:%s\n\n", max, k, s.c_str());
		
		#if 0
		
		if(max==0)
		{
			sDest.append(s).append("@");
		}
		#endif
		sDest.append(s).append("@");
		
	}
	
    printf("Longest-Common-subsequence:%s\n", sDest.c_str());   
	#endif

	
#if 0
    //输出公共子序列
    std::string s,sTmp;  
	i = len1;
	j = len2;
	k = c[i][j];
	max = k;
	
	printf("i=%d, j=%d, k=%d\n", i, j, k);
	
	while(i>0 & j>0)
	{
		if(vStr1[i-1] == vStr2[j-1] && c[i][j]==c[i-1][j-1]+1)
		{
			sTmp = vStr1[i-1];
            sTmp.append(s);
			s = sTmp;
			--j;
			--i;
		}else if(vStr1[i-1] != vStr2[j-1] && c[i-1][j] > c[i][j-1])//上>左
		{
			--i;
		}
		else
		{
			--j;
		}
	}
	
    printf("Longest-Common-subsequence:%s\n", s.c_str());   
#endif
	
//===================================================================	
    for(i = 0; i < len1+1; i++)         //释放动态申请的二维数组
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
} 

int longest_common_subsequence(std::vector<std::string> vStr1, std::vector<std::string> vStr2)  
{ 
	//动态规划算法
    int i,j,k,len1,len2,max,x,y;  

	len1 = vStr1.size();
	len2 = vStr2.size();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//第0列都初始化为0
	}
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//第0行都初始化为0   
	}

	std::vector<int> vPos;
	vPos.push_back(0);
    max = -1;  
    for(i = 1; i < len1+1; i++)//str1
    {  
        for(j = 1; j < len2+1; j++)//str2 
        {  
        	//subsequence
        	if(vStr1[i-1]==vStr2[j-1])
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else if(c[i][j-1] > c[i-1][j])
            {
            	c[i][j]=c[i][j-1];
            }
			else
			{
				c[i][j]=c[i-1][j];
			}
        }
		
		if(vStr1[i-1]=="@")
		{
			vPos.push_back(i);	
		}
    } 

	
	//输出c数组情况
	int q=0, p=0;
	
	for(int x=0; x<vPos.size(); ++x)
	{
		int d = vPos[x];
		printf("@=%d\n", d);
		//c[d][0] = -1;
	}
	
	
	printf("\n=====DumpData_1 after, vPos c_array msg:======\n");
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}
		
	#if 1
	//vPos一定是从小到大的
    //输出公共子序列，按分割符号输出
    
    std::string s,sTmp, sDest;  
	int iPos = vPos.size();
	printf("iPos=%d\n", iPos);
	
	int i1=0, i2=0;
	j = len2;

	for(int x=0; x<iPos; x++)
	{
		i1=0, i2=0, k=0, max=0;
		
		if(x+1 == iPos || vPos[x]==len1)//vPos[iSize-1]一定是最后一行的
		{
			break;
		}

		i1 = vPos[x];
		i2 = vPos[x+1];
		j = len2;
		
		max = i2-(i1+1);//i1和i2之间行，每一行都取一个
		printf("x=%d, i1=%d, i2=%d, j=%d, max=%d, len1=%d\n", x, i1, i2, j, max, len1);

#if 0
		//如下判断不对 abcd@abc@3 abc 这样的例子就匹配不来 ((i2-1)-(i1+1)+1) != (c[i2-1][j]-c[i1+1][j]+1) 
		printf("(i2-1)-(i1+1)+1)=%d, (c[i2-1][j]-c[i1+1][j]+1)=%d\n", (i2-1)-(i1+1)+1, c[i2-1][j]-c[i1+1][j]+1 );
		//判断是否为命中的黑词
		//每一行的最大一个数字肯定能在这一行的最后一列体现出来
		//i2-(i1+1)==c[i2-1][j]-c[i1+1][j]-1 满足这个公式才能保证i1到i2(不包含i1和i2行)之间行是命中的黑词
		if( ((i2-1)-(i1+1)+1) != (c[i2-1][j]-c[i1+1][j]+1) )//这样的公式写发或许更好理解一点
		{
			continue;
		}
		//end结束判断
#endif
//往左边移到是对的，但是，往上移动就错了

		i=i2-1;//取分隔符的上一行
		
		if(i>len1) break;

		s = "";
		sTmp = "";
		
		while(i>i1 && j>0 && max>0)
		{
			
			printf("i=%d, j=%d, vStr1[i-1]=%s, vStr2[j-1]=%s \n", i, j, vStr1[i-1].c_str(), vStr2[j-1].c_str());
			if(vStr1[i-1] == vStr2[j-1] && c[i][j]==c[i-1][j-1]+1 )
			{	
				sTmp = vStr1[i-1];
	            sTmp.append(s);
				s = sTmp;
				
				--j;
				--i;
				--max;
				
			}else if(vStr1[i-1] != vStr2[j-1] && c[i-1][j] > c[i][j-1])//上>左
			{
				//--i;//往上
				break;
			}
			else
			{
				--j;//往左
			}
		}
		printf("max=%d, k=%d, s:%s\n\n", max, k, s.c_str());
		
		#if 1
		
		if(max==0)
		{
			sDest.append(s).append("@");
		}
		#endif
		
	}
	
    printf("Longest-Common-subsequence:%s\n", sDest.c_str());   
	#endif

	
#if 0
    //输出公共子序列
    std::string s,sTmp;  
	i = len1;
	j = len2;
	k = c[i][j];
	max = k;
	
	printf("i=%d, j=%d, k=%d\n", i, j, k);
	
	while(i>0 & j>0)
	{
		if(vStr1[i-1] == vStr2[j-1] && c[i][j]==c[i-1][j-1]+1)
		{
			sTmp = vStr1[i-1];
            sTmp.append(s);
			s = sTmp;
			--j;
			--i;
		}else if(vStr1[i-1] != vStr2[j-1] && c[i-1][j] > c[i][j-1])//上>左
		{
			--i;
		}
		else
		{
			--j;
		}
	}
	
    printf("Longest-Common-subsequence:%s\n", s.c_str());   
#endif
	
//===================================================================	
    for(i = 0; i < len1+1; i++)         //释放动态申请的二维数组
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
} 

#endif
