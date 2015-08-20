#include "DemAspectHandler.h"
#include "DemAspectRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"

namespace auge
{
	DemAspectHandler::DemAspectHandler()
	{

	}

	DemAspectHandler::~DemAspectHandler()
	{

	}

	const char*	DemAspectHandler::GetName()
	{
		return "DemAspect";
	}

	const char*	DemAspectHandler::GetDescription()
	{
		return "ÆÂÏò";
	}

	WebRequest*	DemAspectHandler::ParseRequest(rude::CGI& cgi)
	{
		DemAspectRequest* pRequest = new DemAspectRequest();
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

	WebRequest*	DemAspectHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DemAspectHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		DemAspectRequest* pRequest = new DemAspectRequest();
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

	WebResponse* DemAspectHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		DemAspectRequest* pRequest = static_cast<DemAspectRequest*>(pWebRequest);

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

	WebResponse* DemAspectHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		DemAspectRequest* pRequest = static_cast<DemAspectRequest*>(pWebRequest);

		const char* input_source_name = pRequest->GetInputDataSource();
		const char* input_raster_name	  = pRequest->GetInputRaster();
		const char* output_source_name= pRequest->GetOutputDataSource();
		const char* output_raster_name  = pRequest->GetOutputRaster();

		auge::DemAspectProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateDemAspectProcessor();

		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputRaster(input_raster_name);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputRaster(output_raster_name);

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
