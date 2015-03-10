#ifndef __AUGE_IMS_REGISTER_HANDLER_RESPONSE_H__
#define __AUGE_IMS_REGISTER_HANDLER_RESPONSE_H__

#include "AugeWebEngine.h"
#include <string>

namespace auge
{
	class Service;
	class RegisterHandlerRequest;

	class RegisterHandlerResponse : public WebResponse
	{
	public:
		RegisterHandlerResponse(RegisterHandlerRequest* pRequest);
		virtual ~RegisterHandlerResponse();
	public:
		virtual	RESULTCODE		Write(WebWriter* pWriter);

	public:
		void	SetService(Service* pService);

	private:
		RegisterHandlerRequest *m_pRequest;
		Service				 *m_pService;	
	};
}

#endif //__AUGE_IMS_GETCAPABILITIES_RESPONSE_H__
