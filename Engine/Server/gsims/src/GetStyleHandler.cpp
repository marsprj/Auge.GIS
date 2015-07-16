#include "GetStyleHandler.h"
#include "GetStyleRequest.h"
#include "GetStyleResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"


namespace auge
{
	GetStyleHandler::GetStyleHandler()
	{

	}

	GetStyleHandler::~GetStyleHandler()
	{

	}

	const char*	GetStyleHandler::GetName()
	{
		return "GetStyle";
	}

	WebRequest*	GetStyleHandler::ParseRequest(rude::CGI& cgi)
	{
		GetStyleRequest* pRequest = new GetStyleRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* GetStyleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetStyleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetStyleHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		//return new GetStyleResponse(static_cast<GetStyleRequest*>(pWebRequest));

		GetStyleRequest* pRequest = static_cast<GetStyleRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			EnumStyle* pStyles = pCartoManager->GetStyles();
			GetStyleResponse* pResponse = new GetStyleResponse(pRequest);
			pResponse->SetStyles(pStyles);
			pWebResponse = pResponse;
		}
		else
		{
			char* text = pCartoManager->GetStyleText(name);
			if(text!=NULL)
			{
				GetStyleResponse* pResponse = new GetStyleResponse(pRequest);
				pResponse->SetStyleText(text);
				pWebResponse = pResponse;
			}
			else
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Style [%s] not found", name);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse = pExpResponse;
			}
		}
		return pWebResponse;
	}

	WebResponse* GetStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}