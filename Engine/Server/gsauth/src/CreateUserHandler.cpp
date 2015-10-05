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

	const char*	CreateUserHandler::GetDescription()
	{
		return GetName();
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

	WebResponse* CreateUserHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		//CreateUserRequest* pRequest = static_cast<CreateUserRequest*>(pWebRequest);
		//const char* name = pRequest->GetName();
		//const char* alias = pRequest->GetAlias();
		//const char* passwd = pRequest->GetPassword();
		//const char* email = pRequest->GetEmail();
		//const char* role = pRequest->GetRole();

		//UserManager* pUserManager = augeGetUserManagerInstance();
		//User* pnewUser = pUserManager->CreateUser(name, alias, passwd, email, role);

		//WebResponse* pWebResponse = NULL;
		//if(!pnewUser)
		//{
		//	GError* pError = augeGetErrorInstance();
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
		//	pExpResponse->SetMessage(pError->GetLastError());
		//	pWebResponse = pExpResponse;
		//	return pWebResponse;
		//}



		//WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		//pSusResponse->SetRequest(pRequest->GetRequest());
		//pWebResponse = pSusResponse;
		//pnewUser->Release();

		//return pWebResponse;

		return NULL;
	}

	WebResponse* CreateUserHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		CreateUserRequest* pRequest = static_cast<CreateUserRequest*>(pWebRequest);
		const char* name = pRequest->GetName();
		const char* alias = pRequest->GetAlias();
		const char* passwd = pRequest->GetPassword();
		const char* email = pRequest->GetEmail();
		const char* role = pRequest->GetRole();

		UserManager* pUserManager = augeGetUserManagerInstance();
		User* pnewUser = pUserManager->CreateUser(name, alias, passwd, email, role);

		WebResponse* pWebResponse = NULL;
		if(!pnewUser)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		RESULTCODE rc = CreateUserFolder(pnewUser->GetName(), pWebContext->GetUserRoot());
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		pWebResponse = pSusResponse;
		pnewUser->Release();

		return pWebResponse;
	}

	RESULTCODE CreateUserHandler::CreateUserFolder(const char* name, const char* root)
	{
		char user_folder[AUGE_PATH_MAX];
		memset(user_folder, 0, AUGE_PATH_MAX);
		auge_make_path(user_folder, NULL, root, name, NULL);
		auge_mkdir(user_folder);

		return AG_SUCCESS;
	}
}