#include "SetStyleHandler.h"
#include "SetStyleRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	SetStyleHandler::SetStyleHandler()
	{

	}

	SetStyleHandler::~SetStyleHandler()
	{

	}

	const char*	SetStyleHandler::GetName()
	{
		return "SetStyle";
	}

	const char*	SetStyleHandler::GetDescription()
	{
		return GetName();
	}
	 
	WebRequest*	SetStyleHandler::ParseRequest(rude::CGI& cgi)
	{
		SetStyleRequest* pRequest = new SetStyleRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	SetStyleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		WebRequest* pWebRequest = ParseRequest(cgi);
		if(pWebRequest!=NULL)
		{
			SetStyleRequest* pWRequest = static_cast<SetStyleRequest*>(pWebRequest);
			pWRequest->SetMapName(mapName);
		}
		return pWebRequest;
	}

	WebRequest*	SetStyleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* SetStyleHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* SetStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		SetStyleRequest* pRequest = static_cast<SetStyleRequest*>(pWebRequest);

		const char* mapName = pRequest->GetMapName();
		if(mapName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No Map is attached");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "Cannot load map [%s]", mapName);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		//SetStyleRequest* pRequest = static_cast<SetStyleRequest*>(pWebRequest);

		//const char* mapName = pMap->GetName();
		const char* layerName = pRequest->GetLayerName();
		const char* styleName = pRequest->GetStyleName();

		if(mapName==NULL)
		{
			const char* msg = "Parameter map is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		if(layerName==NULL)
		{
			const char* msg = "Parameter layer is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		if(styleName==NULL)
		{
			const char* msg = "Parameter style is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		g_int l_id = pCartoManager->GetLayerID(pUser->GetID(),layerName, mapName);
		if(l_id < 0)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No Layer %s in Map %s.", layerName, mapName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		g_int s_id = pCartoManager->GetStyleID(styleName);
		if(s_id<0)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No Style %s.", styleName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pCartoManager->SetStyle(l_id, s_id);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		pMap->Release();

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;

		//WebResponse* pWebResponse = Execute(pWebRequest, pWebContext, pMap);
		//pMap->Release();

		//return pWebResponse;
	}

	WebResponse* SetStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
		//SetStyleRequest* pRequest = static_cast<SetStyleRequest*>(pWebRequest);

		//const char* mapName = pMap->GetName();
		//const char* layerName = pRequest->GetLayerName();
		//const char* styleName = pRequest->GetStyleName();

		//if(mapName==NULL)
		//{
		//	const char* msg = "Parameter map is null.";
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}
		//if(layerName==NULL)
		//{
		//	const char* msg = "Parameter layer is null.";
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}
		//if(styleName==NULL)
		//{
		//	const char* msg = "Parameter style is null.";
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}

		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//g_int l_id = pCartoManager->GetLayerID(layerName, mapName);
		//if(l_id < 0)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "No Layer %s in Map %s.", layerName, mapName);
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}

		//g_int s_id = pCartoManager->GetStyleID(styleName);
		//if(s_id<0)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "No Style %s.", styleName);
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}

		//RESULTCODE rc = pCartoManager->SetStyle(l_id, s_id);
		//if(rc!=AG_SUCCESS)
		//{
		//	GError* pError = augeGetErrorInstance();
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(pError->GetLastError());
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(pError->GetLastError());
		//	return pExpResponse;
		//}

		//WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		//pSusResponse->SetRequest(pRequest->GetRequest());
		//return pSusResponse;
	}
}