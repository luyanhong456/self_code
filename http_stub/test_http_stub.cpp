#include <string>
#include "tlog.h"
#include "http_stub.h"
#include "ocpa_bj_struct.h"
#include <ctime>
#include <sys/time.h>

//sz show data
//#include "dtcy_show.pb.h"
#include "ocpa_show_api_struct.h"

#include "test_struct.h"



using namespace ad::dtcy;
using namespace ad::ocpa;
using namespace boss;

uint64_t GetCurTime()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday ( &tv, &tz );
    return ( ( ( uint64_t ) tv.tv_sec ) * 1000000 ) + tv.tv_usec;
}




#if 0
//调用测试fapp程序 test.lyh
//后台会显示这样的错误:[2017-03-08 17:01:31.73.349][pid:32597][fapp.cpp:253][fapp_loop] ERR: not found:[]
int main(int argc, char **argv)
{
	uint64_t ddwBeginTime = GetCurTime();
	TLOG->Init(argv[0]);
	TLOG->SetConsoleShow(1);

	CReq oReq;
	oReq.req = "test";
	oReq.code = 88;

	CHttpStub oStub("lyh.test", "");
	oStub.SetReqUri("http://mrjx.jd.com/fapp/test/lyh/process");
	//oStub.SetAddr("192.168.145.72", 8083);
	
	oStub.SetReqHeader("Accept-Charset", "utf-8");
	oStub.SetReqMethod(HTTPMETHOD_POST);//post
	oStub.SetHttpVersion("HTTP/1.0");
	oStub.SetTimeOutMs(3000);

	oStub.SetProtocol(PRO_TCP);
	CProtocolWaiter oWaiter;
	int iRet = oStub.AsyncInvokeJson(oReq, &oWaiter);
	if(iRet != 0)
	{
		TLOG_ERR("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}
	else
	{
		TLOG_MSG("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}

	oWaiter.Wait(3000);
	TLOG_MSG("errmsg[%s]", oStub.GetErrMsg());
	
	CReq oResp;
	oStub.GetResult(oResp);
	uint64_t ddwEndTime = GetCurTime();


	
	TLOG_MSG("oStub.GetRespData[%s]", oStub.GetRespData().c_str());
	TLOG_MSG("errmsg[%s]", oStub.GetErrMsg());
	
	if(oStub.GetRespData().empty())
	{
		TLOG_ERR("fail, errmsg[%s], costtime[%u]us", oStub.GetErrMsg(), ddwEndTime-ddwBeginTime);
	}
	else
	{
		TLOG_MSG("errmsg[%s], costtime[%u]us", oStub.GetErrMsg(), ddwEndTime-ddwBeginTime);
	}
	
	return 0;
}

#endif

#if 0
//pb方式调用深圳
int main(int argc, char **argv)
{
	
	uint64_t ddwBeginTime = GetCurTime();
	
	TLOG->Init(argv[0]);

	TLOG->SetConsoleShow(1);
	
#if 0
	CDtcyShowReq oReq;
	oReq.set_token("e0c6b7982602200f927b8a02ba441c8c");
	oReq.set_sid(10001);
	oReq.set_userid("3A96AA61EDC6E8ADDCAC5256720B9810");
	oReq.set_usertype(1);
	oReq.set_tagid(1);
	oReq.set_specid(27);
	oReq.set_width(220);
	oReq.set_height(120);

	oReq.set_skucnt(5);
	oReq.set_clientip(3073886824);
	oReq.set_domain("qzone.qq.com");
	oReq.set_bundlename("");
	oReq.set_rtime(1467170576);
#endif

	COcpaShowReq oSzReq;
	oSzReq.token = "e0c6b7982602200f927b8a02ba441c8c";
	oSzReq.sid = 10001;
	oSzReq.userid = "3A96AA61EDC6E8ADDCAC5256720B9810";
	oSzReq.usertype = 1;
	oSzReq.tagid = 1;
	oSzReq.specid = 27;
	oSzReq.width = 220;
	oSzReq.height = 120;
	oSzReq.skucnt = 5;
	oSzReq.clientip = 3073886824;
	oSzReq.domain = "qzone.qq.com";
	oSzReq.bundlename = "";
	oSzReq.rtime = 1467170576;

	//CHttpStub oStub("dtcy.show", "dtcy.show.set");
	//http://mrjx.jd.com/fcgi-bin/ocpa_show?userid=kajdfkjaf&usertype=1&skucnt=5
	CHttpStub oStub("lyh.test", "");
	oStub.SetReqUri("http://mrjx.jd.com/fcgi-bin/ocpa_show");
	//oStub.SetAddr("192.168.145.72", 8083);
	oStub.SetReqHeader("Accept-Charset", "utf-8");
	oStub.SetReqMethod(HTTPMETHOD_POST);//post
	oStub.SetHttpVersion("HTTP/1.0");
	oStub.SetTimeOutMs(3000);

	oStub.SetProtocol(PRO_TCP);

	CProtocolWaiter oWaiter;
	int iRet = oStub.AsyncInvokeJson(oSzReq, &oWaiter);
	if(iRet != 0)
	{
		TLOG_ERR("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}
	else
	{
		TLOG_MSG("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}

	oWaiter.Wait(3000);
	TLOG_MSG("errmsg[%s]", oStub.GetErrMsg());
	
	COcpaShowResp oResp;
	oStub.GetResult(oResp);
	uint64_t ddwEndTime = GetCurTime();


	
	TLOG_MSG("oStub.GetRespData[%s]", oStub.GetRespData().c_str());
	TLOG_MSG("errmsg[%s]", oStub.GetErrMsg());
	
	if(oStub.GetRespData().empty())
	{
		TLOG_ERR("fail, errmsg[%s], costtime[%u]us", oStub.GetErrMsg(), ddwEndTime-ddwBeginTime);
	}
	else
	{
		TLOG_MSG("errmsg[%s], costtime[%u]us", oStub.GetErrMsg(), ddwEndTime-ddwBeginTime);
	}
	return 0;
}
#endif

#if 0
//调用fcgi1-lyh
int main(int argc, char **argv)
{
	uint64_t ddwBeginTime = GetCurTime();

	TLOG->Init(argv[0]);

	TLOG->SetConsoleShow(1);

	#if 0
	CAdRecommendReq oReq;
	oReq.skucnt = 5;
	oReq.userid = "3A96AA61EDC6E8ADDCAC5256720B9810";
	oReq.usertype = 1;
	oReq.token = "afd5428337d6cbfcf42557bbaa492827";
	
	
	std::string sReq = ad::serialize::to_json(oReq);
	#endif
	CReq oReq;
	oReq.req = "test_http_stub";
	oReq.code = 88;
	
	CHttpStub oStub("", "");
	oStub.SetAddr("192.168.145.72", 8083);
	//http://mrjx.jd.com/fcgi-bin/fcgi1_lyh?txt=1&debug=Boss@dump
	
	//oHttpClient.SetHttpFlag(true);
	oStub.SetReqUri("http://mrjx.jd.com/fcgi-bin/fcgi1_lyh?txt=test_http_stub");
	oStub.SetReqHeader("Accept-Charset", "utf-8");
	oStub.SetReqMethod(HTTPMETHOD_POST);//post
	oStub.SetTimeOutMs(3000);

	
	oStub.SetProtocol(PRO_TCP);
	
	
	CProtocolWaiter oWaiter;
	int iRet = oStub.AsyncInvokeJson(oReq, &oWaiter);
	if(iRet != 0)
	{
		TLOG_ERR("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}
	else
	{
		TLOG_MSG("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}

	oWaiter.Wait(15);
	
	CResp oResp;
	oStub.GetResult(oResp);
	std::string sResp = ad::serialize::to_json(oResp, 0);
	
	TLOG_MSG("resp[%s], oResp[%s], errmsg[%s]", oStub.GetRespData().c_str(), sResp.c_str(), oStub.GetErrMsg());
	
	uint64_t ddwEndTime = GetCurTime();
	
	if(oStub.GetRespData().empty())
	{
		TLOG_ERR("fail, costtime[%u]us", ddwEndTime-ddwBeginTime);
	}
	else
	{
		TLOG_MSG("costtime[%u]us", ddwEndTime-ddwBeginTime);
	}
	
	return 0;
}

#endif

#if 1
//调用北京
int main(int argc, char **argv)
{
	uint64_t ddwBeginTime = GetCurTime();

	TLOG->Init(argv[0]);

	TLOG->SetConsoleShow(1);

	
	CAdRecommendReq oReq;
	oReq.skucnt = 5;
	oReq.userid = "3A96AA61EDC6E8ADDCAC5256720B9810";
	oReq.usertype = 1;
	oReq.token = "afd5428337d6cbfcf42557bbaa492827";
	
	std::string sReq = ad::serialize::to_json(oReq);
	CHttpStub oStub("", "");
	//oStub.SetAddr("10.191.87.204", 8080);
	oStub.SetProtocol(PRO_TCP);
	
	//oHttpClient.SetHttpFlag(true);
	oStub.SetReqUri("http://bdsp.jd.local/operad/ad/recommend");
	oStub.SetReqHeader("Accept-Charset", "utf-8");
	oStub.SetReqMethod(HTTPMETHOD_POST);//post
	oStub.SetTimeOutMs(15);
	
	
	CProtocolWaiter oWaiter;
	int iRet = oStub.AsyncInvokeJson(oReq, &oWaiter);
	if(iRet != 0)
	{
		TLOG_ERR("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}
	else
	{
		TLOG_MSG("iRet[%d], errmsg[%s]", iRet, oStub.GetErrMsg());
	}

	oWaiter.Wait(15);
	
	CAdRecommendResp oResp;
	oStub.GetResult(oResp);
	std::string sResp = ad::serialize::to_json(oResp);
	//TLOG_MSG("resp[%s], oResp[%s], errmsg[%s]", oStub.GetRespData().c_str(), sResp.c_str(), oStub.GetErrMsg());
	uint64_t ddwEndTime = GetCurTime();
	if(oStub.GetRespData().empty())
	{
		TLOG_ERR("fail, costtime[%u]us", ddwEndTime-ddwBeginTime);
	}
	else
	{
		TLOG_MSG("costtime[%u]us", ddwEndTime-ddwBeginTime);
	}
	
	return 0;
}

#endif


