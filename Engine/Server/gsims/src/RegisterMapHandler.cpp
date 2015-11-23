#include "RegisterMapHandler.h"
#include "RegisterMapRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeUser.h"

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

	const char*	RegisterMapHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RegisterMapHandler::ParseRequest(rude::CGI& cgi)
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

	WebRequest* RegisterMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RegisterMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RegisterMapHandler::Execute(WebRequest* pWebRequest, User* pUser)
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
		
		int mapID = pCartoManager->GetMapID(pUser->GetID(), mapName);
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

		g_uint user_id = pUser->GetID();
		ServiceManager* pServiceManager = augeGetServiceManagerInstance();
		int serviceID = pServiceManager->GetServiceID(user_id, serviceName);
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

	WebResponse* RegisterMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}