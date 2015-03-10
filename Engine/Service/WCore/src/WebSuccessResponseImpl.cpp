#include "WebSuccessResponseImpl.h"
#include <stdio.h>s

namespace auge
{
	WebSuccessResponse* augeCreateWebSuccessResponse()
	{
		return new WebSuccessResponseImpl();
	}

	WebSuccessResponseImpl::WebSuccessResponseImpl()
	{

	}

	WebSuccessResponseImpl::~WebSuccessResponseImpl()
	{

	}

	RESULTCODE WebSuccessResponseImpl::SetRequest(const char* request)
	{
		if(request==NULL)
		{
			return AG_FAILURE;
		}

		g_strncpy(m_request, request, AUGE_MSG_MAX);
		return AG_SUCCESS;
	}

	const char* WebSuccessResponseImpl::GetRequest()
	{
		return m_request;
	}

	RESULTCODE WebSuccessResponseImpl::Write(WebWriter* pWriter)
	{
		char msg[AUGE_MSG_LONG_MAX] = {0};
		g_sprintf(msg, "<?xml version=\"1.0\"?><%s>Success</%s>", m_request,m_request); 

		pWriter->WriteHead("text/xml");
		pWriter->Write((g_uchar*)msg, strlen(msg));
		pWriter->WriteTail();
		return AG_SUCCESS;
	}

	void WebSuccessResponseImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}
}