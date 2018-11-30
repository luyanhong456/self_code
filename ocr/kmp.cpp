#include <string>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>


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
	//printf("RecognizeCharacter: sStr[%s], length=%d\n", sStr.c_str(), length);
	
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

//==========================================================================================================================================

int longest_common_substring(std::vector<std::string> vMainStr, std::vector<std::string> vSubStr)  
{ 
	//动态规划算法
	
    int i,j,k,len1,len2,max,x,y;  

	len1 = vMainStr.size();
	len2 = vSubStr.size();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **dp = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		dp[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		dp[i][0]=0;//第0列都初始化为0
	}
        
    for(j = 0; j < len2+1; j++)
	{
		dp[0][j]=0;//第0行都初始化为0   
	}


    max = -1; 
    //求dp值
    for(i = 1; i < len1+1; i++)//str1
    {  
        for(j = 1; j < len2+1; j++)//str2 
        {  
            if(vMainStr[i-1]==vSubStr[j-1])
            {
            	dp[i][j]=dp[i-1][j-1]+1;
            }   
            else
            {
            	dp[i][j]=0;
            }
               
            if(dp[i][j] >= max)  
            {  
                max = dp[i][j];  
                x = i;  
                y = j;  
            }  
        }  
    } 
	printf("x=%d, y=%d, max=%d\n", x, y, max);

	//====================================================================================================
	//输出dp数组情况
	printf("\n\n");
	int q=0, p=0;
	for(q=0; q<len2; ++q)
	{
		if(q==0) printf(" \t");

		printf("%s\t", vSubStr[q].c_str());
	}
	printf("\n");

	for(q=0; q<len1+1; ++q)
	{
		if(q==0) printf("   ");
		if(q>0) printf("%s  ",vMainStr[q-1].c_str());
		for(p=0; p<len2+1; ++p)
		{
			//printf("dp[%d][%d]=%d\t", q, p, dp[q][p]);
			printf("%d\t", dp[q][p]);
		}
		printf("\n");
	}
	printf("\n\n");
//====================================================================================================		

		
	
    //输出公共子串  
	std::string s, sTmp;
    k = max;  
    i=x-1, j=y-1;  

	printf("i=%d, j=%d, max=%d\n", i, j, max);
    while(i>=0 && j>=0)  
    {  
        if(vMainStr[i] == vSubStr[j]) //从后往上查的
        {  
            sTmp = vMainStr[i];
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
        delete[] dp[i];
	}
	
    delete[] dp;  
	
    return max;  
} 



//============================================================================================================================================

int longest_common_subsequence(std::vector<std::string> vMainStr, std::vector<std::string> vSubStr)  
{ 
	//动态规划算法
    int i,j,k,len1,len2,max,x,y;  

	len1 = vMainStr.size();
	len2 = vSubStr.size();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **dp = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		dp[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		dp[i][0]=0;//第0列都初始化为0
	}
        
    for(j = 0; j < len2+1; j++)
	{
		dp[0][j]=0;//第0行都初始化为0   
	}


    max = -1; 
    //求dp值
    for(i = 1; i < len1+1; i++)//str1
    {	
        for(j = 1; j < len2+1; j++)//str2 
        {  
        	//subsequence
        	if(vMainStr[i-1]==vSubStr[j-1])
            {
            	dp[i][j]=dp[i-1][j-1]+1;
            }   
            else if(dp[i][j-1] > dp[i-1][j])
            {
            	dp[i][j]=dp[i][j-1];
            }
			else
			{
				dp[i][j]=dp[i-1][j];
			}
        }
    } 

//====================================================================================================
	//输出dp数组情况
	printf("\n\n");
	int q=0, p=0;
	for(q=0; q<len2; ++q)
	{
		if(q==0) printf(" \t");

		printf("%s\t", vSubStr[q].c_str());
	}
	printf("\n");

	for(q=0; q<len1+1; ++q)
	{
		if(q==0) printf("   ");
		if(q>0) printf("%s  ",vMainStr[q-1].c_str());
		for(p=0; p<len2+1; ++p)
		{
			//printf("dp[%d][%d]=%d\t", q, p, dp[q][p]);
			printf("%d\t", dp[q][p]);
		}
		printf("\n");
	}
	printf("\n\n");
//====================================================================================================		

    //输出公共子序列
    std::string s,sTmp;  
	i = len1;
	j = len2;
	k = dp[i][j];
	max = k;
	printf("i=%d, j=%d, k=%d\n", i, j, k);
	
	while(i>0 & j>0)
	{
		printf("i=%d, j=%d\n", i, j);
		if(vMainStr[i-1] == vSubStr[j-1] && dp[i][j]==dp[i-1][j-1]+1)
		{
			sTmp = vMainStr[i-1];
            sTmp.append(s);
			s = sTmp;
			--j;
			--i;
			
		}else if(vMainStr[i-1] != vSubStr[j-1] && dp[i-1][j] > dp[i][j-1])//上>左
		{
			--i;
		}
		else
		{
			--j;
		}
	}
	
    printf("Longest-Common-subsequence:%s\n", s.c_str());   

    for(i = 0; i < len1+1; i++)         //释放动态申请的二维数组
    {
        delete[] dp[i];
	}
	
    delete[] dp;  
	
    return max;  
}


//===========================================================================================================================================


void get_Next_val(std::vector<std::string> vSub, int next[])
{	
	if(next == NULL)
	{
		printf("next is null\n");
		return ;
	}
	
	int iLen = vSub.size();
	int i=0, j=-1;//vSub下标从0开始
	next[0] = -1;
	
	while(i < iLen-1)
	{
		if(j==-1 || strncmp(vSub[i].c_str(), vSub[j].c_str(), vSub[i].length())==0 )
		{
			++i;
			++j;
			next[i]=j;
		}
		else
		{
			j = next[j];
		}
	}
	return;
}


void get_Next_val_1(std::vector<std::string> vSub, int next[])
{	
	if(next == NULL)
	{
		printf("next is null\n");
		return ;
	}
	
	int iLen = vSub.size();
	int i=0, j=-1;//vSub下标从0开始
	next[0] = -1;
	
	while(i < iLen-1)
	{
		if(j==-1 || strncmp(vSub[i].c_str(), vSub[j].c_str(), vSub[i].length())==0 )
		{
			++i;
			++j;
			//next[i]=j;
			if( strncmp(vSub[i].c_str(), vSub[j].c_str(), vSub[i].length())!=0 )
			{
				next[i] = j;
			}
			else
			{
				next[i] = next[j];
			}
		}
		else
		{
			j = next[j];
		}
	}
	return;
}

//返回匹配成功的位置,-1表示没有匹配成功,可以再加一个入参，指定从哪一位开始匹配
int KMP(std::vector<std::string> vMainStr, std::vector<std::string> vSubStr)
{
	int i=0, j=0;
	int iLen1 = vMainStr.size();
	int iLen2 = vSubStr.size();

	//next[]
	int* next = new int[iLen2];
	for(int i=0; i<iLen2; i++)
	{
		next[i] = 0;
	}

	get_Next_val(vSubStr, next);
#if 1
	for(int i=0; i<iLen2; ++i)
	{
		printf("vStr[%d]=%s\t", i, vSubStr[i].c_str());
	}
	printf("\n");
	
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
		if(j==-1 || vMainStr[i] == vSubStr[j])
		{
			++i;
			++j;
		}
		else
		{
			j = next[j];
		}
	}
	//删除next
	delete []next;	
	if(j>=iLen2)//j到模式串的最后一个下标，最后一个下标+1等于len
	{
		return i-iLen2;//返回匹配到的主串位置
	}
	return -1;//-1表示没有找到
}




