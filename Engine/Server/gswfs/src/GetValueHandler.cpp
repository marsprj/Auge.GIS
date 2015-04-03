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

	WebRequest*	GetValueHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		GetValueRequest* pRequest = new GetValueRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetValueHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		GetValueResponse* pWebResponse = NULL;
		GetValueRequest*  pRequest = static_cast<GetValueRequest*>(pWebRequest);
		GLogger *pLogger = augeGetLoggerInstance();

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

		const char* typeName = pRequest->GetTypeName();

		Layer* pLayer = pMap->GetLayer(typeName);
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
		
		EnumValue* pValues = pFeatureClass->GetUniqueValue(field, pRequest->GetOrder());
		pWebResponse = new GetValueResponse(pRequest);
		pWebResponse->SetValues(pValues);
		
		return pWebResponse;
	}
}