#include "FeatureImportHandler.h"
#include "FeatureImportRequest.h"
#include "GProcessEngine.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"

namespace auge
{
	FeatureImportHandler::FeatureImportHandler()
	{

	}

	FeatureImportHandler::~FeatureImportHandler()
	{

	}

	const char*	FeatureImportHandler::GetName()
	{
		return "FeatureImport";
	}

	WebRequest*	FeatureImportHandler::ParseRequest(rude::CGI& cgi)
	{
		FeatureImportRequest* pRequest = new FeatureImportRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		//pRequest->Info();
		return pRequest;
	}

	WebRequest*	FeatureImportHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	FeatureImportHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		FeatureImportRequest* pRequest = new FeatureImportRequest();
		return pRequest;
	}

	WebResponse* FeatureImportHandler::Execute(WebRequest* pWebRequest)
	{
		WebResponse* pWebResponse = NULL;
		//FeaureImportRequest* pRequest = static_cast<FeaureImportRequest*>(pWebRequest);

		//const char* version = pRequest->GetVersion();
		//if(strcmp(version,"1.0.0")==0)
		//{

		//}
		//else if(strcmp(version,"1.3.0")==0)
		//{

		//}
		//else
		//{
		//	char msg[AUGE_MSG_MAX];
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	g_sprintf(msg, "WPS does not support %s",version);
		//	pExpResponse->SetMessage(msg);
		//	pWebResponse = pExpResponse;
		//}

		return pWebResponse;
	}

	WebResponse* FeatureImportHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		RESULTCODE rc = AG_SUCCESS;

		WebResponse* pWebResponse = NULL;
		FeatureImportRequest* pRequest = static_cast<FeatureImportRequest*>(pWebRequest);

		const char* shp_path = NULL;
		const char* shp_name = pRequest->GetShapeName();
		const char* source_name = pRequest->GetSourceName();
		const char* type_name = pRequest->GetTypeName();

		if(shp_name==NULL)
		{
			const char* msg = "Parameter [shpname] is NULL";
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pWebResponse =  pExpResponse;
		}


		if(pRequest->GetShapePath()!=NULL)
		{
			shp_path = pRequest->GetShapePath();
		}
		else
		{
			shp_path = pWebContext->GetUploadPath();
		}

		char shp_ext[AUGE_EXT_MAX];
		memset(shp_ext,0,AUGE_EXT_MAX);
		auge_split_path(shp_name,NULL,NULL,NULL,shp_ext);

		if(g_stricmp(shp_ext,".zip")==0)
		{
			// zip file
#ifdef WIN32
			const char* msg = "Zip file importing is not supported on Windows";
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pWebResponse =  pExpResponse;
#else
			pWebResponse = ImportZipedShapeFile(shp_path, shp_name, source_name, type_name);
#endif
		}
		else
		{
			pWebResponse = ImportShapeFile(shp_path, shp_name, source_name, type_name);
		}
		return pWebResponse;
	}

	WebResponse* FeatureImportHandler::ImportZipedShapeFile(const char* zip_path, const char* zip_name, const char* source_name, const char* type_name)
	{
		WebResponse* pWebResponse = NULL;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GProcessorFactory* factory = augeGetGeoProcessorFactoryInstance();
		ArchiveProcessor* archiver =  factory->CreateArchiveProcessor();

		char zip_file[AUGE_PATH_MAX];
		auge_make_path(zip_file, NULL, zip_path, zip_name,NULL);
		archiver->SetArchivePath(zip_file);
		RESULTCODE rc = archiver->Decompress();
		archiver->Release();

		if(rc!=AG_SUCCESS)
		{
			const char* msg = "Fail to decompress zip file";
			pError->SetError(msg);
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse =  pExpResponse;
		}
		else
		{
			char shp_name[AUGE_NAME_MAX];
			char shp_path[AUGE_PATH_MAX];
			strncpy(shp_name, zip_name, strlen(zip_file)-4);
			//auge_make_path(shp_path, NULL, zip_path, shp_name, NULL);
			//pWebResponse = ImportShapeFile(shp_path, shp_name, source_name, type_name);
			auge_get_cwd(shp_path,AUGE_PATH_MAX);
			pWebResponse = ImportShapeFile(shp_path, shp_name, source_name, type_name);
		}

		return pWebResponse;
	}

	WebResponse* FeatureImportHandler::ImportShapeFile(const char* shp_path, const char* shp_name, const char* source_name, const char* type_name)
	{
		WebResponse* pWebResponse = NULL;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GProcessorFactory* factory = augeGetGeoProcessorFactoryInstance();
		FeatureImportProcessor* processor = factory->CreateFeatureImportProcessor();

		processor->SetShapePath(shp_path);
		processor->SetShapeName(shp_name);
		processor->SetDataSourceName(source_name);
		processor->SetTypeName(type_name);

		RESULTCODE rc = processor->Execute();
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse =  pExpResponse;
		}
		else
		{
			WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
			pSusResponse->SetRequest("FeatureImport");
			pWebResponse = pSusResponse;
		}

		processor->Release();

		return pWebResponse;
	}
}
