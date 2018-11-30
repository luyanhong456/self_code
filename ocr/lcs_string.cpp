/**  
�ҳ������ַ���������������Ӵ��ĳ���  
**/   
#include <stdio.h> 
#include <string.h>  
#include <stdlib.h>  
#include <string>

int longest_common_substring(const char *str1, const char *str2)  
{  
    int i,j,k,len1,len2,max,x,y;  
    len1 = strlen(str1);  
    len2 = strlen(str2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//��0�ж���ʼ��Ϊ0
	}

        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//��0�ж���ʼ��Ϊ0   
	}

    max = -1;  
    for(i = 1 ; i < len1+1 ; i++)  
    {  
        for(j = 1; j < len2+1; j++)  
        {  
            if(str1[i-1]==str2[j-1])     //ֻ��Ҫ�����Ϸ���c[i-1][j-1]�ȽϾͿ�����
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else                       //��������ʱ��Ҫ����ߵ�c[i][j-1]���ϱߵ�c[i-1][j]ֵ�Ƚϣ����ﲻ��Ҫ 
            {
            	c[i][j]=0;
            }
                
            if(c[i][j] > max)  
            {  
                max = c[i][j];  
                x = i;  
                y = j;  
            }  
        }  
    }  
  
    //��������Ӵ�  
    char s[1000];  
    k=max;  
    i=x-1, j=y-1;  
    s[k--]='\0';
	
    while(i>=0 && j>=0)  
    {  
        if(str1[i]==str2[j])  
        {  
            s[k--]=str1[i];  
            i--;  
            j--;  
        }  
        else       //ֻҪ��һ������ȣ���˵����ȵĹ����ַ����ˣ���������  
            break;  
    }
	
    
    printf("Longest-Common-SuString:%s\n", s);   
    
	
    for(i = 0; i < len1+1; i++)         //�ͷŶ�̬����Ķ�ά����  
        delete[] c[i];  
    delete[] c;  
	
    return max;  
}  


int longest_common_substring(const std::string str1, const std::string str2)  
{ 
	//��̬�滮�㷨
	
    int i,j,k,len1,len2,max,x,y;  

	len1 = str1.length();
	len2 = str2.length();
	
	printf("str1:%s, len1=%d, str2:%s, len2=%d\n", str1.c_str(), len1, str2.c_str(), len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//��0�ж���ʼ��Ϊ0
		//printf("c[%d][0]=%d\t", i, c[i][0]);
	}
	//printf("\n=======================================\n");
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//��0�ж���ʼ��Ϊ0   
		//printf("c[0][%d]=%d\t", j, c[0][j]);
	}
	//printf("\n=======================================\n");


	//���c�������
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
            if(str1[i-1]==str2[j-1])
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else
            {
            	c[i][j]=0;
            }
			#endif

			if(str1[i-1] == str2[j-1])
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
			else if(str1[i-1] == ',')
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
               
            if(c[i][j] >= max)  
            {  
                max = c[i][j];  
                x = i;  
                y = j;  
            }  
        }  
    } 
	printf("x=%d, y=%d, max=%d\n", x, y, max);
	
	//���c�������
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
		
	
    //��������Ӵ�  
    char s[1000];  
    k = max;  
    i=x-1, j=y-1;  
    s[k--]='\0';

	printf("i=%d, j=%d, max=%d\n", i, j, max);
    while(i>=0 && j>=0)  
    {  
        if(str1[i] == str2[j])  
        {  
            s[k--] = str1[i];  
            i--;  
            j--;  
        }  
        else       //ֻҪ��һ������ȣ���˵����ȵĹ����ַ����ˣ���������  
        {
            break;
		}
    }
	
    printf("Longest-Common-SuString:%s\n", s);   
	
    for(i = 0; i < len1+1; i++)         //�ͷŶ�̬����Ķ�ά����
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
} 


