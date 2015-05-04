#include "CreateUserHandler.h"
#include "CreateUserRequest.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	CreateUserHandler::CreateUserHandler()
	{

	}

	CreateUserHandler::~CreateUserHandler()
	{

	}

	const char*	CreateUserHandler::GetName()
	{
		return "CreateUser";
	}

	WebRequest*	CreateUserHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateUserRequest* pRequest = new CreateUserRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	CreateUserHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateUserHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//CreateUserRequest* pRequest = new CreateUserRequest();
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

	WebResponse* CreateUserHandler::Execute(WebRequest* pWebRequest)
	{
		CreateUserRequest* pRequest = static_cast<CreateUserRequest*>(pWebRequest);
		const char* name = pRequest->GetName();
		const char* alias = pRequest->GetAlias();
		const char* passwd = pRequest->GetPassword();
		const char* email = pRequest->GetEmail();
		const char* role = pRequest->GetRole();

		UserManager* pUserManager = augeGetUserManagerInstance();
		User* pUser = pUserManager->CreateUser(name, alias, passwd, email, role);

		WebResponse* pWebResponse = NULL;
		if(!pUser)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		else
		{
			WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
			pSusResponse->SetRequest(pRequest->GetRequest());
			pWebResponse = pSusResponse;
			pUser->Release();
		}

		return pWebResponse;
	}

	WebResponse* CreateUserHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		return Execute(pWebRequest);	
	}
}