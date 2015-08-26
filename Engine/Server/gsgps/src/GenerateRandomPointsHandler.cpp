#include "GenerateRandomPointsHandler.h"
#include "GenerateRandomPointsRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	GenerateRandomPointsHandler::GenerateRandomPointsHandler()
	{

	}

	GenerateRandomPointsHandler::~GenerateRandomPointsHandler()
	{

	}

	const char*	GenerateRandomPointsHandler::GetName()
	{
		return "GenerateRandomPoints";
	}

	const char*	GenerateRandomPointsHandler::GetDescription()
	{
		return "随机点生成";
	}

	WebRequest*	GenerateRandomPointsHandler::ParseRequest(rude::CGI& cgi)
	{
		GenerateRandomPointsRequest* pRequest = new GenerateRandomPointsRequest();
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

	WebRequest*	GenerateRandomPointsHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GenerateRandomPointsHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GenerateRandomPointsRequest* pRequest = new GenerateRandomPointsRequest();
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

	WebResponse* GenerateRandomPointsHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GenerateRandomPointsRequest* pRequest = static_cast<GenerateRandomPointsRequest*>(pWebRequest);

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

	WebResponse* GenerateRandomPointsHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GenerateRandomPointsRequest* pRequest = static_cast<GenerateRandomPointsRequest*>(pWebRequest);

		const char* output_source_name= pRequest->GetOutputSourceName();
		const char* output_type_name  = pRequest->GetOutputTypeName();
		GEnvelope& extent = pRequest->GetExtent();
		g_uint srid = pRequest->GetSRID();
		g_uint count= pRequest->GetCount();
		
		auge::RandomPointsGenerator* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateRandomPointsGenerator();

		pProcessor->SetUser(pUser->GetID());

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputFeatureClass(output_type_name);
		pProcessor->SetCount(count);
		pProcessor->SetSRID(srid);
		pProcessor->SetExtent(extent);

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
