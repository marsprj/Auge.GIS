#include "DelaunayHandler.h"
#include "DelaunayRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	DelaunayHandler::DelaunayHandler()
	{
		m_pJob = NULL;
	}

	DelaunayHandler::~DelaunayHandler()
	{
		if(m_pJob!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJob);
		}
	}

	const char*	DelaunayHandler::GetName()
	{
		return "Delaunay";
	}

	const char*	DelaunayHandler::GetDescription()
	{
		return "DelaunayÈý½ÇÍø";
	}

	WebRequest*	DelaunayHandler::ParseRequest(rude::CGI& cgi)
	{
		DelaunayRequest* pRequest = new DelaunayRequest();
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

	WebRequest*	DelaunayHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DelaunayHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		DelaunayRequest* pRequest = new DelaunayRequest();
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

	WebResponse* DelaunayHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		DelaunayRequest* pRequest = static_cast<DelaunayRequest*>(pWebRequest);

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

	WebResponse* DelaunayHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		DelaunayRequest* pRequest = static_cast<DelaunayRequest*>(pWebRequest);

		const char* input_source_name = pRequest->GetInputSourceName();
		const char* input_type_name	  = pRequest->GetInputTypeName();
		const char* input_z_field= pRequest->GetInputTypeName();
		const char* output_source_name= pRequest->GetOutputSourceName();
		const char* output_type_name  = pRequest->GetOutputTypeName();		

		auge::DelaunayProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateDelaunayProcessor();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputFeatureClass(input_type_name);
		pProcessor->SetInputZField(input_z_field);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputFeatureClass(output_type_name);

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

		End();

		return pSusResponse;
	}

	void DelaunayHandler::Begin(User* pUser)
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
		const char* service = "gps";
		const char* params = "";
		m_pJob = pJobmanager->AddJob(pUser->GetID(), service, operation, params, client, server);
	}

	void DelaunayHandler::End()
	{
		JobManager* pJobmanager = augeGetJobManagerInstance();
		if(m_pJob!=NULL)
		{
			pJobmanager->SetEndTime(m_pJob->GetUUID());
		}
	}
}
