#include "DescribeLayerHandler.h"
#include "DescribeLayerRequest.h"
#include "DescribeLayerResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	DescribeLayerHandler::DescribeLayerHandler()
	{

	}

	DescribeLayerHandler::~DescribeLayerHandler()
	{

	}

	const char*	DescribeLayerHandler::GetName()
	{
		return "DescribeLayer";
	}

	const char*	DescribeLayerHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	DescribeLayerHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeLayerRequest* pRequest = new DescribeLayerRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* DescribeLayerHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeLayerHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeLayerHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//DescribeLayerRequest* pRequest = static_cast<DescribeLayerRequest*>(pWebRequest);
		//DescribeLayerResponse* pWebResponse = new DescribeLayerResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->DescribeLayers();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* DescribeLayerHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		DescribeLayerRequest* pRequest = static_cast<DescribeLayerRequest*>(pWebRequest);		
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* mapName = pRequest->GetMapName();
		const char* layerName = pRequest->GetLayerName();

		if(mapName==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter [mapName] is NULL");
			return pExpResponse;
		}

		if(layerName==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter [layerName] is NULL");
			return pExpResponse;
		}

		Map* pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		if(pMap==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		Layer* pLayer = pMap->GetLayer(layerName);
		if(pLayer==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		DescribeLayerResponse* pResponse = new DescribeLayerResponse(pRequest);
		pResponse->SetLayer(pLayer);
		pResponse->SetWebContext(pWebContext);
		
		pLayer->AddRef();
		pMap->Release();

		return pResponse;
	}
}