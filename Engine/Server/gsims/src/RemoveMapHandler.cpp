#include "RemoveMapHandler.h"
#include "RemoveMapRequest.h"
#include "AugeService.h"
#include "AugeCarto.h"

namespace auge
{
	RemoveMapHandler::RemoveMapHandler()
	{

	}

	RemoveMapHandler::~RemoveMapHandler()
	{

	}

	const char*	RemoveMapHandler::GetName()
	{
		return "RemoveMap";
	}

	WebRequest*	RemoveMapHandler::ParseRequest(rude::CGI& cgi)
	{
		RemoveMapRequest* pRequest = new RemoveMapRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RemoveMapHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* RemoveMapHandler::Execute(WebRequest* pWebRequest)
	{				
		RemoveMapRequest* pRequest = static_cast<RemoveMapRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map* pMap = pCartoManager->GetMap(name);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Map %s does not exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pCartoManager->RemoveMap(name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("RemoveMap");
		return pSusResponse;
	}

	WebResponse* RemoveMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}