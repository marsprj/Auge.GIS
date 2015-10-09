#include "CreateDataSetHandler.h"
#include "CreateDataSetRequest.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeUser.h"

namespace auge
{
	CreateDataSetHandler::CreateDataSetHandler()
	{

	}

	CreateDataSetHandler::~CreateDataSetHandler()
	{

	}

	const char*	CreateDataSetHandler::GetName()
	{
		return "CreateDataSet";
	}

	const char*	CreateDataSetHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	CreateDataSetHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateDataSetRequest* pRequest = new CreateDataSetRequest();

		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebRequest*	CreateDataSetHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateDataSetHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		CreateDataSetRequest* pRequest = new CreateDataSetRequest();
		//if(!pRequest->Create(pxDoc, pMap))
		if(!pRequest->Create(pxDoc))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		return pRequest;
	}

	WebResponse* CreateDataSetHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		CreateDataSetRequest* pRequest = static_cast<CreateDataSetRequest*>(pWebRequest);
		const char* sourceName = pRequest->GetSourceName();
		const char* dataSetName= pRequest->GetDataSetName();

		if(sourceName==NULL)
		{
			const char* msg = "Parameter [SourceName] is not defined";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
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

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		FeatureWorkspace* pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspace(pUser->GetID(), sourceName));
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Cannot find DataSource [%s]", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass(dataSetName);
		if(pFeatureClass!=NULL)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Dataset [%s] already existed", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		GFields* pFields = pRequest->GetFields();
		if(pFields==NULL)
		{
			const char* msg = "Fail to parse fields by request xml document";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		FeatureClass* pNewClass = pWorkspace->CreateFeatureClass(dataSetName, pFields);
		if(pNewClass==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		pNewClass->Release();
		WebSuccessResponse* pSussResponse = augeCreateWebSuccessResponse();
		pSussResponse->SetRequest(pRequest->GetRequest());
		return pSussResponse;
	}

	WebResponse* CreateDataSetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}