#include "GetColorMapHandler.h"
#include "GetColorMapRequest.h"
#include "GetColorMapResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"

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
		WebResponse* pWebResponse = NULL;
		GetColorMapRequest* pRequest = static_cast<GetColorMapRequest*>(pWebRequest);
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		
		g_int id = pRequest->GetID();
		if(id<=0)
		{
			GetColorMapResponse* pResponse =  new GetColorMapResponse(static_cast<GetColorMapRequest*>(pWebRequest));
			EnumColorMap* pColorMaps = pCartoManager->GetColorMaps();
			pResponse->SetColorMaps(pColorMaps);
			pWebResponse = pResponse;
		}
		else
		{
			ColorMap* pColorMap = pCartoManager->GetColorMap(id);
			if(pColorMap)
			{
				GetColorMapResponse* pResponse =  new GetColorMapResponse(static_cast<GetColorMapRequest*>(pWebRequest));
				pResponse->SetColorMap(pColorMap);
				pWebResponse = pResponse;
			}
			else
			{
				const char* msg = "Cannot Load ColorMap";
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse = pExpResponse;
			}
		}
		
		return pWebResponse;
	}

	WebResponse* GetColorMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		GetColorMapRequest* pRequest = static_cast<GetColorMapRequest*>(pWebRequest);
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		g_int id = pRequest->GetID();
		if(id<=0)
		{
			GetColorMapResponse* pResponse =  new GetColorMapResponse(static_cast<GetColorMapRequest*>(pWebRequest));
			EnumColorMap* pColorMaps = pCartoManager->GetColorMaps();
			pResponse->SetColorMaps(pColorMaps);
			pResponse->SetWebContext(pWebContext);
			pWebResponse = pResponse;
		}
		else
		{
			ColorMap* pColorMap = pCartoManager->GetColorMap(id);
			if(pColorMap)
			{
				GetColorMapResponse* pResponse =  new GetColorMapResponse(static_cast<GetColorMapRequest*>(pWebRequest));
				pResponse->SetColorMap(pColorMap);
				pResponse->SetWebContext(pWebContext);
				pWebResponse = pResponse;
			}
			else
			{
				const char* msg = "Cannot Load ColorMap";
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse = pExpResponse;
			}
		}

		return pWebResponse;
	}
}
