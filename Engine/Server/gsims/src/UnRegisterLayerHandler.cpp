#include "UnRegisterLayerHandler.h"
#include "UnRegisterLayerRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeCarto.h"

namespace auge
{
	UnRegisterLayerHandler::UnRegisterLayerHandler()
	{

	}

	UnRegisterLayerHandler::~UnRegisterLayerHandler()
	{

	}

	const char*	UnRegisterLayerHandler::GetName()
	{
		return "UnRegisterLayer";
	}

	const char*	UnRegisterLayerHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	UnRegisterLayerHandler::ParseRequest(rude::CGI& cgi)
	{
		UnRegisterLayerRequest* pRequest = new UnRegisterLayerRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* UnRegisterLayerHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	UnRegisterLayerHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* UnRegisterLayerHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		UnRegisterLayerRequest* pRequest = static_cast<UnRegisterLayerRequest*>(pWebRequest);

		const char* mapName = pRequest->GetMapName();
		const char* layerName = pRequest->GetLayerName();
		augeLayerType layerType = augeLayerFeature;
		g_int styleID = -1;

		if(mapName==NULL)
		{
			const char* msg = "Parameter mapName is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		RESULTCODE rc = pCartoManager->RemoveLayer(mapName, layerName);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			pExpResponse->SetMessage(pError->GetLastError()); 
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* UnRegisterLayerHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}