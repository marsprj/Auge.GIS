#include "CapabilitiesHandler.h"
#include "CapabilitiesRequest.h"
#include "CapabilitiesResponse.h"

namespace auge
{
	CapabilitiesHandler::CapabilitiesHandler()
	{

	}

	CapabilitiesHandler::~CapabilitiesHandler()
	{

	}

	const char*	CapabilitiesHandler::GetName()
	{
		return "GetCapabilities";
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi)
	{
		CapabilitiesRequest* pRequest = new CapabilitiesRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(XDocument* pxDoc)
	{
		CapabilitiesRequest* pRequest = new CapabilitiesRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest)
	{
		return new CapabilitiesResponse(static_cast<CapabilitiesRequest*>(pWebRequest));
	}

	WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}