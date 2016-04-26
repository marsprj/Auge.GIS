#include "CreateTileStoreHandler.h"
#include "CreateTileStoreRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeTile.h"
#include "AugeUser.h"

namespace auge
{
	CreateTileStoreHandler::CreateTileStoreHandler()
	{

	}

	CreateTileStoreHandler::~CreateTileStoreHandler()
	{

	}

	const char*	CreateTileStoreHandler::GetName()
	{
		return "CreateTileStore";
	}

	const char*	CreateTileStoreHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	CreateTileStoreHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateTileStoreRequest* pRequest = new CreateTileStoreRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	CreateTileStoreHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateTileStoreHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* CreateTileStoreHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* CreateTileStoreHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		CreateTileStoreRequest* pRequest = static_cast<CreateTileStoreRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		const char* storeName= pRequest->GetStoreName();
		GEnvelope& extent = pRequest->GetExtent();
		g_uint	start_level = pRequest->GetStartLevel();
		g_uint	end_level = pRequest->GetEndLevel();
		
		augeTileType type = pRequest->GetType();

		if(sourceName==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(storeName==NULL)
		{
			const char* msg = "Parameter [storeName] is NULL";
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

		//RESULTCODE rc = pTileWorkspace->CreateTileStore(storeName, type);
		RESULTCODE rc = pTileWorkspace->CreateTileStore(storeName, type, start_level, end_level, extent);
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