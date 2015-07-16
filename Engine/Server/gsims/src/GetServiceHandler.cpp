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

	WebRequest*	GetServiceHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetServiceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetServiceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return new GetServiceResponse(static_cast<GetServiceRequest*>(pWebRequest));
	}

	WebResponse* GetServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return NULL;
	}
}