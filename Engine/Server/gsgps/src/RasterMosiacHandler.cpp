#include "RasterMosiacHandler.h"
#include "RasterMosiacRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	RasterMosiacHandler::RasterMosiacHandler()
	{

	}

	RasterMosiacHandler::~RasterMosiacHandler()
	{

	}

	const char*	RasterMosiacHandler::GetName()
	{
		return "RasterMosiac";
	}

	const char*	RasterMosiacHandler::GetDescription()
	{
		return "Õ¤¸ñÏâÇ¶";
	}

	WebRequest*	RasterMosiacHandler::ParseRequest(rude::CGI& cgi)
	{
		RasterMosiacRequest* pRequest = new RasterMosiacRequest();
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

	WebRequest*	RasterMosiacHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RasterMosiacHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		RasterMosiacRequest* pRequest = new RasterMosiacRequest();
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

	WebResponse* RasterMosiacHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		RasterMosiacRequest* pRequest = static_cast<RasterMosiacRequest*>(pWebRequest);

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

	WebResponse* RasterMosiacHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		RasterMosiacRequest* pRequest = static_cast<RasterMosiacRequest*>(pWebRequest);

		const char* input_source_name_1 = pRequest->GetInputDataSource_1();
		const char* input_raster_name_1 = pRequest->GetInputRaster_1();
		const char* input_raster_path_1 = pRequest->GetInputPath_1();
		const char* input_source_name_2 = pRequest->GetInputDataSource_2();
		const char* input_raster_name_2 = pRequest->GetInputRaster_2();
		const char* input_raster_path_2 = pRequest->GetInputPath_2();
		const char* output_source_name  = pRequest->GetOutputDataSource();
		const char* output_raster_name  = pRequest->GetOutputRaster();
		const char* output_raster_path = pRequest->GetOutputPath();

		auge::RasterMosiacProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreateRasterMosiacProcessor();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetInputDataSource_1(input_source_name_1);
		pProcessor->SetInputRaster_1(input_raster_name_1);
		pProcessor->SetInputPath_1(input_raster_path_1);

		pProcessor->SetInputDataSource_2(input_source_name_2);
		pProcessor->SetInputRaster_2(input_raster_name_2);
		pProcessor->SetInputPath_2(input_raster_path_2);

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

		End();

		return pSusResponse;
	}
}
