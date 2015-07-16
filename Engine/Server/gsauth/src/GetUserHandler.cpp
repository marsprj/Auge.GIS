#include "GetUserHandler.h"
#include "GetUserRequest.h"
#include "GetUserResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetUserHandler::GetUserHandler()
	{

	}

	GetUserHandler::~GetUserHandler()
	{

	}

	const char*	GetUserHandler::GetName()
	{
		return "GetUser";
	}

	WebRequest*	GetUserHandler::ParseRequest(rude::CGI& cgi)
	{
		GetUserRequest* pRequest = new GetUserRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetUserHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetUserHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//GetUserRequest* pRequest = new GetUserRequest();
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

	WebResponse* GetUserHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GetUserRequest* pRequest = static_cast<GetUserRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		UserManager* pUserManager = augeGetUserManagerInstance();

		WebResponse* pWebResponse = NULL;
		if((name==NULL)||strlen(name)==0)
		{
			EnumUser* pUsers = pUserManager->GetUsers();
			GetUserResponse* pResponse = new GetUserResponse(pRequest);
			pResponse->SetUsers(pUsers);
			pWebResponse = pResponse;
		}
		else
		{
			User *pUser = pUserManager->GetUser(name);
			if(pUser==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "User [%s] does not exist.", name);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse = pExpResponse;
			}
			else
			{
				GetUserResponse* pResponse = new GetUserResponse(pRequest);
				pResponse->SetUser(pUser);
				pWebResponse = pResponse;
			}

		}

		return pWebResponse;
	}

	WebResponse* GetUserHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}