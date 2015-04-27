#include "GetDataSetHandler.h"
#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"
#include "AugeService.h"
#include "AugeData.h"


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

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest)
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
		pWorkspace = pConnectionManager->GetWorkspace(sourceName);
		if( pWorkspace==NULL )
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter [sourceName] is NULL");
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
				pExpResponse->SetMessage("Parameter [sourceName] is NULL");
				return pExpResponse;
			}
			else
			{
				GetDataSetResponse* pResponse = new GetDataSetResponse(pRequest);
				pResponse->SetDataSet(pDataSet);
				pWebResponse = pResponse;
			}
		}
		else
		{
			EnumDataSet* pDataSets = pWorkspace->GetDataSets();
			GetDataSetResponse* pResponse = new GetDataSetResponse(pRequest);
			pResponse->SetDataSets(pDataSets);
			pWebResponse = pResponse;
		}

		return pWebResponse;
	}

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		return Execute(pWebRequest);
	}
}