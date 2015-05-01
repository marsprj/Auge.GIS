#include "GetColorMapHandler.h"
#include "GetColorMapRequest.h"
#include "GetColorMapResponse.h"
#include "AugeCarto.h"

namespace auge
{
	GetColorMapHandler::GetColorMapHandler()
	{

	}

	GetColorMapHandler::~GetColorMapHandler()
	{

	}

	const char*	GetColorMapHandler::GetName()
	{
		return "GetColorMap";
	}

	WebRequest*	GetColorMapHandler::ParseRequest(rude::CGI& cgi)
	{
		GetColorMapRequest* pRequest = new GetColorMapRequest();
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

	WebRequest*	GetColorMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetColorMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetColorMapRequest* pRequest = new GetColorMapRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* GetColorMapHandler::Execute(WebRequest* pWebRequest)
	{
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		EnumColorMap* pColorMaps = pCartoManager->GetColorMaps();
		GetColorMapResponse* pResponse =  new GetColorMapResponse(static_cast<GetColorMapRequest*>(pWebRequest));
		pResponse->SetColorMaps(pColorMaps);
		return pResponse;
	}

	WebResponse* GetColorMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		return Execute(pWebRequest);
	}
}
