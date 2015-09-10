#include "IsLoginedHandler.h"
#include "IsLoginedRequest.h"
//#include "IsLoginResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	IsLoginHandler::IsLoginHandler()
	{

	}

	IsLoginHandler::~IsLoginHandler()
	{

	}

	const char*	IsLoginHandler::GetName()
	{
		return "IsLogin";
	}

	const char*	IsLoginHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	IsLoginHandler::ParseRequest(rude::CGI& cgi)
	{
		IsLoginRequest* pRequest = new IsLoginRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	IsLoginHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	IsLoginHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//IsLoginRequest* pRequest = new IsLoginRequest();
		////if(!pRequest->Create(pxDoc, pMap))
		//if(!pRequest->Create(pxDoc))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	return NULL;
		//}
		//return pRequest;
		return NULL;
	}

	WebResponse* IsLoginHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GLogger* pLogger = augeGetLoggerInstance();
		IsLoginRequest* pRequest = static_cast<IsLoginRequest*>(pWebRequest);
		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		UserManager* pUserManager = augeGetUserManagerInstance();
		bool logined = pUserManager->IsLogined(name);
		if(!logined)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "User [%s] is not logined", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());

		return pSusResponse;
	}

	WebResponse* IsLoginHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}