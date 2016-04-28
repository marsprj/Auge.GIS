#include "StopServiceHandler.h"
#include "StopServiceRequest.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	StopServiceHandler::StopServiceHandler()
	{

	}

	StopServiceHandler::~StopServiceHandler()
	{

	}

	const char*	StopServiceHandler::GetName()
	{
		return "StopService";
	}

	const char*	StopServiceHandler::GetDescription()
	{
		return GetName();
	}


	WebRequest*	StopServiceHandler::ParseRequest(rude::CGI& cgi)
	{
		StopServiceRequest* pRequest = new StopServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	StopServiceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	StopServiceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* StopServiceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		Service* pService = NULL;
		ServiceManager* pServiceManager = NULL;		
		StopServiceRequest* pRequest = static_cast<StopServiceRequest*>(pWebRequest);

		g_uint user_id = pUser->GetID();
		const char* name = pRequest->GetName();
		pServiceManager = augeGetServiceManagerInstance();
		pService = pServiceManager->GetService(user_id, name);
		if(pService==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s does not exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pServiceManager->Stop(user_id, name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("StopService");
		return pSusResponse;
	}

	WebResponse* StopServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}