#include "GenerateRandomPointsInPolygonHandler.h"
#include "GenerateRandomPointsInPolygonRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	GenerateRandomPointsInPolygonHandler::GenerateRandomPointsInPolygonHandler()
	{

	}

	GenerateRandomPointsInPolygonHandler::~GenerateRandomPointsInPolygonHandler()
	{

	}

	const char*	GenerateRandomPointsInPolygonHandler::GetName()
	{
		return "GenerateRandomPointsInPolygon";
	}

	const char*	GenerateRandomPointsInPolygonHandler::GetDescription()
	{
		return "多边形内随机点";
	}

	WebRequest*	GenerateRandomPointsInPolygonHandler::ParseRequest(rude::CGI& cgi)
	{
		GenerateRandomPointsInPolygonRequest* pRequest = new GenerateRandomPointsInPolygonRequest();
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

	WebRequest*	GenerateRandomPointsInPolygonHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GenerateRandomPointsInPolygonHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GenerateRandomPointsInPolygonRequest* pRequest = new GenerateRandomPointsInPolygonRequest();
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

	WebResponse* GenerateRandomPointsInPolygonHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GenerateRandomPointsInPolygonRequest* pRequest = static_cast<GenerateRandomPointsInPolygonRequest*>(pWebRequest);

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

	WebResponse* GenerateRandomPointsInPolygonHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		GenerateRandomPointsInPolygonRequest* pRequest = static_cast<GenerateRandomPointsInPolygonRequest*>(pWebRequest);

		const char* input_source_name = pRequest->GetInputSourceName();
		const char* input_type_name	  = pRequest->GetInputTypeName();
		const char* output_source_name= pRequest->GetOutputSourceName();
		const char* output_type_name  = pRequest->GetOutputTypeName();
		g_int count = pRequest->GetCount();

		auge::RandomPointsInPolygonGenerator * pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateRandomPointsInPolygonGenerator();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputFeatureClass(input_type_name);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputFeatureClass(output_type_name);

		pProcessor->SetCount(count);

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
}
