#include "TileUpdateHandler.h"
#include "TileUpdateRequest.h"
#include "GProcessEngine.h"
#include "AugeData.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeTile.h"
#include "AugeCarto.h"
#include "AugeCore.h"

namespace auge
{
	UpdateTileHandler::UpdateTileHandler()
	{

	}

	UpdateTileHandler::~UpdateTileHandler()
	{

	}

	const char*	UpdateTileHandler::GetName()
	{
		return "UpdateTile";
	}

	WebRequest*	UpdateTileHandler::ParseRequest(rude::CGI& cgi)
	{
		UpdateTileRequest* pRequest = new UpdateTileRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	UpdateTileHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	UpdateTileHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		UpdateTileRequest* pRequest = new UpdateTileRequest();
		return pRequest;
	}

	WebResponse* UpdateTileHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
		//WebResponse* pWebResponse = NULL;
		//UpdateTileRequest* pRequest = static_cast<UpdateTileRequest*>(pWebRequest);

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

		//return pWebResponse;
	}

	WebResponse* UpdateTileHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		UpdateTileRequest* pRequest = static_cast<UpdateTileRequest*>(pWebRequest);

		GLogger* pLogger = augeGetLoggerInstance();

		const char* mapName = pRequest->GetMapName();
		const char* sourceName = pRequest->GetSourceName();
		const char* tileStoreName = pRequest->GetTileStoreName();
		g_int	level = pRequest->GetLevel();
		g_int	row   = pRequest->GetRow();
		g_int	col   = pRequest->GetCol();

		if(mapName==NULL)
		{
			const char* msg = "Parameter [MapName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}
		if(sourceName==NULL)
		{
			const char* msg = "Parameter [SourceName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}
		if(tileStoreName==NULL)
		{
			const char* msg = "Parameter [TileStoreName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}

		TileWorkspace* pTileWorkspace = NULL;
		ConnectionManager *pConnManager = augeGetConnectionManagerInstance();
		pTileWorkspace = dynamic_cast<TileWorkspace*>(pConnManager->GetWorkspace(sourceName));
		if(pTileWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Cannot connect to datasource [%s]", sourceName);			
			pLogger->Error(msg,__FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}

		Map* pMap = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		pMap = pCartoManager->LoadMap(mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_SQL_MAX];
			g_sprintf(msg, "Cannot Load Map [%s]", mapName);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}
		
		TileStore* pTileStore = pTileWorkspace->OpenTileStore(tileStoreName);
		if(pTileStore==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Open Tile Store [%s]", tileStoreName);
			pLogger->Error(msg,__FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();			
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}

		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		pCanvas = pCartoFactory->CreateCanvas2D(255, 255);
		
		char t_path[AUGE_PATH_MAX];
		pTileStore->GetTilePath(t_path, AUGE_PATH_MAX, level, row, col);

		GEnvelope viewer;
		pTileStore->GetExtent(viewer, level, row,col);
		pCanvas->SetViewer(viewer);
		pCanvas->Draw(pMap);
		pCanvas->Save(t_path);

		AUGE_SAFE_RELEASE(pMap);
		AUGE_SAFE_RELEASE(pCanvas);
		
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pWebRequest->GetRequest());

		return pSusResponse;

	}
}
