#include "GetMapHandler.h"
#include "GetMapRequest.h"
#include "GetMapResponse.h"


namespace auge
{
	GetMapHandler::GetMapHandler()
	{

	}

	GetMapHandler::~GetMapHandler()
	{

	}

	const char*	GetMapHandler::GetName()
	{
		return "GetMap";
	}

	WebRequest*	GetMapHandler::ParseRequest(rude::CGI& cgi)
	{
		GetMapRequest* pRequest = new GetMapRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetMapHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest)
	{
		return new GetMapResponse(static_cast<GetMapRequest*>(pWebRequest));
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}