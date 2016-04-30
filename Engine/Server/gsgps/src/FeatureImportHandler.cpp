#include "FeatureImportHandler.h"
#include "FeatureImportRequest.h"
#include "GProcessEngine.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

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

	const char*	FeatureImportHandler::GetDescription()
	{
		return "ʸ�����ݵ���";
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

	WebResponse* FeatureImportHandler::Execute(WebRequest* pWebRequest, User* pUser)
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

	/// <summary>
	/// Executes the specified p web request.
	/// </summary>
	/// <param name="pWebRequest">The p web request.</param>
	/// <param name="pWebContext">The p web context.</param>
	/// <param name="pUser">The p user.</param>
	/// <returns></returns>
	WebResponse* FeatureImportHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		RESULTCODE rc = AG_SUCCESS;

		WebResponse* pWebResponse = NULL;
		FeatureImportRequest* pRequest = static_cast<FeatureImportRequest*>(pWebRequest);

		const char* raw_shp_name = pRequest->GetShapeName();
		const char* source_name = pRequest->GetSourceName();
		//const char* raw_type_name = pRequest->GetTypeName();
		const char* type_name = pRequest->GetTypeName();
		
		if(raw_shp_name==NULL)
		{
			const char* msg = "Parameter [shpname] is NULL";
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pWebResponse =  pExpResponse;
		}
		if(type_name==NULL)
		{
			const char* msg = "Parameter [typename] is NULL";
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pWebResponse =  pExpResponse;
		}

		//char type_name[AUGE_NAME_MAX];
		//memset(type_name, 0, AUGE_NAME_MAX);
		//
		//if(pWebContext->IsIE())
		//{
		//	strcpy(type_name, raw_type_name);
		//}
		//else
		//{
		//	size_t type_name_size = AUGE_NAME_MAX;
		//	auge_encoding_convert_2(AUGE_ENCODING_UTF8, AUGE_ENCODING_GBK, raw_type_name, strlen(raw_type_name), type_name, &type_name_size);
		//}

		char shp_name[AUGE_NAME_MAX];
		memset(shp_name, 0, AUGE_NAME_MAX);
#ifdef WIN32
		strcpy(shp_name, raw_shp_name);
#else
		size_t shp_name_size = AUGE_NAME_MAX;
		auge_encoding_convert_2(AUGE_ENCODING_GBK, AUGE_ENCODING_UTF8, raw_shp_name, strlen(raw_shp_name), shp_name, &shp_name_size);
#endif
		pLogger->Debug(shp_name, __FILE__, __LINE__);

		//char user_root[AUGE_PATH_MAX];
		//memset(user_root, 0, AUGE_PATH_MAX);
		//auge_make_path(user_root, NULL, pWebContext->GetUserRoot(), pUser->GetName(), NULL);

		//char user_file_root[AUGE_PATH_MAX];
		//memset(user_file_root, 0, AUGE_PATH_MAX);
		//auge_make_path(user_file_root, NULL, user_root, "upload", NULL);

		char user_file_root[AUGE_PATH_MAX];
		auge_make_user_file_root(user_file_root, AUGE_PATH_MAX, pWebContext->GetUserRoot(), pUser->GetName());

		char shp_path[AUGE_PATH_MAX];
		memset(shp_path, 0, AUGE_PATH_MAX);
		if(pRequest->GetShapePath()!=NULL)
		{
			char req_path[AUGE_PATH_MAX];
			memset(req_path, 0, AUGE_PATH_MAX);
			const char* raw_req_path = pRequest->GetShapePath();
#ifdef WIN32
			strcpy(req_path, raw_req_path);
#else
			size_t req_path_size = AUGE_NAME_MAX;
			auge_encoding_convert_2(AUGE_ENCODING_GBK, AUGE_ENCODING_UTF8, raw_req_path, req_path, &req_path_size);
#endif
			auge_make_path(shp_path, NULL, user_file_root, req_path+1, NULL);
		}
		else
		{
			strcpy(shp_path, user_file_root);
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
			pWebResponse = ImportZipedShapeFile(shp_path, shp_name, source_name, type_name, pUser);
#endif
		}
		else
		{
			pWebResponse = ImportShapeFile(shp_path, shp_name, source_name, type_name, pUser);
		}

		End();

		return pWebResponse;
	}

	WebResponse* FeatureImportHandler::ImportZipedShapeFile(const char* zip_path, const char* zip_name, const char* source_name, const char* type_name, User* pUser)
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
			auge_split_path(zip_name,NULL, NULL, shp_name, NULL);
			//auge_make_path(shp_path, NULL, zip_path, shp_name, NULL);
			//pWebResponse = ImportShapeFile(shp_path, shp_name, source_name, type_name, pUser);
			auge_get_cwd(shp_path,AUGE_PATH_MAX);
			pWebResponse = ImportShapeFile(shp_path, shp_name, source_name, type_name, pUser);
		}

		return pWebResponse;
	}

	/// <summary>
	/// Imports the shape file.
	/// </summary>
	/// <param name="shp_path">The shp_path.</param>
	/// <param name="shp_name">The shp_name.</param>
	/// <param name="source_name">The source_name.</param>
	/// <param name="type_name">The type_name.</param>
	/// <returns></returns>
	WebResponse* FeatureImportHandler::ImportShapeFile(const char* shp_path, const char* shp_name, const char* source_name, const char* type_name, User* pUser)
	{
		WebResponse* pWebResponse = NULL;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GProcessorFactory* factory = augeGetGeoProcessorFactoryInstance();
		FeatureImportProcessor* processor = factory->CreateFeatureImportProcessor();

		processor->SetUser(pUser->GetID());
		processor->SetShapePath(shp_path);
		processor->SetShapeName(shp_name);
		processor->SetDataSourceName(source_name);
		processor->SetFeatureClassName(type_name);

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
