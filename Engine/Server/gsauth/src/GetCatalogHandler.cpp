#include "GetCatalogHandler.h"
#include "GetCatalogRequest.h"
#include "GetCatalogResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetCatalogHandler::GetCatalogHandler()
	{

	}

	GetCatalogHandler::~GetCatalogHandler()
	{

	}

	const char*	GetCatalogHandler::GetName()
	{
		return "GetCatalog";
	}

	const char*	GetCatalogHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetCatalogHandler::ParseRequest(rude::CGI& cgi)
	{
		GetCatalogRequest* pRequest = new GetCatalogRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetCatalogHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetCatalogHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//GetCatalogRequest* pRequest = new GetCatalogRequest();
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

	WebResponse* GetCatalogHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GetCatalogRequest* pRequest = static_cast<GetCatalogRequest*>(pWebRequest);
		const char* user_name = pRequest->GetUser();
		if(user_name==NULL)
		{
			const char* msg = "Paramer [User] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		User* pnUser = NULL;
		UserManager* pUserManager = augeGetUserManagerInstance();
		pnUser = pUserManager->GetUser(user_name);
		if(pnUser==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "User [%s] does not exist.", user_name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		AUGE_SAFE_RELEASE(pnUser);
		GetCatalogResponse* pResponse = new GetCatalogResponse(pRequest);
		pResponse->SetUser(pUser);
		return pResponse;
	}

	WebResponse* GetCatalogHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}