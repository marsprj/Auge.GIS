#include "GetLoginCountHandler.h"
#include "GetLoginCountRequest.h"
#include "GetLoginCountResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetLoginCountHandler::GetLoginCountHandler()
	{

	}

	GetLoginCountHandler::~GetLoginCountHandler()
	{

	}

	const char*	GetLoginCountHandler::GetName()
	{
		return "GetLoginCount";
	}

	const char*	GetLoginCountHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetLoginCountHandler::ParseRequest(rude::CGI& cgi)
	{
		GetLoginCountRequest* pRequest = new GetLoginCountRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetLoginCountHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetLoginCountHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//GetLoginCountRequest* pRequest = new GetLoginCountRequest();
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

	WebResponse* GetLoginCountHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		UserManager* pUserManager = augeGetUserManagerInstance();
		GetLoginCountRequest* pRequest = static_cast<GetLoginCountRequest*>(pWebRequest);
		GetLoginCountResponse* pResponse = new GetLoginCountResponse(pRequest);
		pResponse->SetCount(pUserManager->GetLoginedUserCount());
		return pResponse;
	}

	WebResponse* GetLoginCountHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}