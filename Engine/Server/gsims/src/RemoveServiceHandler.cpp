#include "RemoveServiceHandler.h"
#include "RemoveServiceRequest.h"
#include "RemoveServiceResponse.h"
#include "AugeService.h"

namespace auge
{
	RemoveServiceHandler::RemoveServiceHandler()
	{

	}

	RemoveServiceHandler::~RemoveServiceHandler()
	{

	}

	const char*	RemoveServiceHandler::GetName()
	{
		return "RemoveService";
	}

	WebRequest*	RemoveServiceHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		RemoveServiceRequest* pRequest = new RemoveServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RemoveServiceHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* RemoveServiceHandler::Execute(WebRequest* pWebRequest)
	{
		Service* pService = NULL;
		ServiceManager* pServiceManager = NULL;		
		RemoveServiceRequest* pRequest = static_cast<RemoveServiceRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		pServiceManager = augeGetServiceManagerInstance();
		pService = pServiceManager->GetService(name);
		if(pService==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s does not exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pServiceManager->Unregister(name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("RemoveService");
		return pSusResponse;
	}

	WebResponse* RemoveServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}