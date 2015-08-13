#include "BuildPyramidHandler.h"
#include "BuildPyramidRequest.h"
#include "GProcessEngine.h"
#include "AugeData.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeTile.h"
#include "AugeCarto.h"
#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	BuildPyramidHandler::BuildPyramidHandler()
	{

	}

	BuildPyramidHandler::~BuildPyramidHandler()
	{

	}

	const char*	BuildPyramidHandler::GetName()
	{
		return "BuildPyramid";
	}

	WebRequest*	BuildPyramidHandler::ParseRequest(rude::CGI& cgi)
	{
		BuildPyramidRequest* pRequest = new BuildPyramidRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	BuildPyramidHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	BuildPyramidHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		BuildPyramidRequest* pRequest = new BuildPyramidRequest();
		return pRequest;
	}

	WebResponse* BuildPyramidHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* BuildPyramidHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		BuildPyramidRequest* pRequest = static_cast<BuildPyramidRequest*>(pWebRequest);

		GLogger* pLogger = augeGetLoggerInstance();

		const char* mapName = pRequest->GetMapName();
		const char* sourceName = pRequest->GetSourceName();
		const char* tileStoreName = pRequest->GetTileStoreName();
		g_uint start_level = pRequest->GetStartLevel();
		g_uint end_level = pRequest->GetEndLevel();

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
		if((start_level<=0)||(end_level<=0)||(start_level>end_level))
		{
			const char* msg = "Start or End Level Error";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}

		TileWorkspace* pTileWorkspace = NULL;
		ConnectionManager *pConnManager = augeGetConnectionManagerInstance();
		pTileWorkspace = dynamic_cast<TileWorkspace*>(pConnManager->GetWorkspace(pUser->GetID(), sourceName));
		if(pTileWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Cannot connect to datasource [%s]", sourceName);			
			pLogger->Error(msg,__FILE__, __LINE__);
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

		Map* pMap = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_SQL_MAX];
			g_sprintf(msg, "Cannot Load Map [%s]", mapName);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pWebResponse;
		}

		GEnvelope& viewer = pMap->GetExtent();

		MapTileStoreGenerator* pProcessor = NULL;
		GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		//pProcessor = pFactory->CreateTileStoreGenerator();
		pProcessor = pFactory->CreateMongoTileStoreGenerator();

		pProcessor->SetMap(pMap);
		pProcessor->SetTileStore(pTileStore);
		pProcessor->SetStartLevel(start_level);
		pProcessor->SetEndLevel(end_level);
		pProcessor->SetViewer(viewer);

		g_uint counter = pProcessor->Execute();

		pProcessor->Release();

		AUGE_SAFE_RELEASE(pMap);
		AUGE_SAFE_RELEASE(pTileStore);
		
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pWebRequest->GetRequest());

		return pSusResponse;

	}
}
