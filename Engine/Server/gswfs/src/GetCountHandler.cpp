#include "GetCountHandler.h"
#include "GetCountRequest.h"
#include "GetCountResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h" 

namespace auge
{
	GetCountHandler::GetCountHandler()
	{

	}

	GetCountHandler::~GetCountHandler()
	{

	}

	const char*	GetCountHandler::GetName()
	{
		return "GetCount";
	}

	WebRequest*	GetCountHandler::ParseRequest(rude::CGI& cgi)
	{
		GetCountRequest* pRequest = new GetCountRequest();
		if(!pRequest->Create(cgi, NULL))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetCountHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		GetCountRequest* pRequest = new GetCountRequest();
		if(!pRequest->Create(cgi, NULL))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->SetMapName(mapName);
		return pRequest;
	}
	
	//WebRequest*	GetCountHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	GetCountRequest* pRequest = new GetCountRequest();
	//	if(!pRequest->Create(cgi, pMap))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//	}
	//	return pRequest;
	//}

	WebRequest*	GetCountHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetCountRequest* pRequest = new GetCountRequest();
		//if(!pRequest->Create(pxDoc, pMap))
		if(!pRequest->Create(pxDoc))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		pRequest->SetMapName(mapName);
		return pRequest;
	}

	//WebRequest*	GetCountHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	GetCountRequest* pRequest = new GetCountRequest();
	//	//if(!pRequest->Create(pxDoc, pMap))
	//	if(!pRequest->Create(pxDoc))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		return NULL;
	//	}
	//	return pRequest;
	//}

	WebResponse* GetCountHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* GetCountHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GetCountRequest* pRequest = static_cast<GetCountRequest*>(pWebRequest);

		FeatureClass* pFeatureClass = GetFeatureClass(pWebRequest, pWebContext);
		if(pFeatureClass==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		
		g_uint fcount = 0;
		FeatureCursor *pCursor = NULL; 
		GFilter* pFilter = pRequest->GetFilter(pFeatureClass); 
		if(pFilter!=NULL)
		{ 
			fcount = pFeatureClass->GetCount(pFilter);
		}
		else
		{
			GEnvelope& bbox = pRequest->GetBBox(); 
			if(bbox.IsValid()) 
			{
				fcount = pFeatureClass->GetCount(bbox);
			}
			else
			{
				fcount = pFeatureClass->GetCount();
			}
		}

		WebResponse* pWebResponse = NULL; 
		GetCountResponse *pResponse = new GetCountResponse(pRequest);
		pResponse->SetWebContenxt(pWebContext);
		pResponse->SetCount(fcount);

		pFeatureClass->Release();
		
		return pResponse;
	}

	FeatureClass* GetCountHandler::GetFeatureClass(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		FeatureClass* pFeatureClass = NULL;
		WFeatureRequest* pRequest = static_cast<WFeatureRequest*>(pWebRequest);
		if(pRequest->IsValidSource())
		{
			pFeatureClass = GetFeatureClassBySource(pWebRequest, pWebContext);
		}
		else
		{
			pFeatureClass = GetFeatureClassByMap(pWebRequest, pWebContext);
		}
		return pFeatureClass;
	}

	FeatureClass* GetCountHandler::GetFeatureClassByMap(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetCountRequest* pRequest = static_cast<GetCountRequest*>(pWebRequest);
		typeName = pRequest->GetTypeName();

		const char* mapName = pRequest->GetMapName();
		if(mapName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Map is attached");
			return NULL;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot load map [%s]", mapName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		pLayer = pMap->GetLayer(typeName);
		if(pLayer==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		if(pLayer->GetType()!=augeLayerFeature)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s is not a Feature Layer",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		FeatureClass* pFeatureClass = NULL;
		FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
		pFeatureClass = pFLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		pFeatureClass->AddRef();
		return pFeatureClass;
	}

	FeatureClass* GetCountHandler::GetFeatureClassBySource(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetCountRequest* pRequest = static_cast<GetCountRequest*>(pWebRequest);
		typeName = pRequest->GetTypeName();

		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Source is attached");
			return NULL;
		}

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		Workspace* pWorkspace = pConnManager->GetWorkspace(sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No DataSource Named [%s]", sourceName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		FeatureWorksapce* pFeatureWorkspace = dynamic_cast<FeatureWorksapce*>(pWorkspace);
		FeatureClass* pFeatureClass = pFeatureWorkspace->OpenFeatureClass(typeName);
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		return pFeatureClass;
	}
}
