#include "UpdateStyleHandler.h"
#include "UpdateStyleRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"


namespace auge
{
	UpdateStyleHandler::UpdateStyleHandler()
	{

	}

	UpdateStyleHandler::~UpdateStyleHandler()
	{

	}

	const char*	UpdateStyleHandler::GetName()
	{
		return "UpdateStyle";
	}

	WebRequest*	UpdateStyleHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		UpdateStyleRequest* pRequest = new UpdateStyleRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	UpdateStyleHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* UpdateStyleHandler::Execute(WebRequest* pWebRequest)
	{
		UpdateStyleRequest* pRequest = static_cast<UpdateStyleRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		const char* text = pRequest->GetStyle();
		RESULTCODE rc = pCartoManager->UpdateStyle(name, text);
		if(rc!=AG_SUCCESS) 
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		else
		{
			WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
			pSusResponse->SetRequest(pRequest->GetRequest());
			pWebResponse = pSusResponse;
		}
		return pWebResponse;
	}

	WebResponse* UpdateStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}