#include "CreateTaskHandler.h"
#include "CreateTaskRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

#include "CsTaskManager.h"

namespace auge
{
	CreateTaskHandler::CreateTaskHandler()
	{

	}

	CreateTaskHandler::~CreateTaskHandler()
	{

	}

	const char*	CreateTaskHandler::GetName()
	{
		return "CreateTask";
	}

	const char*	CreateTaskHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	CreateTaskHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateTaskRequest* pRequest = new CreateTaskRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* CreateTaskHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateTaskHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* CreateTaskHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//CreateTaskRequest* pRequest = static_cast<CreateTaskRequest*>(pWebRequest);
		//CreateTaskResponse* pWebResponse = new CreateTaskResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->CreateTasks();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* CreateTaskHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		CreateTaskRequest* pRequest = static_cast<CreateTaskRequest*>(pWebRequest);		
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		GError* pError = augeGetErrorInstance();

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			const char* msg = "[Task Name] is null";
			pLogger->Error(msg, __FILE__, __LINE__);
			pError->SetError(msg);

			WebExceptionResponse* pResponse = augeCreateWebExceptionResponse();
			pResponse->SetMessage(msg);
			return pResponse;
		}

		const char* mapName = pRequest->GetMapName();
		if(mapName==NULL)
		{
			const char* msg = "[Map Name] is null";
			pLogger->Error(msg, __FILE__, __LINE__);
			pError->SetError(msg);

			WebExceptionResponse* pResponse = augeCreateWebExceptionResponse();
			pResponse->SetMessage(msg);
			return pResponse;
		}

		const char* description = pRequest->GetDescription();
		if(description==NULL)
		{
			description = "";
		}

		g_int mapID = pCartoManager->GetMapID(pUser->GetID(), mapName);
		if(mapID<0)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot find map [%s].", mapName);

			pLogger->Error(msg, __FILE__, __LINE__);
			pError->SetError(msg);

			WebExceptionResponse* pResponse = augeCreateWebExceptionResponse();
			pResponse->SetMessage(msg);
			return pResponse;
		}

		TaskManager* pManager = augeGetTaskManagerInstance();
		g_int tid = pManager->CreateTask(name, description, mapID, pUser->GetID());
		if(tid<0)
		{
			const char* msg = pError->GetLastError();
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pResponse = augeCreateWebExceptionResponse();
			pResponse->SetMessage(msg);
			return pResponse;
		}

		pManager->Join(tid, pUser->GetID(), true);

		WebSuccessResponse* pWebResponse = augeCreateWebSuccessResponse();
		pWebResponse->SetRequest(pRequest->GetRequest());
		
		return pWebResponse;
	}
}