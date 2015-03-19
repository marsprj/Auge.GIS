#include "WebExceptionResponseImpl.h"
#include <stdio.h>

namespace auge
{
	WebExceptionResponse* augeCreateWebExceptionResponse()
	{
		return new WebExceptionResponseImpl();
	}

	WebExceptionResponseImpl::WebExceptionResponseImpl()
	{

	}

	WebExceptionResponseImpl::~WebExceptionResponseImpl()
	{

	}

	RESULTCODE WebExceptionResponseImpl::SetMessage(const char* message)
	{
		if(message==NULL)
		{
			return AG_FAILURE;
		}

		g_strncpy(m_message, message, AUGE_MSG_MAX);
		return AG_SUCCESS;
	}

	const char* WebExceptionResponseImpl::GetMessage()
	{
		return m_message;
	}

	RESULTCODE WebExceptionResponseImpl::Write(WebWriter* pWriter)
	{
		char msg[AUGE_MSG_LONG_MAX] = {0};
		g_sprintf(msg, "<ows:ExceptionReport xmlns:ows=\"http://www.opengis.net/ows\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" version=\"1.0.0\" xsi:schemaLocation=\"http://www.opengis.net/ows http://ourgis.digitalearth.cn:80/geoserver/schemas/ows/1.0.0/owsExceptionReport.xsd\"><ows:Exception exceptionCode=\"InvalidParameterValue\" locator=\"service\"><ows:ExceptionText>%s</ows:ExceptionText></ows:Exception></ows:ExceptionReport>", m_message);

		pWriter->WriteHead("text/xml");
		pWriter->Write((g_uchar*)msg, strlen(msg));
		pWriter->WriteTail();
		return AG_SUCCESS;
	}

	void WebExceptionResponseImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}
}
