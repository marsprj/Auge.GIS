#include "GetRasterHandler.h"
#include "GetRasterRequest.h"
#include "GetRasterResponse.h"

#include "AugeProcessor.h"
#include "AugeUser.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeRaster.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

namespace auge
{
	extern void rds_get_raster_repository(char* raster_repository, size_t size, const char* user_name, WebContext* pWebContext);

	GetRasterHandler::GetRasterHandler()
	{

	}

	GetRasterHandler::~GetRasterHandler()
	{

	}

	const char*	GetRasterHandler::GetName()
	{
		return "GetRaster";
	}

	WebRequest*	GetRasterHandler::ParseRequest(rude::CGI& cgi)
	{
		GetRasterRequest* pRequest = new GetRasterRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetRasterHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetRasterHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetRasterHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetRasterRequest* pRequest = static_cast<GetRasterRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* GetRasterHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		GetRasterRequest* pRequest = static_cast<GetRasterRequest*>(pWebRequest);

		const char* folder_path = pRequest->GetPath();
		const char* raster_name=pRequest->GetName();
		const char* source_name=pRequest->GetSourceName();
		GEnvelope extent = pRequest->GetExtent();

		if(raster_name==NULL)
		{
			const char* msg = "Parameter [rasterName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		if(source_name==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;

		}

		if(folder_path==NULL)
		{
			const char* msg = "Parameter [Path] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		WebResponse* pWebResponse = NULL;
		if( (extent.IsValid()))
		{
			RasterFormatConvertToJPEGProcessor* pProcessor = NULL;
			GProcessorFactory* pFactory = augeGetGeoProcessorFactoryInstance();
			pProcessor = pFactory->CreateRasterFormatConvertToJPEGProcessor();
			
			pProcessor->SetInputRectangle(extent);
			pProcessor->SetInputDataSource(source_name);
			pProcessor->SetInputRaster(raster_name);
			pProcessor->SetInputPath(folder_path);

			char uuid[AUGE_NAME_MAX];
			memset(uuid, 0, AUGE_NAME_MAX);
			auge_generate_uuid(uuid, AUGE_PATH_MAX);
			char output_path[AUGE_PATH_MAX];
			memset(output_path, 0, AUGE_PATH_MAX);
			auge_make_path(output_path, NULL, pWebContext->GetCacheMapPath(), uuid, "jpeg");
			pProcessor->SetOutputPath(output_path);

			pProcessor->SetRed(pRequest->GetR());
			pProcessor->SetGreen(pRequest->GetG());
			pProcessor->SetBlue(pRequest->GetB());

			RESULTCODE rc = pProcessor->Execute();
			pProcessor->Release();

			if(rc==AG_SUCCESS)
			{
				GetRasterResponse* pGetRasterResponse = new GetRasterResponse(pRequest);
				pGetRasterResponse->SetPath(output_path);
				pWebResponse = pGetRasterResponse;
			}
			else
			{
				GError* pError = augeGetErrorInstance();
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(pError->GetLastError());
				pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);
				pWebResponse = pExpResponse;
			}
		}
		else
		{
			Workspace* pWorkspace = NULL;
			RasterWorkspace* pRasterWorkspace = NULL;
			ConnectionManager* pConnectionManager = NULL;
			pConnectionManager = augeGetConnectionManagerInstance();
			pWorkspace = pConnectionManager->GetWorkspace(pUser->GetID(), source_name);
			if(pWorkspace==NULL)
			{
				char msg[AUGE_MSG_MAX];
				memset(msg,0,AUGE_MSG_MAX);
				g_sprintf(msg,"Cannot Get DataSource [%s]", source_name);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pLogger->Error(msg,__FILE__,__LINE__);

				return pExpResponse;
			}

			pRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);
			//if(pRasterWorkspace==NULL)
			//{
			//	char msg[AUGE_MSG_MAX];
			//	memset(msg,0,AUGE_MSG_MAX);
			//	g_sprintf(msg,"Cannot Get DataSource [%s]", sourceName);
			//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			//	pExpResponse->SetMessage(msg);
			//	pLogger->Error(msg,__FILE__,__LINE__);

			//	return pExpResponse;
			//}

			RasterFolder* pFolder = pRasterWorkspace->GetFolder(folder_path);
			if(pFolder==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Path [%s] does not exist.", folder_path);
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error(msg, __FILE__, __LINE__);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				return pExpResponse;
			}

			RESULTCODE rc = AG_FAILURE;
			RasterDataset* pRasterDataset = pFolder->GetRasterDataset();
			Raster* pRaster = pRasterDataset->GetRaster(raster_name);
			if(pRaster==NULL)
			{
				pFolder->Release();

				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Raster [%s/%s] does not exist.", folder_path, raster_name);
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error(msg, __FILE__, __LINE__);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				return pExpResponse;
			}

			const char* raster_path = pRaster->GetPath();

			//char local_path[AUGE_PATH_MAX];
			//memset(local_path,0,AUGE_PATH_MAX);
			//auge_make_path(local_path,NULL,raster_repository,folder_path+1,NULL);

			//char raster_path[AUGE_PATH_MAX];
			//memset(raster_path,0,AUGE_PATH_MAX);
			//auge_make_path(raster_path,NULL,local_path,raster_name,NULL);

			//if(g_access(raster_path,4))
			//{
			//	char msg[AUGE_MSG_MAX];
			//	memset(msg,0,AUGE_MSG_MAX);
			//	g_sprintf(msg,"Folder [%s] does not have Raster [%s] in folder  does not exist.", folder_path, raster_name);
			//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			//	pExpResponse->SetMessage(msg);
			//	pLogger->Error(msg,__FILE__,__LINE__);

			//	pWebResponse = pExpResponse;
			//}
			//else
			{
				GetRasterResponse* pGetRasterResponse = new GetRasterResponse(pRequest);
				pGetRasterResponse->SetPath(raster_path);
				pWebResponse = pGetRasterResponse;
			}
			pRaster->Release();
			pFolder->Release();
		}

