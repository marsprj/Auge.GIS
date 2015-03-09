#ifndef __AUGE_IMS_SERVICE_REMOVE_RESPONSE_H__
#define __AUGE_IMS_SERVICE_REMOVE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Service;
	class RemoveServiceRequest;

	class RemoveServiceResponse : public WebResponse
	{
	public:
		RemoveServiceResponse(RemoveServiceRequest* pRequest);
		virtual ~RemoveServiceResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	private:
		RemoveServiceRequest *m_pRequest;
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
