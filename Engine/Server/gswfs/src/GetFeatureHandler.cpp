#include "GetFeatureHandler.h"
#include "GetFeatureRequest.h"
#include "GetFeatureResponse.h"
#include "GetFeatureResponseShapezip.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h" 
#include "AugeUser.h"

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

	const char*	GetFeatureHandler::GetDescription()
	{
		return GetName();
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
		if((mapName!=NULL)&&(strlen(mapName)>0))
		{
			pRequest->SetMapName(mapName);
		}
		pRequest->Info();
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

	WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);

		FeatureClass* pFeatureClass = GetFeatureClass(pWebRequest, pWebContext, pUser);
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
		//GEnvelope& bbox = pRequest->GetBBox(); 
		//if(bbox.IsValid()) 
		//{
		//	pCursor = pFeatureClass->Query(bbox);
		//}
		//else
		{
			GQuery* pQuery = pRequest->GetQuery(pFeatureClass); 
			if(pQuery!=NULL)
			{ 
				pCursor = pFeatureClass->Query(pQuery);
			}
			else
			{
				pCursor = pFeatureClass->Query();
			}
		}

		WebResponse* pWebResponse = NULL; 
		if(pCursor==NULL) 
		{
			pFeatureClass->Release();

			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}

		const char* output_format = pRequest->GetOutputFormat();
		
		if(g_stricmp(output_format,"shape-zip")==0)
		{
			GetFeatureShapeZipResponse* pResponse = NULL;
			pResponse = new GetFeatureShapeZipResponse(pRequest);
			pResponse->SetWebContenxt(pWebContext);
			pResponse->SetFeatureCursor(pCursor);
			pWebResponse = pResponse;
		}
		else
		{
			GetFeatureResponse *pResponse = NULL;
			pResponse = new GetFeatureResponse(pRequest);
			pResponse->SetWebContenxt(pWebContext);
			pResponse->SetFeatureCursor(pCursor);
			pWebResponse = pResponse;
		}
		
		
		pFeatureClass->Release();
		
		return pWebResponse;
	}
	
	FeatureClass* GetFeatureHandler::GetFeatureClass(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		FeatureClass* pFeatureClass = NULL;
		WFeatureRequest* pRequest = static_cast<WFeatureRequest*>(pWebRequest);
		if(pRequest->IsValidSource())
		{
			pFeatureClass = GetFeatureClassBySource(pWebRequest, pWebContext, pUser);
		}
		else
		{
			pFeatureClass = GetFeatureClassByMap(pWebRequest, pWebContext, pUser);
		}
		return pFeatureClass;
	}

	FeatureClass* GetFeatureHandler::GetFeatureClassByMap(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);
		typeName = pRequest->GetTypeName();

		const char* raw_mapName = pRequest->GetMapName();
		if(raw_mapName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Map is attached");
			return NULL;
		}

		char mapName[AUGE_NAME_MAX];	
		auge_web_parameter_encoding(raw_mapName,mapName,AUGE_NAME_MAX,pWebContext->IsIE());

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
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

	FeatureClass* GetFeatureHandler::GetFeatureClassBySource(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{	
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);
		const char* raw_typeName = pRequest->GetTypeName();
		const char* raw_sourceName = pRequest->GetSourceName();

		if(raw_sourceName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Source is attached");
			return NULL;
		}

		if(raw_typeName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("Parameter [typeName] is NULL");
			return NULL;
		}

		char sourceName[AUGE_NAME_MAX];	
		auge_web_parameter_encoding(raw_sourceName,sourceName,AUGE_NAME_MAX,pWebContext->IsIE());
		char typeName[AUGE_NAME_MAX];	
		auge_web_parameter_encoding(raw_typeName,typeName,AUGE_NAME_MAX,pWebContext->IsIE());

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		Workspace* pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No DataSource Named [%s]", sourceName);
			pLogger->Error(msg);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		FeatureWorkspace* pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
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
