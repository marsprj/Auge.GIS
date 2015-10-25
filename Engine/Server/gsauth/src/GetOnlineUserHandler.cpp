#include "GetOnlineUserHandler.h"
#include "GetOnlineUserRequest.h"
#include "GetOnlineUserResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetOnlineUserHandler::GetOnlineUserHandler()
	{

	}

	GetOnlineUserHandler::~GetOnlineUserHandler()
	{

	}

	const char*	GetOnlineUserHandler::GetName()
	{
		return "GetOnlineUser";
	}

	const char*	GetOnlineUserHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetOnlineUserHandler::ParseRequest(rude::CGI& cgi)
	{
		GetOnlineUserRequest* pRequest = new GetOnlineUserRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetOnlineUserHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetOnlineUserHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//GetOnlineUserRequest* pRequest = new GetOnlineUserRequest();
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

	WebResponse* GetOnlineUserHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GetOnlineUserRequest* pRequest = static_cast<GetOnlineUserRequest*>(pWebRequest);
		g_uint count = pRequest->GetCount();
		g_uint offset= pRequest->GetOffset();

		UserManager* pUserManager = augeGetUserManagerInstance();
		EnumUser* pUsers = pUserManager->GetLoginedUsers(count, offset);
		GetOnlineUserResponse* pResponse = new GetOnlineUserResponse(pRequest);
		pResponse->SetUsers(pUsers);
		return pResponse;
	}

	WebResponse* GetOnlineUserHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}