#include "JobStatisticsHandler.h"
#include "JobStatisticsRequest.h"
#include "JobStatisticsResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	JobStatisticsHandler::JobStatisticsHandler()
	{

	}

	JobStatisticsHandler::~JobStatisticsHandler()
	{

	}

	const char*	JobStatisticsHandler::GetName()
	{
		return "JobStatistics";
	}

	const char*	JobStatisticsHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	JobStatisticsHandler::ParseRequest(rude::CGI& cgi)
	{
		JobStatisticsRequest* pRequest = new JobStatisticsRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* JobStatisticsHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	JobStatisticsHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* JobStatisticsHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//JobStatisticsRequest* pRequest = static_cast<JobStatisticsRequest*>(pWebRequest);
		//JobStatisticsResponse* pWebResponse = new JobStatisticsResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->JobStatisticss();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* JobStatisticsHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		JobStatisticsRequest* pRequest = static_cast<JobStatisticsRequest*>(pWebRequest);
		GLogger* pLogger = augeGetLoggerInstance();
		
		const char* field = pRequest->GetField();
		if(field==NULL)
		{
			const char* msg = "Parameter [Field] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return pExpResponse;
		}

		const char* start_time	= pRequest->GetStartTime();
		const char* end_time	= pRequest->GetEndTime();

		GStatistics* pStatistics = NULL;
		JobManager* pJobManager = augeGetJobManagerInstance();
		if(start_time!=NULL&&end_time!=NULL)
		{
			pStatistics = pJobManager->Statistics(field, start_time, end_time);
		}
		else
		{
			pStatistics = pJobManager->Statistics(field);
		}

		JobStatisticsResponse* pResponse = new JobStatisticsResponse(pRequest);
		pResponse->SetStatistics(pStatistics);

		return pResponse;
	}
}