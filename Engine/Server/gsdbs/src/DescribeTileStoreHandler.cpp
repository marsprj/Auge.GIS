#include "DescribeTileStoreHandler.h"
#include "DescribeTileStoreRequest.h"
#include "DescribeTileStoreResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	DescribeTileStoreHandler::DescribeTileStoreHandler()
	{

	}

	DescribeTileStoreHandler::~DescribeTileStoreHandler()
	{

	}

	const char*	DescribeTileStoreHandler::GetName()
	{
		return "DescribeTileStore";
	}

	const char*	DescribeTileStoreHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	DescribeTileStoreHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeTileStoreRequest* pRequest = new DescribeTileStoreRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	DescribeTileStoreHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeTileStoreHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeTileStoreHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* DescribeTileStoreHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		DescribeTileStoreRequest* pRequest = static_cast<DescribeTileStoreRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		const char* storeName= pRequest->GetStoreName();

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

		TileWorkspace* pTileWorkspace = dynamic_cast<TileWorkspace*>(pWorkspace);
		if(pTileWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "[%s] is not a Tile Workspace", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		
		WebResponse* pWebResponse = NULL;
		if(storeName!=NULL)
		{	
			TileStore* pTileStore = pTileWorkspace->OpenTileStore(storeName);
			if(pTileStore==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Cannot Find TileStore [%s]", storeName);
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error(msg, __FILE__, __LINE__);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				return pExpResponse;
			}
			else
			{
				DescribeTileStoreResponse* pResponse = new DescribeTileStoreResponse(pRequest);
				pResponse->SetTileStore(pTileStore);
				pResponse->SetWebContext(pWebContext);
				pWebResponse = pResponse;
			}
		}
		else
		{
			EnumTileStore* pTileStores = pTileWorkspace->GetTileStores();
			DescribeTileStoreResponse* pResponse = new DescribeTileStoreResponse(pRequest);
			pResponse->SetTileStores(pTileStores);
			pResponse->SetWebContext(pWebContext);
			pWebResponse = pResponse;
		}
		
		return pWebResponse;
	}
}