#include "RegisterMapHandler.h"
#include "RegisterMapRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeCarto.h"

namespace auge
{
	RegisterMapHandler::RegisterMapHandler()
	{

	}

	RegisterMapHandler::~RegisterMapHandler()
	{

	}

	const char*	RegisterMapHandler::GetName()
	{
		return "RegisterMap";
	}

	WebRequest*	RegisterMapHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		RegisterMapRequest* pRequest = new RegisterMapRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RegisterMapHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* RegisterMapHandler::Execute(WebRequest* pWebRequest)
	{
		RegisterMapRequest* pRequest = static_cast<RegisterMapRequest*>(pWebRequest);

		const char* mapName = pRequest->GetMapName();
		const char* serviceName = pRequest->GetServiceName();

		if(mapName==NULL)
		{
			const char* msg = "Parameter mapName is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(serviceName==NULL)
		{
			const char* msg = "Parameter serviceName is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Map* pMap = NULL;
		CartoManager* pCartoManager = NULL;
		pCartoManager = augeGetCartoManagerInstance();
		
		int mapID = pCartoManager->GetMapID(mapName);
		if(mapID<0)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Map %s is not registered.", mapName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		ServiceManager* pServiceManager = augeGetServiceManagerInstance();
		int serviceID = pServiceManager->GetServiceID(serviceName);
		if(serviceID<0)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Service %s is not registered.", serviceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pServiceManager->RegisterMap(serviceID, mapID);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* RegisterMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}