#include "GetFeatureHandler.h"
#include "GetFeatureRequest.h"
#include "GetFeatureResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h" 

namespace auge
{
	GetFeatureHandler::GetFeatureHandler()
	{

	}

	GetFeatureHandler::~GetFeatureHandler()
	{

	}

	const char*	GetFeatureHandler::GetName()
	{
		return "GetFeature";
	}

	WebRequest*	GetFeatureHandler::ParseRequest(rude::CGI& cgi)
	{
		GetFeatureRequest* pRequest = new GetFeatureRequest();
		if(!pRequest->Create(cgi, NULL))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetFeatureHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		GetFeatureRequest* pRequest = new GetFeatureRequest();
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
	
	//WebRequest*	GetFeatureHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	GetFeatureRequest* pRequest = new GetFeatureRequest();
	//	if(!pRequest->Create(cgi, pMap))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//	}
	//	return pRequest;
	//}

	WebRequest*	GetFeatureHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetFeatureRequest* pRequest = new GetFeatureRequest();
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

	//WebRequest*	GetFeatureHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	GetFeatureRequest* pRequest = new GetFeatureRequest();
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

	WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);

		FeatureClass* pFeatureClass = GetFeatureClass(pWebRequest, pWebContext);
		if(pFeatureClass==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		//char cache_name[AUGE_NAME_MAX];
		//char cache_file[AUGE_PATH_MAX];
		const char* typeName = pRequest->GetTypeName();
		//const char* cache_path = pWebContext->GetCacheProtocolPath();
		//g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
		//auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

		//if(access(capa_path,4))
		FeatureCursor *pCursor = NULL; 
		GQuery* pQuery = pRequest->GetQuery(pFeatureClass); 
		if(pQuery!=NULL)
		{ 
			pCursor = pFeatureClass->Query(pQuery);
		}
		else
		{
			GEnvelope& bbox = pRequest->GetBBox(); 
			if(bbox.IsValid()) 
			{
				pCursor = pFeatureClass->Query(bbox);
			}
			else
			{
				pCursor = pFeatureClass->Query();
			}
		}

		WebResponse* pWebResponse = NULL; 
		if(pCursor!=NULL) 
		{ 
			GetFeatureResponse *pResponse = new GetFeatureResponse(pRequest);
			pResponse->SetWebContenxt(pWebContext);
			pResponse->SetFeatureCursor(pCursor);
			pWebResponse = pResponse;
		}
		else
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		
		pFeatureClass->Release();
		
		return pWebResponse;
	}

	//WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	//{
	//	GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);

	//	const char* typeName = NULL;
	//	Layer* pLayer = NULL;
	//	GLogger *pLogger = augeGetLoggerInstance();
	//	typeName = pRequest->GetTypeName();

	//	const char* mapName = pRequest->GetMapName();
	//	if(mapName==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		g_sprintf(msg, "No Map is attached");
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	CartoManager* pCartoManager = augeGetCartoManagerInstance();
	//	Map *pMap = pCartoManager->LoadMap(mapName);
	//	if(pMap==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		g_sprintf(msg, "Cannot load map [%s]", mapName);
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	pLayer = pMap->GetLayer(typeName);
	//	if(pLayer==NULL)
	//	{
	//		const char* ns = pWebContext->GetService() ? pWebContext->GetService() : pMap->GetName();

	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Service %s has not FeatureType %s,",ns, typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	if(pLayer->GetType()!=augeLayerFeature)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s is not a Feature Layer",typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	FeatureClass* pFeatureClass = NULL;
	//	FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
	//	pFeatureClass = pFLayer->GetFeatureClass();
	//	if(pFeatureClass==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	FeatureCursor *pCursor = NULL; 
	//	GQuery* pQuery = pRequest->GetQuery(pFeatureClass); 
	//	if(pQuery!=NULL)
	//	{ 
	//		pCursor = pFeatureClass->Query(pQuery);
	//	}
	//	else
	//	{
	//		GEnvelope& bbox = pRequest->GetBBox(); 
	//		if(bbox.IsValid()) 
	//		{
	//			pCursor = pFeatureClass->Query(bbox);
	//		}
	//		else
	//		{
	//			pCursor = pFeatureClass->Query();
	//		}
	//	}

	//	WebResponse* pWebResponse = NULL; 
	//	if(pCursor!=NULL) 
	//	{ 
	//		GetFeatureResponse *pResponse = new GetFeatureResponse(pRequest);
	//		pResponse->SetWebContenxt(pWebContext);
	//		pResponse->SetFeatureCursor(pCursor);
	//		pWebResponse = pResponse;
	//	}
	//	else
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		pWebResponse = pExpResponse;
	//	}
	//	
	//	pMap->Release();

	//	return pWebResponse;
	//}

	//WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);

	//	const char* typeName = NULL;
	//	Layer* pLayer = NULL;
	//	GLogger *pLogger = augeGetLoggerInstance();
	//	typeName = pRequest->GetTypeName();

	//	//if(pMap==NULL)
	//	//{
	//	//	char msg[AUGE_MSG_MAX];
	//	//	g_sprintf(msg, "FeatureType [%s] doesn't exist.",typeName);
	//	//	pLogger->Error(msg, __FILE__, __LINE__);

	//	//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//	//	pExpResopnse->SetMessage(msg);
	//	//	return pExpResopnse;
	//	//}

	//	pLayer = pMap->GetLayer(typeName);
	//	if(pLayer==NULL)
	//	{
	//		const char* ns = pWebContext->GetService() ? pWebContext->GetService() : pMap->GetName();

	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Service %s has not FeatureType %s,",ns, typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	if(pLayer->GetType()!=augeLayerFeature)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s is not a Feature Layer",typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	FeatureClass* pFeatureClass = NULL;
	//	FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
	//	pFeatureClass = pFLayer->GetFeatureClass();
	//	if(pFeatureClass==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	FeatureCursor *pCursor = NULL; 
	//	GQuery* pQuery = pRequest->GetQuery(pFeatureClass); 
	//	if(pQuery!=NULL)
	//	{ 
	//		pCursor = pFeatureClass->Query(pQuery);
	//	}
	//	else
	//	{
	//		GEnvelope& bbox = pRequest->GetBBox(); 
	//		if(bbox.IsValid()) 
	//		{
	//			pCursor = pFeatureClass->Query(bbox);
	//		}
	//		else
	//		{
	//			pCursor = pFeatureClass->Query();
	//		}
	//	}

	//	WebResponse* pWebResponse = NULL; 
	//	if(pCursor!=NULL) 
	//	{ 
	//		GetFeatureResponse *pResponse = new GetFeatureResponse(pRequest);
	//		pResponse->SetWebContenxt(pWebContext);
	//		pResponse->SetFeatureCursor(pCursor);
	//		pWebResponse = pResponse;
	//	}
	//	else
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		pWebResponse = pExpResponse;
	//	}
	//	
	//	return pWebResponse;
	//}


	FeatureClass* GetFeatureHandler::GetFeatureClass(WebRequest* pWebRequest, WebContext* pWebContext)
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

	FeatureClass* GetFeatureHandler::GetFeatureClassByMap(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);
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

	FeatureClass* GetFeatureHandler::GetFeatureClassBySource(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);
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
