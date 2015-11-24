#include "GetSubscriptionHandler.h"
#include "GetSubscriptionRequest.h"
#include "GetSubscriptionResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetSubscriptionHandler::GetSubscriptionHandler()
	{

	}

	GetSubscriptionHandler::~GetSubscriptionHandler()
	{

	}

	const char*	GetSubscriptionHandler::GetName()
	{
		return "GetSubscription";
	}

	const char*	GetSubscriptionHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetSubscriptionHandler::ParseRequest(rude::CGI& cgi)
	{
		GetSubscriptionRequest* pRequest = new GetSubscriptionRequest();
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

	WebRequest* GetSubscriptionHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetSubscriptionHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetSubscriptionHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetSubscriptionRequest* pRequest = static_cast<GetSubscriptionRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		SubscriptionManager* pSubscriptionManager = augeGetSubscriptionManagerInstance();

		const char* theme = pRequest->GetTheme();
		if(theme==NULL)
		{
			EnumSubscriptionTheme* pThemes = NULL;
			pThemes = pSubscriptionManager->GetThemes(pUser->GetID());
			GetSubscriptionResponse* pResponse = new GetSubscriptionResponse(pRequest);
			pResponse->SetThemes(pThemes);
			pWebResponse = pResponse;
		}
		else
		{
			SubscriptionTheme* pTheme = NULL;
			pTheme = pSubscriptionManager->GetTheme(pUser->GetID(), theme);
			if(pTheme==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "订阅主题[%s]不存在", theme);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse = pExpResponse;
			}
			else
			{
				GetSubscriptionResponse* pResponse = new GetSubscriptionResponse(pRequest);
				pResponse->SetTheme(pTheme);
				pWebResponse = pResponse;
			}
		}
		
		return pWebResponse;
	}

	WebResponse* GetSubscriptionHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}