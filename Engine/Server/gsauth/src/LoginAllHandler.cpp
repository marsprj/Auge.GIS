#include "LoginAllHandler.h"
#include "LoginAllRequest.h"
//#include "LoginAllResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	LoginAllHandler::LoginAllHandler()
	{

	}

	LoginAllHandler::~LoginAllHandler()
	{

	}

	const char*	LoginAllHandler::GetName()
	{
		return "LoginAll";
	}

	const char*	LoginAllHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	LoginAllHandler::ParseRequest(rude::CGI& cgi)
	{
		LoginAllRequest* pRequest = new LoginAllRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	LoginAllHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	LoginAllHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//LoginAllRequest* pRequest = new LoginAllRequest();
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

	WebResponse* LoginAllHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GLogger* pLogger = augeGetLoggerInstance();
		LoginAllRequest* pRequest = static_cast<LoginAllRequest*>(pWebRequest);
		UserManager* pUserManager = augeGetUserManagerInstance();
		RESULTCODE rc = pUserManager->Login();
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* LoginAllHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		LoginAllRequest* pRequest = static_cast<LoginAllRequest*>(pWebRequest);
		UserManager* pUserManager = augeGetUserManagerInstance();
		RESULTCODE rc = pUserManager->Login();
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return pExpResponse;
		}

		char user_root[AUGE_PATH_MAX];
		memset(user_root, 0, AUGE_PATH_MAX);
		auge_make_path(user_root, NULL, pWebContext->GetUserRoot(), name, NULL);
		if(g_access(user_root, 4))
		{
			auge_mkdir(user_root);
		}

		char user_file_root[AUGE_PATH_MAX];
		memset(user_file_root, 0, AUGE_PATH_MAX);
		auge_make_path(user_file_root, NULL, user_root, "upload", NULL);
		if(g_access(user_file_root, 4))
		{
			auge_mkdir(user_file_root);
		}

		char user_raster_root[AUGE_PATH_MAX];
		memset(user_raster_root, 0, AUGE_PATH_MAX);
		auge_make_path(user_raster_root, NULL, user_root, "rds", NULL);
		if(g_access(user_raster_root, 4))
		{
			auge_mkdir(user_raster_root);
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}