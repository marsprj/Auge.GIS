#include "GetServiceHandler.h"
#include "GetServiceRequest.h"
#include "GetServiceResponse.h"


namespace auge
{
	GetServiceHandler::GetServiceHandler()
	{

	}

	GetServiceHandler::~GetServiceHandler()
	{

	}

	const char*	GetServiceHandler::GetName()
	{
		return "GetService";
	}

	WebRequest*	GetServiceHandler::ParseRequest(rude::CGI& cgi)
	{
		GetServiceRequest* pRequest = new GetServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetServiceHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* GetServiceHandler::Execute(WebRequest* pWebRequest)
	{
		return new GetServiceResponse(static_cast<GetServiceRequest*>(pWebRequest));
	}
}