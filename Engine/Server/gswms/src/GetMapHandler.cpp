#include "GetMapHandler.h"
#include "GetMapRequest.h"
#include "GetMapResponse.h"
#include "WMapEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"

namespace auge
{
	GetMapHandler::GetMapHandler()
	{

	}

	GetMapHandler::~GetMapHandler()
	{

	}

	const char*	GetMapHandler::GetName()
	{
		return "GetMap";
	}

	WebRequest*	GetMapHandler::ParseRequest(rude::CGI& cgi)
	{
		GetMapRequest* pRequest = new GetMapRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetMapRequest* pRequest = new GetMapRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest)
	{
		WebResponse* pWebResponse = NULL;
		GetMapRequest* pRequest = static_cast<GetMapRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{

		}
		else if(strcmp(version,"1.3.0")==0)
		{

		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GetMapRequest* pRequest = static_cast<GetMapRequest*>(pWebRequest);

		const char* mapName = pRequest->GetMapName();
		if(mapName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No Map is attached");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "Cannot load map [%s]", mapName);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		WebResponse* pWebResponse = Execute(pWebRequest, pWebContext, pMap);
		pMap->Release();

		return pWebResponse;
	}

	WebResponse* GetMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		GetMapRequest* pRequest = static_cast<GetMapRequest*>(pWebRequest);

		g_uint width  = pRequest->GetWidth();
		g_uint height = pRequest->GetHeight();

		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		pCanvas = pCartoFactory->CreateCanvas2D(width, height);

		GColor& bgColor = pRequest->GetBgColor(); 
		//GColor bgColor(255,0,0,255);
		char temp[AUGE_NAME_MAX];
		g_sprintf(temp, "[Alpha]:%d", bgColor.GetAlpha());
		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Info(temp, __FILE__, __LINE__);
		pCanvas->DrawBackground(bgColor);

		GEnvelope& extent = pRequest->GetExtent();
		pCanvas->SetViewer(extent);

		const char* lname = NULL;
		const char* sname = NULL;

		Layer* pLayer = NULL;
		Style* pStyle = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		g_uint lc = pRequest->GetLayerCount();
		for(g_int i=lc-1; i>=0; i--)
		{
			lname = pRequest->GetLayer(i);
			sname = pRequest->GetStyle(i);

			pLayer = pMap->GetLayer(lname);
			if(pLayer!=NULL)
			{
				if(!strlen(sname))
				{	// Default Style
					//pCanvas->DrawLayer(pLayer);
					switch(pLayer->GetType())
					{
					case augeLayerFeature:
						{
							FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
							Style* pStyle = pFeatureLayer->GetStyle();
							if(pStyle!=NULL)
							{
								sname = pStyle->GetName();
							}
						}
						break;
					}
					
				}

				DrawNamedLayer(pCanvas, pLayer, sname);
			}
		}
		pCanvas->Label();

		char img_sfix[AUGE_EXT_MAX] = {0};
		char img_name[AUGE_NAME_MAX] = {0};
		char img_path[AUGE_PATH_MAX] = {0};
		auge_get_image_suffix(pRequest->GetMimeType(), img_sfix, AUGE_EXT_MAX);
		auge_generate_uuid(img_name, AUGE_NAME_MAX);
		const char* cache_path = pWebContext->GetCacheMapPath();
		auge_make_path(img_path, NULL, cache_path, img_name, img_sfix);
		pCanvas->Save(img_path);
		pCanvas->Release();
		GetMapResponse* pMapResponse = new GetMapResponse(pRequest);
		pMapResponse->SetPath(img_path);

		char img_fname[AUGE_NAME_MAX];
		memset(img_fname,0,AUGE_NAME_MAX);
		auge_make_path(img_fname, NULL, NULL, img_name, img_sfix);
		pCartoManager->SetMapThumbnail(-1, pMap->GetID(), img_fname);

		return pMapResponse;
	}

	void GetMapHandler::DrawNamedLayer(Canvas* pCanvas, Layer* pLayer, const char* style_name)
	{
		Style* pStyle = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		switch(pLayer->GetType())
		{
		case augeLayerFeature:
			{
				FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
				FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
				pStyle = pCartoManager->GetStyle(style_name, pFeatureClass);
			}
			break;
		case augeLayerRaster:
			{

			}
			break;
		}
		if(pStyle!=NULL)
		{
			pCanvas->DrawLayer(pLayer, pStyle);
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Style [%s] Not Defined", style_name);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Info(msg, __FILE__, __LINE__);
		}
	}

}