		return pWebResponse;
	}

	//WebResponse* GetRasterHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	//{
	//	GLogger* pLogger = augeGetLoggerInstance();
	//	GetRasterRequest* pRequest = static_cast<GetRasterRequest*>(pWebRequest);

	//	const char* root_path = pWebContext->GetUploadPath();
	//	const char* folder_path = pRequest->GetPath();
	//	const char* raster_name=pRequest->GetName();
	//	GEnvelope extent = pRequest->GetExtent();

	//	if(raster_name==NULL)
	//	{
	//		const char* msg = "Parameter [Name] is NULL";
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		pLogger->Error(msg,__FILE__,__LINE__);

	//		return pExpResponse;
	//	}

	//	if(folder_path==NULL)
	//	{
	//		const char* msg = "Parameter [Path] is NULL";
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		pLogger->Error(msg,__FILE__,__LINE__);

	//		return pExpResponse;
	//	}

	//	char local_path[AUGE_PATH_MAX];
	//	memset(local_path,0,AUGE_PATH_MAX);
	//	auge_make_path(local_path,NULL,root_path,folder_path+1,NULL);

	//	char raster_path[AUGE_PATH_MAX];
	//	memset(raster_path,0,AUGE_PATH_MAX);
	//	auge_make_path(raster_path,NULL,local_path,raster_name,NULL);

	//	if(g_access(raster_path,4))
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		memset(msg,0,AUGE_MSG_MAX);
	//		g_sprintf(msg,"Folder [%s] does not have Raster [%s] in folder  does not exist.", folder_path, raster_name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		pLogger->Error(msg,__FILE__,__LINE__);

	//		return pExpResponse;
	//	}

	//	GetRasterResponse* pGetRasterResponse = new GetRasterResponse(pRequest);
	//	pGetRasterResponse->SetPath(raster_path);

	//	return pGetRasterResponse;
	//}
}