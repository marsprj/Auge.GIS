#include "DescribeTaskHandler.h"
#include "DescribeTaskRequest.h"
#include "DescribeTaskResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

#include "CsTaskManager.h"
#include "EnumTask.h"
#include "CsTask.h"

namespace auge
{
	DescribeTaskHandler::DescribeTaskHandler()
	{

	}

	DescribeTaskHandler::~DescribeTaskHandler()
	{

	}

	const char*	DescribeTaskHandler::GetName()
	{
		return "DescribeTask";
	}

	const char*	DescribeTaskHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	DescribeTaskHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeTaskRequest* pRequest = new DescribeTaskRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* DescribeTaskHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeTaskHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeTaskHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//DescribeTaskRequest* pRequest = static_cast<DescribeTaskRequest*>(pWebRequest);
		//DescribeTaskResponse* pWebResponse = new DescribeTaskResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->DescribeTasks();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* DescribeTaskHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		DescribeTaskRequest* pRequest = static_cast<DescribeTaskRequest*>(pWebRequest);		
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		TaskManager*  pTaskManager  = augeGetTaskManagerInstance();

		const char* name = pRequest->GetName();
		g_int role = pRequest->GetRole();
		if(name==NULL)
		{
			EnumTask* pTasks = NULL;
			if(pUser->IsPublic())
			{
				pTasks = pTaskManager->GetTask();
			}
			else
			{
				pTasks = pTaskManager->GetTask(pUser->GetID(), role);
			}

			DescribeTaskResponse* pResponse = new DescribeTaskResponse(pRequest);
			pResponse->SetTasks(pTasks);
			pResponse->SetWebContext(pWebContext);
			pWebResponse = pResponse;
		}
		else
		{
			CsTask* pTask = pTaskManager->GetTask(pUser->GetID(), name);

			//Map* pMap = pCartoManager->LoadMap(pUser->GetID(), name);
			if(pTask==NULL)
			{
				GError* pError = augeGetErrorInstance();
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(pError->GetLastError());
				pWebResponse = pExpResponse;
			}
			else
			{
				DescribeTaskResponse* pResponse = new DescribeTaskResponse(pRequest);
				pResponse->SetTask(pTask);
				pResponse->SetWebContext(pWebContext);
				pWebResponse = pResponse;;
			}
		}
		
		return pWebResponse;
	}
}