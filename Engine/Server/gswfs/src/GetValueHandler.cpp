#include "GetValueHandler.h"
#include "GetValueRequest.h"
#include "GetValueResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"

namespace auge
{
	GetValueHandler::GetValueHandler()
	{

	}

	GetValueHandler::~GetValueHandler()
	{

	}
	
	const char*	GetValueHandler::GetName()
	{
		return "GetValue";
	}

	WebRequest* GetValueHandler::ParseRequest(rude::CGI& cgi)
	{
		GetValueRequest* pRequest = new GetValueRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		return pRequest;
	}

	WebRequest* GetValueHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		GetValueRequest* pRequest = new GetValueRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		pRequest->SetMapName(mapName);
		return pRequest;
	}

	//WebRequest*	GetValueHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	GetValueRequest* pRequest = new GetValueRequest();
	//	if(!pRequest->Create(cgi))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//	}
	//	return pRequest;
	//}

	WebRequest*	GetValueHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		GetValueRequest* pRequest = static_cast<GetValueRequest*>(pWebRequest);

		const char* field = pRequest->GetField();
		if(field==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Invalid Parameter Field");
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		FeatureClass* pFeatureClass = GetFeatureClass(pWebRequest, pWebContext);
		if(pFeatureClass==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		EnumValue* pValues = pFeatureClass->GetUniqueValue(field, pRequest->GetOrder());
	 	GetValueResponse* pWebResponse = new GetValueResponse(pRequest);
		pWebResponse->SetValues(pValues);

		pFeatureClass->Release();

		return pWebResponse;
	}

	//WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	//{
	//	GetValueResponse* pWebResponse = NULL;
	//	GetValueRequest*  pRequest = static_cast<GetValueRequest*>(pWebRequest);
	//	GLogger *pLogger = augeGetLoggerInstance();

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

	//	const char* field = pRequest->GetField();
	//	if(field==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Invalid Parameter Field");
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	const char* typeName = pRequest->GetTypeName();

	//	Layer* pLayer = pMap->GetLayer(typeName);
	//	if(pLayer==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
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

	//	EnumValue* pValues = pFeatureClass->GetUniqueValue(field, pRequest->GetOrder());
	//	pWebResponse = new GetValueResponse(pRequest);
	//	pWebResponse->SetValues(pValues);

	//	pMap->Release();

	//	return pWebResponse;
	//}

	//WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	GetValueResponse* pWebResponse = NULL;
	//	GetValueRequest*  pRequest = static_cast<GetValueRequest*>(pWebRequest);
	//	GLogger *pLogger = augeGetLoggerInstance();

	//	const char* field = pRequest->GetField();
	//	if(field==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Invalid Parameter Field");
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}

	//	const char* typeName = pRequest->GetTypeName();

	//	Layer* pLayer = pMap->GetLayer(typeName);
	//	if(pLayer==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
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
	//	
	//	EnumValue* pValues = pFeatureClass->GetUniqueValue(field, pRequest->GetOrder());
	//	pWebResponse = new GetValueResponse(pRequest);
	//	pWebResponse->SetValues(pValues);
	//	
	//	return pWebResponse;
	//}

	FeatureClass* GetValueHandler::GetFeatureClass(WebRequest* pWebRequest, WebContext* pWebContext)
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

	FeatureClass* GetValueHandler::GetFeatureClassByMap(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetValueRequest* pRequest = static_cast<GetValueRequest*>(pWebRequest);
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

	FeatureClass* GetValueHandler::GetFeatureClassBySource(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetValueRequest* pRequest = static_cast<GetValueRequest*>(pWebRequest);
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

		FeatureWorksapce* pFeatureWorkspace = static_cast<FeatureWorksapce*>(pWorkspace);
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