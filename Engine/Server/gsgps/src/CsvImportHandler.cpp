#include "CsvImportHandler.h"
#include "CsvImportRequest.h"
#include "GProcessEngine.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	CsvImportHandler::CsvImportHandler()
	{

	}

	CsvImportHandler::~CsvImportHandler()
	{

	}

	const char*	CsvImportHandler::GetName()
	{
		return "CsvImport";
	}

	const char*	CsvImportHandler::GetDescription()
	{
		return "CSV数据导入";
	}

	WebRequest*	CsvImportHandler::ParseRequest(rude::CGI& cgi)
	{
		CsvImportRequest* pRequest = new CsvImportRequest();
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

	WebRequest*	CsvImportHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CsvImportHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		CsvImportRequest* pRequest = new CsvImportRequest();
		return pRequest;
	}

	WebResponse* CsvImportHandler::Execute(WebRequest* pWebRequest, User* pUser)
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
	WebResponse* CsvImportHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();
		RESULTCODE rc = AG_SUCCESS;

		WebResponse* pWebResponse = NULL;
		CsvImportRequest* pRequest = static_cast<CsvImportRequest*>(pWebRequest);

		const char* csv_name = pRequest->GetCsvName();
		const char* source_name = pRequest->GetSourceName();
		const char* type_name = pRequest->GetTypeName();
		const char* user_root = pWebContext->GetUserRoot();

		if(csv_name==NULL)
		{
			const char* msg = "Parameter [csvname] is NULL";
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pWebResponse =  pExpResponse;
		}

		char csv_path[AUGE_PATH_MAX];
		memset(csv_path, 0, AUGE_PATH_MAX);
		if(pRequest->GetCsvPath()!=NULL)
		{
			const char* req_path = pRequest->GetCsvPath();
			auge_make_path(csv_path, NULL, pWebContext->GetUploadPath(), req_path+1, NULL);
		}
		else
		{
			strcpy(csv_path, pWebContext->GetUploadPath());
		}

		char csv_ext[AUGE_EXT_MAX];
		memset(csv_ext,0,AUGE_EXT_MAX);
		auge_split_path(csv_name,NULL,NULL,NULL,csv_ext);

		pWebResponse = ImportCsvFile(csv_path, csv_name, source_name, type_name,pUser->GetID());

		End();

		return pWebResponse;
	}

	/// <summary>
	/// Imports the shape file.
	/// </summary>
	/// <param name="csv_path">The csv_path.</param>
	/// <param name="csv_name">The csv_name.</param>
	/// <param name="source_name">The source_name.</param>
	/// <param name="type_name">The type_name.</param>
	/// <returns></returns>
	WebResponse* CsvImportHandler::ImportCsvFile(const char* csv_path, const char* csv_name, const char* source_name, const char* type_name, g_int user_id)
	{
		WebResponse* pWebResponse = NULL;

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GProcessorFactory* factory = augeGetGeoProcessorFactoryInstance();
		CsvImportProcessor* processor = factory->CreateCsvImportProcessor();

		char local_csv_path[AUGE_PATH_MAX];
		memset(local_csv_path, 0, AUGE_PATH_MAX);
		auge_make_path(local_csv_path, NULL, csv_path, csv_name, NULL);

		processor->SetUser(user_id);
		processor->SetCsvPath(local_csv_path);
		processor->SetDataSource(source_name);
		processor->SetDatasetName(type_name);

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
			pSusResponse->SetRequest("CsvImport");
			pWebResponse = pSusResponse;
		}

		processor->Release();

		return pWebResponse;
	}
}
