#include <stdint.h>
#include <stdio.h>



int main(int argc, char **argv)
{

	int     str[10];  
    int     i, j;  
    //��ʼ������Ϊ10 9 8 7 6 5 4 3 2 1  
    for (i = 0; i < 10; i++)  
    {  
        str[i] = 10 - i;  
    }  
	  //��ʮ�������  
    for (i = 0; i < 10; i++)  
        printf("%d\t", str[i]);  
	
	printf("\n");
	
    //���򣬴�a[0]��ʼ�ţ���С����  
    for (i = 0; i < 10; i++)  
    {  
        for (j = i + 1; j < 10; j++)  
        {  
            if (str[i] > str[j])  
            {  
            	int tmp;
				tmp = str[i];
				str[i] = str[j];
				str[j] = tmp;
							
            }  
        }  
    }  
    //��ʮ�������  
    for (i = 0; i < 10; i++)  
        printf("%d\t", str[i]);  
	
	printf("\n");
    return    0; 
}
 

