#include "FeatureProjectHandler.h"
#include "FeatureProjectRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	FeatureProjectHandler::FeatureProjectHandler()
	{
		m_pJob = NULL;
	}

	FeatureProjectHandler::~FeatureProjectHandler()
	{
		if(m_pJob!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJob);
		}
	}

	const char*	FeatureProjectHandler::GetName()
	{
		return "FeatureProject";
	}

	const char*	FeatureProjectHandler::GetDescription()
	{
		return "矢量数据投影";
	}

	WebRequest*	FeatureProjectHandler::ParseRequest(rude::CGI& cgi)
	{
		FeatureProjectRequest* pRequest = new FeatureProjectRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->Info();
		return pRequest;
	}

	WebRequest*	FeatureProjectHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	FeatureProjectHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		FeatureProjectRequest* pRequest = new FeatureProjectRequest();
		//if(!pRequest->Create(pxDoc, pMap))
		if(!pRequest->Create(pxDoc))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		//if((mapName!=NULL)&&(strlen(mapName)>0))
		//{
		//	pRequest->SetMapName(mapName);
		//}
		pRequest->Info();
		return pRequest;
	}

	WebResponse* FeatureProjectHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		FeatureProjectRequest* pRequest = static_cast<FeatureProjectRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{

		}
		else if(strcmp(version,"1.3.0")==0)
		{

		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WPS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* FeatureProjectHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		FeatureProjectRequest* pRequest = static_cast<FeatureProjectRequest*>(pWebRequest);

		const char* input_source_name = pRequest->GetInputSourceName();
		const char* input_type_name	  = pRequest->GetInputTypeName();
		const char* output_source_name= pRequest->GetOutputSourceName();
		const char* output_type_name  = pRequest->GetOutputTypeName();
		g_int output_srid = pRequest->GetOutputSrid();
		
		if(output_srid < 0)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Invalie output sird [%d]", output_srid);
			augeGetLoggerInstance()->Error(msg,__FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		auge::FeatureProjectProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateFeatureProjectProcessor();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputFeatureClass(input_type_name);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputFeatureClass(output_type_name);
		pProcessor->SetOutputSRID(output_srid);

		RESULTCODE rc = pProcessor->Execute();
		pProcessor->Release();

		if(rc != AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			augeGetLoggerInstance()->Error(pError->GetLastError(),__FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());

		return pSusResponse;
	}

	void FeatureProjectHandler::Begin(User* pUser)
	{
		JobManager* pJobmanager = augeGetJobManagerInstance();
		WebContext* pWebContext = augeGetWebContextInstance();
		
		if(m_pJob!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJob);
		}
		const char* client = "";
		const char* server = pWebContext->GetServer();
		const char* operation= GetName();
		const char* params = "";
		m_pJob = pJobmanager->AddJob(pUser->GetID(), operation, params, client, server);
	}

	void FeatureProjectHandler::End()
	{
		JobManager* pJobmanager = augeGetJobManagerInstance();
		if(m_pJob!=NULL)
		{
			pJobmanager->SetEndTime(m_pJob->GetUUID());
		}
	}
}
