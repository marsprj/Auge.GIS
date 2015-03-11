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
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetFeatureHandler::ParseRequest(XDocument* pxDoc)
	{
		GetFeatureRequest* pRequest = new GetFeatureRequest();
		if(!pRequest->Create(pxDoc))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* GetFeatureHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);

		const char* typeName = NULL;
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
		pFeatureClass = pFLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		FeatureCursor *pCursor = NULL;
		GFilter* pFilter = pRequest->GetFilter();
		if(pFilter!=NULL)
		{
			pCursor = pFeatureClass->Query(pFilter);
		}
		else
		{
			GEnvelope& bbox = pRequest->GetBBox();
			if(bbox.IsValid())
			{
				pCursor = pFeatureClass->Query(bbox);
			}
		}
		
		GetFeatureResponse *pResponse = new GetFeatureResponse(pRequest);
		pResponse->SetFeatureCursor(pCursor);
		return pResponse;
	}
}