#include "RemoveServiceHandler.h"
#include "RemoveServiceRequest.h"
#include "RemoveServiceResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

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

	const char*	RemoveServiceHandler::GetDescription()
	{
		return GetName();
	}


	WebRequest*	RemoveServiceHandler::ParseRequest(rude::CGI& cgi)
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

	WebRequest*	RemoveServiceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveServiceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RemoveServiceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		Service* pService = NULL;
		ServiceManager* pServiceManager = NULL;		
		RemoveServiceRequest* pRequest = static_cast<RemoveServiceRequest*>(pWebRequest);

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

		RESULTCODE rc = pServiceManager->Unregister(user_id, name);
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

	WebResponse* RemoveServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}