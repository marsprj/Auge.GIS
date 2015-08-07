#include "LogoutHandler.h"
#include "LogoutRequest.h"
//#include "LogoutResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	LogoutHandler::LogoutHandler()
	{

	}

	LogoutHandler::~LogoutHandler()
	{

	}

	const char*	LogoutHandler::GetName()
	{
		return "Logout";
	}

	WebRequest*	LogoutHandler::ParseRequest(rude::CGI& cgi)
	{
		LogoutRequest* pRequest = new LogoutRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	LogoutHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	LogoutHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//LogoutRequest* pRequest = new LogoutRequest();
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

	WebResponse* LogoutHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		LogoutRequest* pRequest = static_cast<LogoutRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		UserManager* pUserManager = augeGetUserManagerInstance();

		WebResponse* pWebResponse = NULL;
		//if((name==NULL)||strlen(name)==0)
		//{
		//	EnumUser* pUsers = pUserManager->Logouts();
		//	LogoutResponse* pResponse = new LogoutResponse(pRequest);
		//	pResponse->SetUsers(pUsers);
		//	pWebResponse = pResponse;
		//}
		//else
		//{
		//	User *pUser = pUserManager->Logout(name);
		//	if(pUser==NULL)
		//	{
		//		char msg[AUGE_MSG_MAX];
		//		g_sprintf(msg, "User [%s] does not exist.", name);
		//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//		pExpResponse->SetMessage(msg);
		//		pWebResponse = pExpResponse;
		//	}
		//	else
		//	{
		//		LogoutResponse* pResponse = new LogoutResponse(pRequest);
		//		pResponse->SetUser(pUser);
		//		pWebResponse = pResponse;
		//	}

		//}

		return pWebResponse;
	}

	WebResponse* LogoutHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}