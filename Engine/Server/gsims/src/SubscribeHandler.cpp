#include "SubscribeHandler.h"
#include "SubscribeRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	SubscribeHandler::SubscribeHandler()
	{

	}

	SubscribeHandler::~SubscribeHandler()
	{

	}

	const char*	SubscribeHandler::GetName()
	{
		return "Subscribe";
	}

	const char*	SubscribeHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	SubscribeHandler::ParseRequest(rude::CGI& cgi)
	{
		SubscribeRequest* pRequest = new SubscribeRequest();
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

	WebRequest* SubscribeHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	SubscribeHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* SubscribeHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		SubscribeRequest* pRequest = static_cast<SubscribeRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* keyword = NULL;
		const char* theme = pRequest->GetTheme();
		g_uint keyword_count = pRequest->GetKeywordCount();

		if(theme==NULL)
		{
			const char* msg = "参数[Theme]未定义";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(keyword_count==0)
		{
			const char* msg = "参数[Keywords]未定义或非法";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		char msg[AUGE_MSG_MAX];
		RESULTCODE rc = AG_SUCCESS;
		SubscriptionManager* pSubscriptionManager = augeGetSubscriptionManagerInstance();

		for(g_uint i=0; i<keyword_count; i++)
		{
			keyword = pRequest->GetKeyword(i);
			if(keyword!=NULL)
			{
				if(pSubscriptionManager->IsSubscribed(pUser->GetID(), theme, keyword))
				{
					g_sprintf(msg, "关键字[%s]已经订阅", keyword);
					pError->SetError(msg);
					pLogger->Error(msg);
				}
				else
				{
					rc = pSubscriptionManager->Subscribe(pUser->GetID(), theme, keyword);
					if(rc!=AG_SUCCESS)
					{

					}
				}
			}
		}
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* SubscribeHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}