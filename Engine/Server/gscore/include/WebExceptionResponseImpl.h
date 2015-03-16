#ifndef __AUGE_WEB_EXCEPTION_RESPONSE_H__
#define __AUGE_WEB_EXCEPTION_RESPONSE_H__

#include "AugeWebCore.h"

namespace auge
{
	class WebExceptionResponseImpl : public WebExceptionResponse
	{
	public:
		WebExceptionResponseImpl();
		virtual ~WebExceptionResponseImpl();
	public:
		virtual	const char*		GetMessage();
		virtual	RESULTCODE		SetMessage(const char* message);
		
		virtual RESULTCODE		Write(WebWriter* pWriter);
		virtual void			Release();
	private:
		char m_message[AUGE_MSG_MAX];
	};
}

#endif //__AUGE_WEB_EXCEPTION_RESPONSE_H__
