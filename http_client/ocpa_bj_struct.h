#ifndef OCPA_BJ_STRUCT_H
#define OCPA_BJ_STRUCT_H

#include "serialize.h" 		//对象序列化类
#include <string>
#include <stdint.h>
#include "jserialize.h"

using namespace boss;
using namespace ad::serialize;
using namespace boss::serialize;

namespace ad
{
	namespace ocpa
	{
		//调用北京请求的结构
        class CAdRecommendReq 
        {
        public:
			CAdRecommendReq():usertype(0),skucnt(0){}
			~CAdRecommendReq(){}
			
			template<class AR>
            AR &serialize(AR &ar)
            {
                _SERIALIZE_(ar, token);
                _SERIALIZE_(ar, userid);
                _SERIALIZE_(ar, usertype);
                _SERIALIZE_(ar, skucnt);
                return ar;
            };
			 
        public:
            std::string token;
            std::string userid;
            uint32_t    usertype;
            uint32_t    skucnt;

           
        };

		
		class CAdRecommendNode 
		{
		public:
			CAdRecommendNode():skuid(0),sku_cid3(0){}
			~CAdRecommendNode(){}
			template<class AR>
            AR &serialize(AR &ar)
            {
                _SERIALIZE_(ar, skuid);
                _SERIALIZE_(ar, sku_cid3);
                return ar;
            };
		public:
            uint64_t  skuid;
            uint32_t  sku_cid3;

            
        };
		
		//调用北京返回的结果
        class CAdRecommendResp 
       	{
       	public:
			CAdRecommendResp():errcode(0),cpc(0),total(0){}
			~CAdRecommendResp(){}
				
            template<class AR>
            AR &serialize(AR &ar)
            {
                _SERIALIZE_(ar, cpc);
                _SERIALIZE_(ar, data);
                _SERIALIZE_(ar, total);
                _SERIALIZE_(ar, errcode);
                _SERIALIZE_(ar, errmsg);
                return ar;
            };
			
       	public:
			
            uint32_t     errcode;
            std::string errmsg;
            uint32_t    cpc;
            uint32_t    total;
            std::vector<CAdRecommendNode> data;

        };
		
	}
}

#endif
