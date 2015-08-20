#include "ClusterKmeanHandler.h"
#include "ClusterKmeanRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	KMeanHandler::KMeanHandler()
	{

	}

	KMeanHandler::~KMeanHandler()
	{

	}

	const char*	KMeanHandler::GetName()
	{
		return "KMean";
	}

	const char*	KMeanHandler::GetDescription()
	{
		return "kmeans¾ÛÀà";
	}

	WebRequest*	KMeanHandler::ParseRequest(rude::CGI& cgi)
	{
		KMeanRequest* pRequest = new KMeanRequest();
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

	WebRequest*	KMeanHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	KMeanHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		KMeanRequest* pRequest = new KMeanRequest();
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

	WebResponse* KMeanHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		KMeanRequest* pRequest = static_cast<KMeanRequest*>(pWebRequest);

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

	WebResponse* KMeanHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		KMeanRequest* pRequest = static_cast<KMeanRequest*>(pWebRequest);

		const char* input_source_name = pRequest->GetInputDataSource();
		const char* input_class_name	  = pRequest->GetInputFeatureClass();
		const char* output_source_name= pRequest->GetOutputDataSource();
		const char* output_class_name  = pRequest->GetOutputFeatureClass();
		g_int clusters = pRequest->GetClusters();
		
		if(clusters<=0)
		{
			const char* msg = "Cluster Count should be greater than 0";
			augeGetLoggerInstance()->Error(msg,__FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		auge::KMeansProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateKMeansProcessor();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetK(clusters);
		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputFeatureClass(input_class_name);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputFeatureClass(output_class_name);

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
}
