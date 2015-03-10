#ifndef __AUGE_IMS_SERVICE_CREATE_RESPONSE_H__
#define __AUGE_IMS_SERVICE_CREATE_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Service;
	class CreateServiceRequest;

	class CreateServiceResponse : public WebResponse
	{
	public:
		CreateServiceResponse(CreateServiceRequest* pRequest);
		virtual ~CreateServiceResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetService(Service* pService);

	private:
		CreateServiceRequest *m_pRequest;
		Service				 *m_pService;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
