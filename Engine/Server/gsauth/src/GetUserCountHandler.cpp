#include "GetUserCountHandler.h"
#include "GetUserCountRequest.h"
#include "GetUserCountResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetUserCountHandler::GetUserCountHandler()
	{

	}

	GetUserCountHandler::~GetUserCountHandler()
	{

	}

	const char*	GetUserCountHandler::GetName()
	{
		return "GetUserCount";
	}

	const char*	GetUserCountHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetUserCountHandler::ParseRequest(rude::CGI& cgi)
	{
		GetUserCountRequest* pRequest = new GetUserCountRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetUserCountHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetUserCountHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//GetUserCountRequest* pRequest = new GetUserCountRequest();
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

	WebResponse* GetUserCountHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		UserManager* pUserManager = augeGetUserManagerInstance();
		GetUserCountRequest* pRequest = static_cast<GetUserCountRequest*>(pWebRequest);
		GetUserCountResponse* pResponse = new GetUserCountResponse(pRequest);
		pResponse->SetCount(pUserManager->GetUserCount());
		return pResponse;
	}

	WebResponse* GetUserCountHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}