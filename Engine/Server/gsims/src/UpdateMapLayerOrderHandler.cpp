#include "UpdateMapLayerOrderHandler.h"
#include "UpdateMapLayerOrderRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"

namespace auge
{
	UpdateMapLayerOrderHandler::UpdateMapLayerOrderHandler()
	{

	}

	UpdateMapLayerOrderHandler::~UpdateMapLayerOrderHandler()
	{

	}

	const char*	UpdateMapLayerOrderHandler::GetName()
	{
		return "UpdateMapLayerOrder";
	}

	WebRequest*	UpdateMapLayerOrderHandler::ParseRequest(rude::CGI& cgi)
	{
		UpdateMapLayerOrderRequest* pRequest = new UpdateMapLayerOrderRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* UpdateMapLayerOrderHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	UpdateMapLayerOrderHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* UpdateMapLayerOrderHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GLogger* pLogger = augeGetLoggerInstance();
		UpdateMapLayerOrderRequest* pRequest = static_cast<UpdateMapLayerOrderRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			const char* msg = NULL;
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* layers = pRequest->GetLayers();

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		RESULTCODE rc = pCartoManager->UpdateMapLayers(name, layers);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* UpdateMapLayerOrderHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}