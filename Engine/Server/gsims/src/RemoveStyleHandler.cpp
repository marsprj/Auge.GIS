#include "RemoveStyleHandler.h"
#include "RemoveStyleRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	RemoveStyleHandler::RemoveStyleHandler()
	{

	}

	RemoveStyleHandler::~RemoveStyleHandler()
	{

	}

	const char*	RemoveStyleHandler::GetName()
	{
		return "RemoveStyle";
	}

	WebRequest*	RemoveStyleHandler::ParseRequest(rude::CGI& cgi)
	{
		RemoveStyleRequest* pRequest = new RemoveStyleRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* RemoveStyleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveStyleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RemoveStyleHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		RemoveStyleRequest* pRequest = static_cast<RemoveStyleRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		RESULTCODE rc = pCartoManager->RemoveStyle(pUser->GetID(), pRequest->GetName());
		if(rc==AG_SUCCESS)
		{
			WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
			pSusResponse->SetRequest(pRequest->GetRequest());
			pWebResponse = pSusResponse;
		}
		else
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		return pWebResponse;
	}

	WebResponse* RemoveStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}