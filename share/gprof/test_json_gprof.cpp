#include <string>
#include "jserialize.h"
#include <stdint.h>
#include <vector>


class CTest
{
public:
	CTest():dwTest(0){}
	~CTest(){}

	template<class AR> AR & serialize(AR &ar)
	{
		_SERIALIZE_(ar, sTest);
		_SERIALIZE_(ar, dwTest);
		
		return ar; 
	}

public:
	std::string sTest;
	uint32_t dwTest;
};


int test(int i)
{
	
	CTest oTest;
	oTest.sTest = "luyanhong";
	oTest.dwTest = 19;
	std::string sTest;
	
	for(int i=0; i<100000; i++)
	{
		sTest = ad::serialize::to_json(oTest, i);
		//printf("sTest[%s]\n", sTest.c_str());
	}
	return 0;
}

int main(int argc, char **argv)
{
	
	CTest oTest;
	oTest.sTest = "luyanhong";
	oTest.dwTest = 19;
	std::string sTest;
	
	std::vector<CTest> o;
	o.push_back(oTest);

	for(int i=0; i<10000; i++)
	{
		sTest = ad::serialize::to_json(o, 0);
		//printf("sTest[%s]\n", sTest.c_str());
	}

	#if 0
	std::map<std::string, std::string> m;
	std::string s1 = ad::serialize::to_json(m);
	printf("m:%s\n", s1.c_str());
	#endif
	return 0;
}


