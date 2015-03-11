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

	WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		CapabilitiesRequest* pRequest = new CapabilitiesRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->Info();
		return pRequest;
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
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
