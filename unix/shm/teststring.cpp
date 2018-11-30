#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;
int main(int argc, char **argv)
{
	string pStr;
	string pTest = "test";
	char test[]= "test";
	cout<<"test"<<endl;
	memcpy(&pStr, test, 5);
	cout<<"test1"<<endl;
	//cout<<"pstr=" << pStr <<endl;
	if( pStr.empty() )
	{
		cout<<"test2-n"<<endl;
		//cout<<"pStr is null" <<endl;
	}
	else
	{
		cout<<"test2"<<endl;
		//cout<<"pstr=" << pStr <<endl;
	}
	
	return 0;
}
