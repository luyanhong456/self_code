#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
using namespace std;
int bag_0_1()
{
	int n=5;//物品个数
	int cap = 10;//最大承重
	int v[5] = {5,18,23,22,8};//每个物品的价值
	int w[5] = {4,3,7,6,1};//每个物品的重量

	int i=0, j=0;

	//dp[n+1][cap+1]
	int **dp = new int*[n+1];  
    for(i = 0; i < n+1; i++)
	{
		dp[i] = new int[cap+1];
	}

    for(i = 0; i < n+1; i++)
	{
		for(j = 0; j < cap+1; j++)
		{
			dp[i][j]=0;//第0行都初始化为0   
		}
	}
//==============================================================================    

	for (i=1; i <=n; i++)/*枚举物品*/ 
	{ 
        for (j=1; j<cap+1; j++)/*枚举重量*/  
        {
            //判断枚举的重量和当前选择的物品重量的关系
            //如果枚举的和总量大于等于选择物品，则需要判断是否选择当前物品 
            if (j-w[i-1]>=0)
            {
                dp[i][j] = max(dp[i-1][j], dp[i-1][j-w[i-1]]+v[i-1]);
            }  
            else
            {
                /*如果枚举的重量还没有当前选择物品的重量大，那就只能是不取当前物品*/  
                dp[i][j] = dp[i-1][j];
            } 
        }  
    }  
//=============================================================
    printf("dp[%d][%d]=%d\n", n, cap, dp[n][cap]);

    for(j=0; j<cap+1; ++j)
    {
    	if(j==0) printf("\t     ");
    	printf("%d\t", j);
    }
    printf("\n");

    for(i=0; i<n+1; ++i)
    {
    	if(i==0) printf("\t   %d ", i);
    	if(i>0)
    	{
    		printf("w[%d]=%d(%d) %d ", i-1, w[i-1], v[i-1], i);
    	}
    	for(j=0; j<cap+1; ++j)
    	{
    		//printf("dp[%d][%d]=%d\t", i, j, dp[i][j]);
    		//if(j==0) printf("\t");
    		printf("%d\t", dp[i][j]);
    	}
    	printf("\n");
    }
	
    printf("\n");
//=============================================================
	//逆序倒推求得命中的数据
    i=n;
    j=cap;
    while(i>=1 && j>=1 /*&& j-w[i-1]>=0*/)
    {
		if(j-w[i-1]>=0 && dp[i][j] == dp[i-1][j-w[i-1]]+v[i-1] )
		{
			printf("hit info: w[%d]=%d, v[%d]=%d\n", i-1, w[i-1], i-1, v[i-1]);
			j=j-w[i-1];
			i=i-1;
		}
		else if(dp[i][j]==dp[i-1][j] || j-w[i-1]<0)
		{
			i--;
		}
		else if(dp[i][j] > dp[i-1][j])
		{
			j--;
		}
    	
    }

    for(i = 0; i < n+1; i++)         //释放动态申请的二维数组
    {
        delete[] dp[i];
	}
	
    delete[] dp; 

    return -1; 
}

int main(int argc, char **argv)
{
	bag_0_1();
	return 0;
}
