#ifndef __AUGE_WEB_SUCCESS_RESPONSE_H__
#define __AUGE_WEB_SUCCESS_RESPONSE_H__

#include "AugeWebCore.h"

namespace auge
{
	class WebSuccessResponseImpl : public WebSuccessResponse
	{
	public:
		WebSuccessResponseImpl();
		virtual ~WebSuccessResponseImpl();
	public:
		virtual	const char*		GetRequest();
		virtual	RESULTCODE		SetRequest(const char* request);
		
		virtual RESULTCODE		Write(WebWriter* pWriter);
		virtual void			Release();
	private:
		char m_request[AUGE_MSG_MAX];
	};
}

#endif //__AUGE_WEB_SUCCESS_RESPONSE_H__
