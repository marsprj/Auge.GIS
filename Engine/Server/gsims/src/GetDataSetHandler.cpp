#include "GetDataSetHandler.h"
#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"


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

	WebRequest*	GetDataSetHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
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

	WebRequest*	GetDataSetHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
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

		EnumDataSet* pDataSets = pWorkspace->GetDataSets();
		GetDataSetResponse* pResponse = new GetDataSetResponse();
		pResponse->SetDataSets();

		return pResponse;
	}

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}