#include "GetDataSetHandler.h"
#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetDataSetHandler::GetDataSetHandler()
	{

	}

	GetDataSetHandler::~GetDataSetHandler()
	{

	}

	const char*	GetDataSetHandler::GetName()
	{
		return "GetDataSet";
	}

	WebRequest*	GetDataSetHandler::ParseRequest(rude::CGI& cgi)
	{
		GetDataSetRequest* pRequest = new GetDataSetRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetDataSetHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetDataSetHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GetDataSetRequest* pRequest = static_cast<GetDataSetRequest*>(pWebRequest);

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
		if(dataSetName!=NULL)
		{	
			DataSet* pDataSet = pWorkspace->OpenDataSet(dataSetName);
			if(pDataSet==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Cannot Find DataSet [%s]", dataSetName);
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error(msg, __FILE__, __LINE__);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				return pExpResponse;
			}
			else
			{
				GetDataSetResponse* pResponse = new GetDataSetResponse(pRequest);
				pResponse->SetDataSet(pDataSet);
				pResponse->SetWebContext(pWebContext);
				pWebResponse = pResponse;
			}
		}
		else
		{
			FeatureWorkspace* pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
			EnumDataSet* pDataSets = pFeatureWorkspace->GetDataSets();
			GetDataSetResponse* pResponse = new GetDataSetResponse(pRequest);
			pResponse->SetDataSets(pDataSets);
			pResponse->SetWebContext(pWebContext);
			pWebResponse = pResponse;
		}
		
		return pWebResponse;
	}
}