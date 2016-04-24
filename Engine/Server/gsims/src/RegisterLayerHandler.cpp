#include "RegisterLayerHandler.h"
#include "RegisterLayerRequest.h"
#include "RegisterLayerResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	RegisterLayerHandler::RegisterLayerHandler()
	{

	}

	RegisterLayerHandler::~RegisterLayerHandler()
	{

	}

	const char*	RegisterLayerHandler::GetName()
	{
		return "RegisterLayer";
	}

	const char*	RegisterLayerHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RegisterLayerHandler::ParseRequest(rude::CGI& cgi)
	{
		RegisterLayerRequest* pRequest = new RegisterLayerRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* RegisterLayerHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RegisterLayerHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RegisterLayerHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		RegisterLayerRequest* pRequest = static_cast<RegisterLayerRequest*>(pWebRequest);

		const char* mapName = pRequest->GetMapName();
		const char* dataSource = pRequest->GetDataSource();
		const char* layerName = pRequest->GetLayerName();
		const char* tableName = pRequest->GetTableName();
		augeLayerType layerType = pRequest->GetType();
		g_int styleID = -1;

		if(layerType==augeLayerNone)
		{
			const char* msg = "Paramter [LayerType] is not defined";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(mapName==NULL)
		{
			const char* msg = "Parameter mapName is null.";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Map* pMap = NULL;
		CartoManager* pCartoManager = NULL;
		pCartoManager = augeGetCartoManagerInstance();
		
		pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Map %s is not registered.", mapName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		WebResponse* pWebResponse = NULL;
		switch(layerType)
		{
		case augeLayerFeature:
			pWebResponse = RegisterFeatureLayer(pRequest, pMap, pUser);
			break;
		case augeLayerRaster:
			pWebResponse = RegisterRasterLayer(pRequest, pMap, pUser);
			break;
		case augeLayerQuadServer:
			pWebResponse = RegisterQuadServerLayer(pRequest, pMap, pUser);
			break;
		}
		//Workspace *pWorkspace = NULL;
		//ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		//pWorkspace = pConnManager->GetWorkspace(dataSource);
		//if(pWorkspace == NULL)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "DataSource %s is not registered.", dataSource);
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}

		//Layer* pLayer = NULL;
		//pLayer = pCartoManager->CreateLayer(layerName, layerType, tableName, pMap->GetID(), pWorkspace->GetID(), styleID);
		//if(pLayer==NULL)
		//{
		//	GError* pError = augeGetErrorInstance();
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(pError->GetLastError());
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(pError->GetLastError());
		//	return pExpResponse;
		//}
		//pMap->AddLayer(pLayer);

		//WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		//pSusResponse->SetRequest(pRequest->GetRequest());
		//return pSusResponse;

		return pWebResponse;
	}

	WebResponse* RegisterLayerHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}

	WebResponse* RegisterLayerHandler::RegisterFeatureLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser)
	{
		const char* dataSource = pRequest->GetDataSource();
		const char* layerName = pRequest->GetLayerName();
		const char* tableName = pRequest->GetTableName();
		g_int styleID = -1;
		
		Workspace *pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), dataSource);
		if(pWorkspace == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "DataSource %s is not registered.", dataSource);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Layer* pLayer = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		pLayer = pCartoManager->CreateLayer(layerName, augeLayerFeature, tableName, pMap->GetID(), pWorkspace->GetID(), styleID);
		if(pLayer==NULL)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		pMap->AddLayer(pLayer);
		pCartoManager->UpdateMapLayers(pMap);

		//WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		//pSusResponse->SetRequest(pRequest->GetRequest());
		//return pSusResponse;

		RegisterLayerResponse* pResponse = new RegisterLayerResponse(pRequest);
		pResponse->SetLayer(pLayer);
		pLayer->AddRef();
		return pResponse;
	}

	WebResponse* RegisterLayerHandler::RegisterRasterLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser)
	{
		const char* dataSource = pRequest->GetDataSource();
		const char* layerName = pRequest->GetLayerName();
		const char* rasterName = pRequest->GetRasterName();
		const char* rasterPath = pRequest->GetRasterPath();
		g_int styleID = -1;

		Workspace *pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), dataSource);
		if(pWorkspace == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "DataSource %s is not registered.", dataSource);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Layer* pLayer = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		pLayer = pCartoManager->CreateRasterLayer(layerName, rasterName, rasterPath, pMap->GetID(), pWorkspace->GetID());
		if(pLayer==NULL)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		pMap->AddLayer(pLayer);
		pCartoManager->UpdateMapLayers(pMap);

		//WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		//pSusResponse->SetRequest(pRequest->GetRequest());
		//return pSusResponse;

		RegisterLayerResponse* pResponse = new RegisterLayerResponse(pRequest);
		pResponse->SetLayer(pLayer);
		pLayer->AddRef();
		return pResponse;

		return NULL;
	}

	WebResponse* RegisterLayerHandler::RegisterQuadServerLayer(RegisterLayerRequest* pRequest, Map* pMap, User* pUser)
	{
		const char* layerName = pRequest->GetLayerName();
		augeLayerType layerType = pRequest->GetType();
		const char* weburl = pRequest->GetWebURL();		
		
		if(weburl==NULL)
		{
			const char* msg = "Parameter [URL] is NULL";
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		
		Layer* pLayer = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		pLayer = pCartoManager->CreateWebLayer(layerName, layerType, weburl, pMap->GetID());
		if(pLayer==NULL)
		{
			GError* pError = augeGetErrorInstance();
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		pMap->AddLayer(pLayer);
		pCartoManager->UpdateMapLayers(pMap);

		//WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		//pSusResponse->SetRequest(pRequest->GetRequest());
		//return pSusResponse;

		RegisterLayerResponse* pResponse = new RegisterLayerResponse(pRequest);
		pResponse->SetLayer(pLayer);
		pLayer->AddRef();
		return pResponse;
	}
}