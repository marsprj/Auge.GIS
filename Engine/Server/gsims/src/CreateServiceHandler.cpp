#include "CreateServiceHandler.h"
#include "CreateServiceRequest.h"
#include "CreateServiceResponse.h"
#include "AugeService.h"

namespace auge
{
	CreateServiceHandler::CreateServiceHandler()
	{

	}

	CreateServiceHandler::~CreateServiceHandler()
	{

	}

	const char*	CreateServiceHandler::GetName()
	{
		return "CreateService";
	}

	WebRequest*	CreateServiceHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		CreateServiceRequest* pRequest = new CreateServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	CreateServiceHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* CreateServiceHandler::Execute(WebRequest* pWebRequest)
	{
		ServiceManager* pServiceManager = NULL;
		Service* pService = NULL;
		CreateServiceRequest* pRequest = static_cast<CreateServiceRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		pServiceManager = augeGetServiceManagerInstance();
		pService = pServiceManager->GetService(name);
		if(pService!=NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s already exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pServiceManager->Register(name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
				
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("CreateService");
		return pSusResponse;
	}

	WebResponse* CreateServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}