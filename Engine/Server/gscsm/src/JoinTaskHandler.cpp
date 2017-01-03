#include "JoinTaskHandler.h"
#include "JoinTaskRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

#include "CsTaskManager.h"

namespace auge
{
	JoinTaskHandler::JoinTaskHandler()
	{

	}

	JoinTaskHandler::~JoinTaskHandler()
	{

	}

	const char*	JoinTaskHandler::GetName()
	{
		return "JoinTask";
	}

	const char*	JoinTaskHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	JoinTaskHandler::ParseRequest(rude::CGI& cgi)
	{
		JoinTaskRequest* pRequest = new JoinTaskRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* JoinTaskHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	JoinTaskHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* JoinTaskHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* JoinTaskHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		JoinTaskRequest* pRequest = static_cast<JoinTaskRequest*>(pWebRequest);		
		GLogger* pLogger = augeGetLoggerInstance();
		GError* pError = augeGetErrorInstance();

		TaskManager* pTaskManager = augeGetTaskManagerInstance();

		g_int tid = pRequest->GetTaskID();
		RESULTCODE rc = pTaskManager->Join(tid, pUser->GetID(), false);
		if(rc!=AG_SUCCESS)
		{
			const char* msg = pError->GetLastError();
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pResponse = augeCreateWebExceptionResponse();
			pResponse->SetMessage(msg);
			return pResponse;
		}

		WebSuccessResponse* pWebResponse = augeCreateWebSuccessResponse();
		pWebResponse->SetRequest(pRequest->GetRequest());
		
		return pWebResponse;
	}
}