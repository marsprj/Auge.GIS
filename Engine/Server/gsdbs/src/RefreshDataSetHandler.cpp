#include "RefreshDataSetHandler.h"
#include "RefreshDataSetRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	RefreshDatasetHandler::RefreshDatasetHandler()
	{

	}

	RefreshDatasetHandler::~RefreshDatasetHandler()
	{

	}

	const char*	RefreshDatasetHandler::GetName()
	{
		return "Refresh";
	}

	const char*	RefreshDatasetHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RefreshDatasetHandler::ParseRequest(rude::CGI& cgi)
	{
		RefreshDatasetRequest* pRequest = new RefreshDatasetRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RefreshDatasetHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RefreshDatasetHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RefreshDatasetHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* RefreshDatasetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		RefreshDatasetRequest* pRequest = static_cast<RefreshDatasetRequest*>(pWebRequest);

		const char* raw_sourceName = pRequest->GetSourceName();
		const char* raw_dataSetName= pRequest->RefreshDatasetName();

		if(raw_sourceName==NULL)
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
			const char* msg = "Parameter [datasetName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		char sourceName[AUGE_NAME_MAX];
		char dataSetName[AUGE_NAME_MAX];
		auge_web_parameter_encoding(raw_sourceName, sourceName, AUGE_NAME_MAX, pWebContext->IsIE());
		auge_web_parameter_encoding(raw_dataSetName, dataSetName, AUGE_NAME_MAX, pWebContext->IsIE());

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(pUser->GetID(), sourceName);
		if( pWorkspace==NULL )
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get DataSource [%s]", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(!pWorkspace->IsOpen())
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "[%s] Cannot connect to data source", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		WebResponse* pWebResponse = NULL;
		if(dataSetName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Parameter [%s] is NULL", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		DataSet* pDataset = pWorkspace->OpenDataSet(dataSetName);
		if(pDataset==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Find DataSet [%s]", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		
		if(pDataset->GetType()!=augeDataSetFeature)
		{
			pDataset->Release();

			const char* msg = "Only FeatureClass is supported";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		FeatureClass* pFeatureClass = static_cast<FeatureClass*>(pDataset);
		RESULTCODE rc = pFeatureClass->Refresh();
		pDataset->Release();

		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}