#include "UnsubscribeHandler.h"
#include "UnsubscribeRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	UnsubscribeHandler::UnsubscribeHandler()
	{

	}

	UnsubscribeHandler::~UnsubscribeHandler()
	{

	}

	const char*	UnsubscribeHandler::GetName()
	{
		return "Unsubscribe";
	}

	const char*	UnsubscribeHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	UnsubscribeHandler::ParseRequest(rude::CGI& cgi)
	{
		UnsubscribeRequest* pRequest = new UnsubscribeRequest();
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

	WebRequest* UnsubscribeHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	UnsubscribeHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* UnsubscribeHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		UnsubscribeRequest* pRequest = static_cast<UnsubscribeRequest*>(pWebRequest);
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
				//if(pSubscriptionManager->IsSubscribed(pUser->GetID(), theme, keyword))
				//{
				//	g_sprintf(msg, "关键字[%s]已经订阅", keyword);
				//	pError->SetError(msg);
				//	pLogger->Error(msg);
				//}
				//else
				{
					rc = pSubscriptionManager->Unsubscribe(pUser->GetID(), theme, keyword);
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

	WebResponse* UnsubscribeHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}