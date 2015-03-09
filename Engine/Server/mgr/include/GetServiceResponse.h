#ifndef __AUGE_IMS_SERVICE_GET_RESPONSE_H__
#define __AUGE_IMS_SERVICE_GET_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Service;
	class GetServiceRequest;

	class GetServiceResponse : public WebResponse
	{
	public:
		GetServiceResponse(GetServiceRequest* pRequest);
		virtual ~GetServiceResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);
		
	private:
		GetServiceRequest *m_pRequest;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
