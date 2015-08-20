#include "RemoveMapHandler.h"
#include "RemoveMapRequest.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeUser.h"

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

	const char*	RemoveMapHandler::GetDescription()
	{
		return GetName();
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

	WebRequest* RemoveMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RemoveMapHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{				
		RemoveMapRequest* pRequest = static_cast<RemoveMapRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map* pMap = pCartoManager->GetMap(pUser->GetID(), name);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Map %s does not exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pCartoManager->RemoveLayers(name);

		rc = pCartoManager->RemoveMap(name);
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

	WebResponse* RemoveMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}