//===========================================================================================================================================

int main(int argc, char **argv)
{
	if(argc < 4)
	{
		printf("pls input cmd:kmp or dynamic or dynamic_1, mainstr, substr\n");
		return 0;
	}


	std::string cmd, str1, str2;
	cmd = argv[1];
	str1= argv[2];
	str2 = argv[3];
	printf("str1[%s], str2[%s]\n", str1.c_str(), str2.c_str());


	std::vector<std::string> vStr1;
	std::vector<std::string> vStr2;
	
	RecognizeCharacter(str1, vStr1);	
	RecognizeCharacter(str2, vStr2);

	#if 0
	for(int i=0; i<vStr1.size(); ++i)
	{
		printf("vStr1[%d]:%s\t", i, vStr1[i].c_str());
	}
	printf("\n");

	for(int i=0; i<vStr2.size(); ++i)
	{
		printf("vStr2[%d]:%s\t", i, vStr2[i].c_str());
	}
	printf("\n");
	#endif
	
	if(cmd == "kmp")
	{
		int iPos = 0;
		iPos = KMP(vStr1, vStr2);
		printf("iPos=%d\n", iPos);
		
		int iFound = 0;
		iFound = str1.find(str2);
		printf("iFound=%d\n", iFound);
	}

	if(cmd == "dynamic")
	{
		longest_common_substring(vStr1, vStr2);
	}


	if(cmd == "dynamic_1")
	{
		longest_common_subsequence(vStr1, vStr2);
	}

	if(cmd == "kmp_1")
	{
		int iPos = 0;
		iPos = KMP(vStr1, vStr2);
		printf("iPos=%d\n", iPos);
	}
	
	return 0;
}
