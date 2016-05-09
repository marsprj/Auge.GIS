#include "GetJobHandler.h"
#include "GetJobRequest.h"
#include "GetJobResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetJobHandler::GetJobHandler()
	{

	}

	GetJobHandler::~GetJobHandler()
	{

	}

	const char*	GetJobHandler::GetName()
	{
		return "GetJob";
	}

	const char*	GetJobHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetJobHandler::ParseRequest(rude::CGI& cgi)
	{
		GetJobRequest* pRequest = new GetJobRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* GetJobHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetJobHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetJobHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//GetJobRequest* pRequest = static_cast<GetJobRequest*>(pWebRequest);
		//GetJobResponse* pWebResponse = new GetJobResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->GetJobs();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* GetJobHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GetJobRequest* pRequest = static_cast<GetJobRequest*>(pWebRequest);		
		
		augeProcssState state = pRequest->GetState();
		g_uint maxJobs = pRequest->GetMaxJobs();
		g_uint offset = pRequest->GetOffset();

		JobManager* pJobManager = augeGetJobManagerInstance();
		const char* uuid = pRequest->GetUUID();
		if(uuid==NULL)
		{
			EnumJob* pEnumJobs = pJobManager->GetJob(pUser, state, maxJobs, offset);
			GetJobResponse* pResponse = new GetJobResponse(pRequest);
			pResponse->SetJobs(pEnumJobs);
			pResponse->SetWebContext(pWebContext);
			pWebResponse = pResponse;
		}
		else
		{
			Job* pJob = pJobManager->GetJob(uuid);
			if(pJob==NULL)
			{
				GError* pError = augeGetErrorInstance();
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(pError->GetLastError());
				pWebResponse = pExpResponse;
			}
			else
			{
				GetJobResponse* pResponse = new GetJobResponse(pRequest);
				pResponse->SetJob(pJob);
				pResponse->SetWebContext(pWebContext);
				pWebResponse = pResponse;;
			}
		}
				
		return pWebResponse;
	}
}