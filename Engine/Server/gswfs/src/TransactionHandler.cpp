#include "TransactionHandler.h"
#include "TransactionRequest.h"
#include "TransactionResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h" 

namespace auge
{
	TransactionHandler::TransactionHandler()
	{

	}

	TransactionHandler::~TransactionHandler()
	{

	}

	const char*	TransactionHandler::GetName()
	{
		return "Transaction";
	}
	
	WebRequest*	TransactionHandler::ParseRequest(rude::CGI& cgi)
	{
		TransactionRequest* pRequest = new TransactionRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	TransactionHandler::ParseRequest(XDocument* pxDoc)
	{
		TransactionRequest* pRequest = new TransactionRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* TransactionHandler::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* TransactionHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		TransactionRequest* pRequest = static_cast<TransactionRequest*>(pWebRequest);

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

		FeatureCursor *pCursor = pFeatureClass->Query();
		TransactionResponse *pResponse = new TransactionResponse(pRequest);
		pResponse->SetFeatureCursor(pCursor);
		return pResponse;
	}
}