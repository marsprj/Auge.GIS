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
		IsLoginRequest* pRequest = static_cast<IsLoginRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		UserManager* pUserManager = augeGetUserManagerInstance();

		WebResponse* pWebResponse = NULL;
		//if((name==NULL)||strlen(name)==0)
		//{
		//	EnumUser* pUsers = pUserManager->IsLogins();
		//	IsLoginResponse* pResponse = new IsLoginResponse(pRequest);
		//	pResponse->SetUsers(pUsers);
		//	pWebResponse = pResponse;
		//}
		//else
		//{
		//	User *pUser = pUserManager->IsLogin(name);
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
		//		IsLoginResponse* pResponse = new IsLoginResponse(pRequest);
		//		pResponse->SetUser(pUser);
		//		pWebResponse = pResponse;
		//	}

		//}

		return pWebResponse;
	}

	WebResponse* IsLoginHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}