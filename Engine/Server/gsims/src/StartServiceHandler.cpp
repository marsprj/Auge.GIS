#include "StartServiceHandler.h"
#include "StartServiceRequest.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	StartServiceHandler::StartServiceHandler()
	{

	}

	StartServiceHandler::~StartServiceHandler()
	{

	}

	const char*	StartServiceHandler::GetName()
	{
		return "StartService";
	}

	const char*	StartServiceHandler::GetDescription()
	{
		return GetName();
	}


	WebRequest*	StartServiceHandler::ParseRequest(rude::CGI& cgi)
	{
		StartServiceRequest* pRequest = new StartServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	StartServiceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	StartServiceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* StartServiceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		Service* pService = NULL;
		ServiceManager* pServiceManager = NULL;		
		StartServiceRequest* pRequest = static_cast<StartServiceRequest*>(pWebRequest);

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

		RESULTCODE rc = pServiceManager->Start(user_id, name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("StartService");
		return pSusResponse;
	}

	WebResponse* StartServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}