#include "RemoveUserHandler.h"
#include "RemoveUserRequest.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	RemoveUserHandler::RemoveUserHandler()
	{

	}

	RemoveUserHandler::~RemoveUserHandler()
	{

	}

	const char*	RemoveUserHandler::GetName()
	{
		return "RemoveUser";
	}

	const char*	RemoveUserHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RemoveUserHandler::ParseRequest(rude::CGI& cgi)
	{
		RemoveUserRequest* pRequest = new RemoveUserRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RemoveUserHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveUserHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//RemoveUserRequest* pRequest = new RemoveUserRequest();
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

	WebResponse* RemoveUserHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		RemoveUserRequest* pRequest = static_cast<RemoveUserRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		UserManager* pUserManager = augeGetUserManagerInstance();
		RESULTCODE rc = pUserManager->RemoveUser(name);

		WebResponse* pWebResponse = NULL;
		if(rc!=AG_SUCCESS)
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
		}

		return pWebResponse;
	}

	WebResponse* RemoveUserHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}