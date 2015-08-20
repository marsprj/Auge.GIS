#include "LoginHandler.h"
#include "LoginRequest.h"
//#include "LoginResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	LoginHandler::LoginHandler()
	{

	}

	LoginHandler::~LoginHandler()
	{

	}

	const char*	LoginHandler::GetName()
	{
		return "Login";
	}

	const char*	LoginHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	LoginHandler::ParseRequest(rude::CGI& cgi)
	{
		LoginRequest* pRequest = new LoginRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	LoginHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	LoginHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//LoginRequest* pRequest = new LoginRequest();
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

	WebResponse* LoginHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		LoginRequest* pRequest = static_cast<LoginRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		UserManager* pUserManager = augeGetUserManagerInstance();

		WebResponse* pWebResponse = NULL;
		//if((name==NULL))
		//{
		//	EnumUser* pUsers = pUserManager->Logins();
		//	LoginResponse* pResponse = new LoginResponse(pRequest);
		//	pResponse->SetUsers(pUsers);
		//	pWebResponse = pResponse;
		//}
		//else
		//{
		//	User *pUser = pUserManager->Login(name);
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
		//		LoginResponse* pResponse = new LoginResponse(pRequest);
		//		pResponse->SetUser(pUser);
		//		pWebResponse = pResponse;
		//	}

		//}

		return pWebResponse;
	}

	WebResponse* LoginHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}