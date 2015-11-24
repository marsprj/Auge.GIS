#include "GetSubscriptionThemeHandler.h"
#include "GetSubscriptionThemeRequest.h"
#include "GetSubscriptionThemeResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetSubscriptionThemeHandler::GetSubscriptionThemeHandler()
	{

	}

	GetSubscriptionThemeHandler::~GetSubscriptionThemeHandler()
	{

	}

	const char*	GetSubscriptionThemeHandler::GetName()
	{
		return "GetSubscriptionTheme";
	}

	const char*	GetSubscriptionThemeHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetSubscriptionThemeHandler::ParseRequest(rude::CGI& cgi)
	{
		GetSubscriptionThemeRequest* pRequest = new GetSubscriptionThemeRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		pRequest->Info();
		return pRequest;
	}

	WebRequest* GetSubscriptionThemeHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetSubscriptionThemeHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetSubscriptionThemeHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetSubscriptionThemeRequest* pRequest = static_cast<GetSubscriptionThemeRequest*>(pWebRequest);
		GetSubscriptionThemeResponse* pResponse = new GetSubscriptionThemeResponse(pRequest);
		return pResponse;
	}

	WebResponse* GetSubscriptionThemeHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}