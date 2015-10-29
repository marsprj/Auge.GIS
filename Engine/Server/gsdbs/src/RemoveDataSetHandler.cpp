#include "RemoveDataSetHandler.h"
#include "RemoveDataSetRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeUser.h"

namespace auge
{
	RemoveDataSetHandler::RemoveDataSetHandler()
	{

	}

	RemoveDataSetHandler::~RemoveDataSetHandler()
	{

	}

	const char*	RemoveDataSetHandler::GetName()
	{
		return "RemoveDataSet";
	}

	const char*	RemoveDataSetHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RemoveDataSetHandler::ParseRequest(rude::CGI& cgi)
	{
		RemoveDataSetRequest* pRequest = new RemoveDataSetRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RemoveDataSetHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveDataSetHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RemoveDataSetHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		RemoveDataSetRequest* pRequest = static_cast<RemoveDataSetRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		const char* dataSetName= pRequest->GetDataSetName();

		if(sourceName==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(dataSetName==NULL)
		{
			const char* msg = "Parameter [DataSetName] is not defined";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(pUser->GetID(), sourceName);
		if( pWorkspace==NULL )
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter [sourceName] is NULL");
			return pExpResponse;
		}

		DataSet* pDataSet = pWorkspace->OpenDataSet(dataSetName);
		if(pDataSet==NULL)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "DataSet [%s] does not exists.", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		pDataSet->Release();

		FeatureWorkspace* pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
		RESULTCODE rc = pFeatureWorkspace->RemoveFeatureClass(dataSetName);
		if(rc!=AG_SUCCESS)
		{
			GError *pError = augeGetErrorInstance();
			GLogger *pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());

		return pSusResponse;
	}

	WebResponse* RemoveDataSetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		RemoveDataSetRequest* pRequest = static_cast<RemoveDataSetRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		const char* raw_dataSetName= pRequest->GetDataSetName();

		if(sourceName==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(raw_dataSetName==NULL)
		{
			const char* msg = "Parameter [DataSetName] is not defined";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		char dataSetName[AUGE_NAME_MAX];
		memset(dataSetName, 0, AUGE_NAME_MAX);
		if(pWebContext->IsIE())
		{
			strcpy(dataSetName, raw_dataSetName);
		}
		else
		{
			size_t dataset_name_size = AUGE_NAME_MAX;
			auge_encoding_convert_2(AUGE_ENCODING_UTF8, AUGE_ENCODING_GBK, raw_dataSetName, strlen(raw_dataSetName), dataSetName, &dataset_name_size);
		}

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(pUser->GetID(), sourceName);
		if( pWorkspace==NULL )
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter [sourceName] is NULL");
			return pExpResponse;
		}

		DataSet* pDataSet = pWorkspace->OpenDataSet(dataSetName);
		if(pDataSet==NULL)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "DataSet [%s] does not exists.", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		pDataSet->Release();

		FeatureWorkspace* pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
		RESULTCODE rc = pFeatureWorkspace->RemoveFeatureClass(dataSetName);
		if(rc!=AG_SUCCESS)
		{
			GError *pError = augeGetErrorInstance();
			GLogger *pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());

		return pSusResponse;
	}
}