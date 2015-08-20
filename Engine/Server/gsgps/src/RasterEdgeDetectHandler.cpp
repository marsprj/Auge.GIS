#include "RasterEdgeDetectHandler.h"
#include "RasterEdgeDetectRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	RasterEdgeDetectHandler::RasterEdgeDetectHandler()
	{

	}

	RasterEdgeDetectHandler::~RasterEdgeDetectHandler()
	{

	}

	const char*	RasterEdgeDetectHandler::GetName()
	{
		return "RasterEdgeDetect";
	}

	const char*	RasterEdgeDetectHandler::GetDescription()
	{
		return "±ßÔµÌáÈ¡";
	}

	WebRequest*	RasterEdgeDetectHandler::ParseRequest(rude::CGI& cgi)
	{
		RasterEdgeDetectRequest* pRequest = new RasterEdgeDetectRequest();
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

	WebRequest*	RasterEdgeDetectHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RasterEdgeDetectHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		RasterEdgeDetectRequest* pRequest = new RasterEdgeDetectRequest();
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

	WebResponse* RasterEdgeDetectHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		RasterEdgeDetectRequest* pRequest = static_cast<RasterEdgeDetectRequest*>(pWebRequest);

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

	WebResponse* RasterEdgeDetectHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		RasterEdgeDetectRequest* pRequest = static_cast<RasterEdgeDetectRequest*>(pWebRequest);

		const char* input_source_name  = pRequest->GetInputDataSource();
		const char* input_raster_name  = pRequest->GetInputRaster();
		const char* input_raster_path  = pRequest->GetInputPath();
		const char* output_source_name = pRequest->GetOutputDataSource();
		const char* output_raster_name = pRequest->GetOutputRaster();
		const char* output_raster_path = pRequest->GetOutputPath();

		auge::RasterEdgeDetectProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateRasterEdgeDetectProcessor();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputRaster(input_raster_name);
		pProcessor->SetInputPath(input_raster_path);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputRaster(output_raster_name);
		pProcessor->SetOutputPath(output_raster_path);

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