int longest_common_substring_all(const std::string str1, const std::string str2)  
{ 
	//��̬�滮�㷨
	
    int i,j,k,len1,len2,max,x,y;  

	len1 = str1.length();
	len2 = str2.length();
	
	printf("str1:%s, len1=%d, str2:%s, len2=%d\n", str1.c_str(), len1, str2.c_str(), len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//��0�ж���ʼ��Ϊ0
		//printf("c[%d][0]=%d\t", i, c[i][0]);
	}
	//printf("\n=======================================\n");
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//��0�ж���ʼ��Ϊ0   
		//printf("c[0][%d]=%d\t", j, c[0][j]);
	}
	//printf("\n=======================================\n");


	//���c�������
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
            if(str1[i-1]==str2[j-1])
            {
            	c[i][j]=c[i-1][j-1]+1;
            }   
            else
            {
            	c[i][j]=0;
            }
			#endif

			if(str1[i-1] == str2[j-1])
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
			else if(str1[i-1] == ',')
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
        }  
    } 
	//����ѭ��������len1*len2,ʱ�临�Ӷ�ΪO(len1*len2)
	//=========================================

	
	//���c�������
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
	//end ��̬�滮
	//=========================================

	//begin ��ȡ��̬�滮��������ǰ����-1���±�(i,j)��ֵ
	for(i=0; i<len1+1; i++)
	{
		if(c[i][0] == -1)//������0��,(i,0)==-1
		{
						
		}
	}



	//end 
    for(i=0; i < len1+1; i++)         //�ͷŶ�̬����Ķ�ά����
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
} 

//�󹫹�������
int longest_common_subsequence(const std::string str1, const std::string str2)  
{ 
	//��̬�滮�㷨
	
    int i,j,k,len1,len2,max,x,y;  

	len1 = str1.length();
	len2 = str2.length();
	
	printf("len1=%d, len2=%d\n", len1, len2);
	
    int **c = new int*[len1+1];  
    for(i = 0; i < len1+1; i++)
	{
		c[i] = new int[len2+1];
	}
		  
    for(i = 0; i < len1+1; i++)
	{
		c[i][0]=0;//��0�ж���ʼ��Ϊ0
		//printf("c[%d][0]=%d\t", i, c[i][0]);
	}
	printf("\n=======================================\n");
        
    for(j = 0; j < len2+1; j++)
	{
		c[0][j]=0;//��0�ж���ʼ��Ϊ0   
		//printf("c[0][%d]=%d\t", j, c[0][j]);
	}
	//printf("\n=======================================\n");

	#if 0
	//���c�������
	
	int q=0, p=0;
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
            if(str1[i-1]==str2[j-1])
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
			
			#if 1
            if(c[i][j] > max)  
            {  
                max = c[i][j];  
                x = i;  
                y = j;  
            }
			#endif
                
        }  
    } 
	
	//���c�������
	int q=0, p=0;
	printf("=====after, printf c_array msg:======\n");
	for(q=0; q<len1+1; ++q)
	{
		for(p=0; p<len2+1; ++p)
		{
			printf("c[%d][%d]=%d\t", q, p, c[q][p]);
		}
		printf("\n");
	}
		
	#if 1
    //��������Ӵ�
    char s[1000];  
	//i = len1;
	//j = len2;
	
	i=x;
	j=y;

	k = c[i][j];
	max = k;
	printf("i=%d, j=%d, k=%d\n", i, j, k);
	
	s[k--]='\0';

	while(i>0 & j>0)
	{
		if(str1[i-1] == str2[j-1] && c[i][j]==c[i-1][j-1]+1)
		{
			s[k--] = str1[i-1];
			--j;
			--i;
		}else if(str1[i-1] != str2[j-1] && c[i-1][j] > c[i][j-1])//��>��
		{
			--i;
		}
		else
		{
			--j;
		}
	}
	
    printf("Longest-Common-subsequence:%s\n", s);   
	#endif
	
    for(i = 0; i < len1+1; i++)         //�ͷŶ�̬����Ķ�ά����
    {
        delete[] c[i];
	}
	
    delete[] c;  
	
    return max;  
} 



int main(int argc, char **argv)  
{  
	#if 0
    char str1[1000],str2[1000]; 
	
    printf("�������һ���ַ�����");  
    gets(str1);  
    printf("������ڶ����ַ�����");  
    gets(str2);  
	#endif
	
	if(argc <3)
	{
		printf("pls input str1 & str2\n");
		return 0;
	}

	
	std::string sStr1, sStr2;

	sStr1 = argv[1];
	sStr2 = argv[2];

	printf("str1[%s], len1=%d, str2[%s],len2=%d\n", sStr1.c_str(), sStr1.length(), sStr2.c_str(), sStr2.length());

#if 1
	int len = 0;
		
    //int len = longest_common_substring(sStr1.c_str(), sStr2.c_str());  
	
    //printf("lcs length=%d\n",len);


	//len = longest_common_substring(sStr1, sStr2);  
	
    //printf("lcs length=%d\n",len);
	
	printf("==================longest_common_subsequence=====================\n");

	len = longest_common_subsequence(sStr1, sStr2);
	printf("lcs length=%d\n",len);
#endif

    return 0;  
} 

