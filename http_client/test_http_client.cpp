#include <string>
#include "tlog.h"
#include "http_client.h"
#include "ocpa_bj_struct.h"
#include <ctime>
#include <sys/time.h>

using namespace ad::ocpa;

uint64_t GetCurTime()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday ( &tv, &tz );
    return ( ( ( uint64_t ) tv.tv_sec ) * 1000000 ) + tv.tv_usec;
}


//调用北京
int main(int argc, char **argv)
{
	uint64_t ddwBeginTime = GetCurTime();

	TLOG->Init(argv[0]);

	TLOG->SetConsoleShow(1);
	TLOG_MSG("begin");
	
	CAdRecommendReq oReq;
	oReq.skucnt = 5;
	oReq.userid = "3A96AA61EDC6E8ADDCAC5256720B9810";
	oReq.usertype = 1;
	oReq.token = "afd5428337d6cbfcf42557bbaa492827";
	
	std::string sReq = ad::serialize::to_json(oReq);
	
	CHttpClient oHttpClient("10.191.87.204", 8080);
	
	//oHttpClient.SetHttpFlag(true);
	oHttpClient.SetReqUri("http://bdsp.jd.local/operad/ad/recommend");
	oHttpClient.SetReqHeader("Accept-Charset", "utf-8");
	oHttpClient.SetReqMethod(HTTPMETHOD_POST);//post
	oHttpClient.SetTimeOutMs(5);
	
	oHttpClient.SetReqBody(sReq);
	int iRet = oHttpClient.Invoke();
	if(iRet != 0)
	{
		TLOG_ERR("iRet[%d]", iRet);
	}
	else
	{
		TLOG_MSG("iRet[%d]", iRet);
	}
	
	
	TLOG_MSG("http_code[%u]", oHttpClient.GetRespStatus());

	TLOG_MSG("reqbody[%s]", oHttpClient.GetReqBody().c_str());
	
	TLOG_MSG("RespRaw[%s]", oHttpClient.GetRespRaw());
	
	TLOG_MSG("resp[%s]", oHttpClient.GetRespData().c_str());
	
	uint64_t ddwEndTime = GetCurTime();
	
	TLOG_MSG("costtime[%u]us", ddwEndTime-ddwBeginTime);
	
	return 0;
}
