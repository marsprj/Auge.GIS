#include "GetFeatureInfoHandler.h"
#include "GetFeatureInfoRequest.h"
#include "GetFeatureInfoResponse.h"
#include "WMapEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"

namespace auge
{
	GetFeatureInfoHandler::GetFeatureInfoHandler()
	{

	}

	GetFeatureInfoHandler::~GetFeatureInfoHandler()
	{

	}

	const char*	GetFeatureInfoHandler::GetName()
	{
		return "GetFeatureInfo";
	}

	WebRequest*	GetFeatureInfoHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		GetFeatureInfoRequest* pRequest = new GetFeatureInfoRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetFeatureInfoHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		GetFeatureInfoRequest* pRequest = new GetFeatureInfoRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* GetFeatureInfoHandler::Execute(WebRequest* pWebRequest)
	{
		WebResponse* pWebResponse = NULL;
		GetFeatureInfoRequest* pRequest = static_cast<GetFeatureInfoRequest*>(pWebRequest);

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

	WebResponse* GetFeatureInfoHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		GetFeatureInfoRequest* pRequest = static_cast<GetFeatureInfoRequest*>(pWebRequest);

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
				if(strlen(sname)>0)
				{	// User defined Style
					pStyle = pCartoManager->GetStyle(sname);
					if(pStyle!=NULL)
					{
						pCanvas->DrawLayer(pLayer, pStyle);
					}
					else
					{
						char msg[AUGE_MSG_MAX];
						g_sprintf(msg, "Style [%s] Not Defined", sname);
						GLogger* pLogger = augeGetLoggerInstance();
						pLogger->Info(msg, __FILE__, __LINE__);
					}
				}
				else
				{	// Default Style
					pCanvas->DrawLayer(pLayer);
				}
			}
		}

		char img_sfix[AUGE_EXT_MAX] = {0};
		char img_name[AUGE_NAME_MAX] = {0};
		char img_path[AUGE_PATH_MAX] = {0};
		auge_get_image_suffix(pRequest->GetMimeType(), img_sfix, AUGE_EXT_MAX);
		auge_generate_uuid(img_name, AUGE_NAME_MAX);
		const char* cache_path = pWebContext->GetCacheMapPath();
		auge_make_path(img_path, NULL, cache_path, img_name, img_sfix);
		pCanvas->Save(img_path);
		pCanvas->Release();
		GetFeatureInfoResponse* pMapResponse = new GetFeatureInfoResponse(pRequest);
		pMapResponse->SetPath(img_path);
		return pMapResponse;
	}

}