#include "GetMapHandler.h"
#include "GetMapRequest.h"
#include "GetMapResponse.h"
#include "AugeCarto.h"

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

	WebRequest* GetMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;

		//GetMapRequest* pRequest = static_cast<GetMapRequest*>(pWebRequest);
		//GetMapResponse* pWebResponse = new GetMapResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->GetMaps();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GetMapRequest* pRequest = static_cast<GetMapRequest*>(pWebRequest);
		GetMapResponse* pWebResponse = new GetMapResponse(pRequest);
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			EnumMap* pMaps = pCartoManager->GetMaps();
			pWebResponse->SetMaps(pMaps);
		}
		else
		{
			Map* pMap = pCartoManager->LoadMap(name);
			pWebResponse->SetMap(pMap);
		}

		pWebResponse->SetWebContext(pWebContext);
		return pWebResponse;
	}
}