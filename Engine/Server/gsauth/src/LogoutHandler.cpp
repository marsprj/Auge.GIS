#include "LogoutHandler.h"
#include "LogoutRequest.h"
//#include "LogoutResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	LogoutHandler::LogoutHandler()
	{

	}

	LogoutHandler::~LogoutHandler()
	{

	}

	const char*	LogoutHandler::GetName()
	{
		return "Logout";
	}

	const char*	LogoutHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	LogoutHandler::ParseRequest(rude::CGI& cgi)
	{
		LogoutRequest* pRequest = new LogoutRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	LogoutHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	LogoutHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//LogoutRequest* pRequest = new LogoutRequest();
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

	WebResponse* LogoutHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	

		GLogger* pLogger = augeGetLoggerInstance();
		LogoutRequest* pRequest = static_cast<LogoutRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		if(name==NULL)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		UserManager* pUserManager = augeGetUserManagerInstance();
		RESULTCODE rc = pUserManager->Logout(name);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* LogoutHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}