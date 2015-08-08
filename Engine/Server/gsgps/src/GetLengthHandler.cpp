#include "GetLengthHandler.h"
#include "GetLengthRequest.h"
#include "GetLengthResponse.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	GetLengthHandler::GetLengthHandler()
	{

	}

	GetLengthHandler::~GetLengthHandler()
	{

	}

	const char*	GetLengthHandler::GetName()
	{
		return "GetLength";
	}

	WebRequest*	GetLengthHandler::ParseRequest(rude::CGI& cgi)
	{
		GetLengthRequest* pRequest = new GetLengthRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->Info();
		return pRequest;
	}

	WebRequest*	GetLengthHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetLengthHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetLengthRequest* pRequest = new GetLengthRequest();
		return pRequest;
	}

	WebResponse* GetLengthHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GetLengthRequest* pRequest = static_cast<GetLengthRequest*>(pWebRequest);

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
			g_sprintf(msg, "WPS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* GetLengthHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GetLengthRequest* pRequest = static_cast<GetLengthRequest*>(pWebRequest);

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = GetFeatureClass(pRequest,pWebContext,pUser);
		if(pFeatureClass==NULL)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		FeatureCursor* pCursor = pFeatureClass->Query(pRequest->GetFilter());
		if(pCursor!=NULL) 
		{ 
			GetLengthResponse *pResponse = new GetLengthResponse(pRequest);
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

		return pWebResponse;
	}

	FeatureClass* GetLengthHandler::GetFeatureClass(GetLengthRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		FeatureClass* pFeatureClass = NULL;
		if(pWebRequest->GetSourceName())
		{
			pFeatureClass = GetFeatureClassBySource(pWebRequest, pWebContext, pUser);
		}
		else
		{
			pFeatureClass = GetFeatureClassByMap(pWebRequest, pWebContext, pUser);
		}
		return pFeatureClass;
	}

	FeatureClass* GetLengthHandler::GetFeatureClassByMap(GetLengthRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		FeatureClass* pFeatureClass = NULL;

		//GLogger *pLogger = augeGetLoggerInstance();
		//GetLengthRequest* pRequest = static_cast<GetLengthRequest*>(pWebRequest);
		//typeName = pRequest->GetTypeName();

		//const char* mapName = pRequest->GetMapName();
		//if(mapName==NULL)
		//{
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError("No Map is attached");
		//	return NULL;
		//}

		//CartoManager* pCartoManager = augeGetCartoManagerInstance();
		//Map *pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		//if(pMap==NULL)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "Cannot load map [%s]", mapName);
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	return NULL;
		//}

		//pLayer = pMap->GetLayer(typeName);
		//if(pLayer==NULL)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	return NULL;
		//}

		//if(pLayer->GetType()!=augeLayerFeature)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "%s is not a Feature Layer",typeName);
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	return NULL;
		//}

		//FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
		//pFeatureClass = pFLayer->GetFeatureClass();
		//if(pFeatureClass==NULL)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	return NULL;
		//}
		//pFeatureClass->AddRef();

		return pFeatureClass;
	}

	FeatureClass* GetLengthHandler::GetFeatureClassBySource(GetLengthRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		GetLengthRequest* pRequest = static_cast<GetLengthRequest*>(pWebRequest);
		typeName = pRequest->GetTypeName();

		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("No Source is attached");
			return NULL;
		}

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		Workspace* pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No DataSource Named [%s]", sourceName);
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
