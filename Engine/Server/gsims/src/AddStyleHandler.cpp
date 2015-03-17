#include "AddStyleHandler.h"
#include "AddStyleRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"


namespace auge
{
	AddStyleHandler::AddStyleHandler()
	{

	}

	AddStyleHandler::~AddStyleHandler()
	{

	}

	const char*	AddStyleHandler::GetName()
	{
		return "AddStyle";
	}

	WebRequest*	AddStyleHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		AddStyleRequest* pRequest = new AddStyleRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	AddStyleHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* AddStyleHandler::Execute(WebRequest* pWebRequest)
	{
		AddStyleRequest* pRequest = static_cast<AddStyleRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		const char* text = pRequest->GetStyle();
		RESULTCODE rc = pCartoManager->CreateStyle(name, text);
		if(rc>0)
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

	WebResponse* AddStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}