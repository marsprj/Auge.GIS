#include "GetGmlObjectHandler.h"
#include "GetGmlObjectRequest.h"
#include "GetGmlObjectResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h" 

namespace auge
{
	GetGmlObjectHandler::GetGmlObjectHandler()
	{

	}

	GetGmlObjectHandler::~GetGmlObjectHandler()
	{

	}

	const char*	GetGmlObjectHandler::GetName()
	{
		return "GetGmlObject";
	}

	WebRequest*	GetGmlObjectHandler::ParseRequest(rude::CGI& cgi)
	{
		GetGmlObjectRequest* pRequest = new GetGmlObjectRequest();		
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		return pRequest;
	}

	WebRequest*	GetGmlObjectHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		GetGmlObjectRequest* pRequest = new GetGmlObjectRequest();		
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		pRequest->SetTypeName(mapName);
		return pRequest;
	}
	
	//WebRequest*	GetGmlObjectHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	GetGmlObjectRequest* pRequest = new GetGmlObjectRequest();
	//	if(!pRequest->Create(cgi))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//	}
	//	return pRequest;
	//}

	WebRequest*	GetGmlObjectHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GetGmlObjectRequest* pRequest = new GetGmlObjectRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* GetGmlObjectHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetGmlObjectHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GetGmlObjectRequest* pRequest = static_cast<GetGmlObjectRequest*>(pWebRequest);

		/*const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();

		typeName = pRequest->GetTypeName();
		pLayer = pMap->GetLayer(typeName);
		if(pLayer==NULL)
		{
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
		pLogger->Error(msg, __FILE__, __LINE__);

		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
		pExpResopnse->SetMessage(msg);
		return pExpResopnse;
		}

		if(pLayer->GetType()!=augeLayerFeature)
		{
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg, "%s is not a Feature Layer",typeName);
		pLogger->Error(msg, __FILE__, __LINE__);

		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
		pExpResopnse->SetMessage(msg);
		return pExpResopnse;
		}

		FeatureClass* pFeatureClass = NULL;
		FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
		pFeatureClass = pFLayer->GetGmlObjectClass();
		if(pFeatureClass==NULL)
		{
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
		pLogger->Error(msg, __FILE__, __LINE__);

		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
		pExpResopnse->SetMessage(msg);
		return pExpResopnse;
		}

		FeatureCursor *pCursor = pFeatureClass->Query();
		GetGmlObjectResponse *pResponse = new GetGmlObjectResponse(pRequest);
		pResponse->SetFeatureCursor(pCursor);
		return pResponse;*/

		return NULL;
	}

	//WebResponse* GetGmlObjectHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	GetGmlObjectRequest* pRequest = static_cast<GetGmlObjectRequest*>(pWebRequest);

	//	/*const char* typeName = NULL;
	//	Layer* pLayer = NULL;
	//	GLogger *pLogger = augeGetLoggerInstance();

	//	typeName = pRequest->GetTypeName();
	//	pLayer = pMap->GetLayer(typeName);
	//	if(pLayer==NULL)
	//	{
	//	char msg[AUGE_MSG_MAX];
	//	g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
	//	pLogger->Error(msg, __FILE__, __LINE__);

	//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//	pExpResopnse->SetMessage(msg);
	//	return pExpResopnse;
	//	}

	//	if(pLayer->GetType()!=augeLayerFeature)
	//	{
	//	char msg[AUGE_MSG_MAX];
	//	g_sprintf(msg, "%s is not a Feature Layer",typeName);
	//	pLogger->Error(msg, __FILE__, __LINE__);

	//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//	pExpResopnse->SetMessage(msg);
	//	return pExpResopnse;
	//	}

	//	FeatureClass* pFeatureClass = NULL;
	//	FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
	//	pFeatureClass = pFLayer->GetGmlObjectClass();
	//	if(pFeatureClass==NULL)
	//	{
	//	char msg[AUGE_MSG_MAX];
	//	g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
	//	pLogger->Error(msg, __FILE__, __LINE__);

	//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//	pExpResopnse->SetMessage(msg);
	//	return pExpResopnse;
	//	}

	//	FeatureCursor *pCursor = pFeatureClass->Query();
	//	GetGmlObjectResponse *pResponse = new GetGmlObjectResponse(pRequest);
	//	pResponse->SetFeatureCursor(pCursor);
	//	return pResponse;*/

	//	return NULL;
	//}
}