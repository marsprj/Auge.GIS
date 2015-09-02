#include "GetMapScaleHandler.h"
#include "GetMapScaleRequest.h"
#include "GetMapScaleResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"

namespace auge
{
	GetMapScaleHandler::GetMapScaleHandler()
	{

	}

	GetMapScaleHandler::~GetMapScaleHandler()
	{

	}

	const char*	GetMapScaleHandler::GetName()
	{
		return "GetMapScale";
	}

	const char*	GetMapScaleHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetMapScaleHandler::ParseRequest(rude::CGI& cgi)
	{
		GetMapScaleRequest* pRequest = new GetMapScaleRequest();
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

	WebRequest*	GetMapScaleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetMapScaleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetMapScaleRequest* pRequest = new GetMapScaleRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* GetMapScaleHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetMapScaleRequest* pRequest = static_cast<GetMapScaleRequest*>(pWebRequest);
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		g_int srid = pRequest->GetSRID();
		g_uint screen_width = pRequest->GetScreenWidth();
		float map_width = pRequest->GetMapWidth();

		float map_scale = auge_get_map_sacle(srid, map_width, screen_width);
		
		GetMapScaleResponse* pResponse = new GetMapScaleResponse(pRequest);
		pResponse->SetMapScale(map_scale);
		return pResponse;
	}

	WebResponse* GetMapScaleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GetMapScaleRequest* pRequest = static_cast<GetMapScaleRequest*>(pWebRequest);
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		g_int srid = pRequest->GetSRID();
		g_uint screen_width = pRequest->GetScreenWidth();
		float map_width = pRequest->GetMapWidth();

		float map_scale = auge_get_map_sacle(srid, map_width, screen_width);

		GetMapScaleResponse* pResponse = new GetMapScaleResponse(pRequest);
		pResponse->SetMapScale(map_scale);
		return pResponse;
	}
}
