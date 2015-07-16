#include "TransactionHandler.h"
#include "TransactionRequest.h"
#include "TransactionResponse.h"
#include "WFeatureEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeFeature.h" 
#include "AugeField.h"
#include "AugeUser.h"

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
		return NULL;
	}
	
	WebRequest*	TransactionHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return NULL;
	}

	WebRequest*	TransactionHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		TransactionRequest* pRequest = new TransactionRequest();
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

	//WebRequest*	TransactionHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	//{
	//	TransactionRequest* pRequest = new TransactionRequest();
	//	if(!pRequest->Create(pxDoc))
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
	//		pRequest->Release();
	//		pRequest = NULL;
	//	}
	//	return pRequest;
	//}

	WebResponse* TransactionHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* TransactionHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse *pWebResponse = NULL;
		WFeatureRequest* pRequest = static_cast<WFeatureRequest*>(pWebRequest);
		if(pRequest->IsValidSource())
		{
			pWebResponse = ExecuteBySource(pWebRequest, pWebContext, pUser);
		}
		else
		{
			pWebResponse = ExecuteByMap(pWebRequest, pWebContext, pUser);
		}
		return pWebResponse;
	}

	WebResponse* TransactionHandler::ExecuteByMap(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger *pLogger = augeGetLoggerInstance();

		TransactionRequest* pRequest = static_cast<TransactionRequest*>(pWebRequest);

		const char* mapName = pRequest->GetMapName();
		if(mapName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No Map is attached");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map *pMap = pCartoManager->LoadMap(pUser->GetID(), mapName);
		if(pMap==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "Cannot load map [%s]", mapName);
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* typeName = NULL;
		Layer* pLayer = NULL;

		XDocument* pxDoc = NULL;
		pxDoc = pRequest->GetXmlDoc();
		if(pxDoc==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("transaction xml parse error");
			return pExpResponse;
		}

		XNodeSet	*pxNodeSet = NULL;
		XElement	*pxRoot = pxDoc->GetRootNode();
		pxNodeSet = pxRoot->GetChildren("Insert");
		g_uint num_insert = Insert(pxNodeSet, pWebContext, pMap);
		pxNodeSet->Release();

		pxNodeSet = pxRoot->GetChildren("Update");
		g_uint num_update = Update(pxNodeSet, pWebContext, pMap);
		pxNodeSet->Release();

		pxNodeSet = pxRoot->GetChildren("Delete");
		g_uint num_delete = Delete(pxNodeSet, pWebContext, pMap);
		pxNodeSet->Release();

		TransactionResponse *pResponse = new TransactionResponse(pRequest);	
		pResponse->SetInsertCount(num_insert);
		pResponse->SetUpdateCount(num_update);
		pResponse->SetDeleteCount(num_delete);

		pMap->Release();

		return pResponse;
	}

	WebResponse* TransactionHandler::ExecuteBySource(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger *pLogger = augeGetLoggerInstance();

		TransactionRequest* pRequest = static_cast<TransactionRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "No DataSource is Defined");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		FeatureWorksapce* pWorkspace = dynamic_cast<FeatureWorksapce*>(pConnManager->GetWorkspace(sourceName));
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No DataSource Named [%s]", sourceName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* typeName = NULL;
		Layer* pLayer = NULL;

		XDocument* pxDoc = NULL;
		pxDoc = pRequest->GetXmlDoc();
		if(pxDoc==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("transaction xml parse error");
			return pExpResponse;
		}

		XNodeSet	*pxNodeSet = NULL;
		XElement	*pxRoot = pxDoc->GetRootNode();
		pxNodeSet = pxRoot->GetChildren("Insert");
		g_uint num_insert = Insert(pxNodeSet, pWebContext, pWorkspace);
		pxNodeSet->Release();

		pxNodeSet = pxRoot->GetChildren("Update");
		g_uint num_update = Update(pxNodeSet, pWebContext, pWorkspace);
		pxNodeSet->Release();

		pxNodeSet = pxRoot->GetChildren("Delete");
		g_uint num_delete = Delete(pxNodeSet, pWebContext, pWorkspace);
		pxNodeSet->Release();

		TransactionResponse *pResponse = new TransactionResponse(pRequest);	
		pResponse->SetInsertCount(num_insert);
		pResponse->SetUpdateCount(num_update);
		pResponse->SetDeleteCount(num_delete);

		return pResponse;
	}

	//WebResponse* TransactionHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	GLogger *pLogger = augeGetLoggerInstance();

	//	//if(pMap==NULL)
	//	//{
	//	//	char msg[AUGE_MSG_MAX];
	//	//	g_sprintf(msg, "Service is empty");
	//	//	pLogger->Error(msg, __FILE__, __LINE__);

	//	//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//	//	pExpResopnse->SetMessage(msg);
	//	//	return pExpResopnse;
	//	//}

	//	TransactionRequest* pRequest = static_cast<TransactionRequest*>(pWebRequest);

	//	const char* typeName = NULL;
	//	Layer* pLayer = NULL;
	//	

	//	XDocument* pxDoc = NULL;
	//	pxDoc = pRequest->GetXmlDoc();
	//	if(pxDoc==NULL)
	//	{
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage("transaction xml parse error");
	//		return pExpResponse;
	//	}

	//	XNodeSet	*pxNodeSet = NULL;
	//	XElement	*pxRoot = pxDoc->GetRootNode();
	//	pxNodeSet = pxRoot->GetChildren("Insert");
	//	g_uint num_insert = Insert(pxNodeSet, pWebContext, pMap);
	//	pxNodeSet->Release();

	//	pxNodeSet = pxRoot->GetChildren("Update");
	//	g_uint num_update = Update(pxNodeSet, pWebContext, pMap);
	//	pxNodeSet->Release();

	//	pxNodeSet = pxRoot->GetChildren("Delete");
	//	g_uint num_delete = Delete(pxNodeSet, pWebContext, pMap);
	//	pxNodeSet->Release();
	//	
	//	TransactionResponse *pResponse = new TransactionResponse(pRequest);	
	//	pResponse->SetInsertCount(num_insert);
	//	pResponse->SetUpdateCount(num_update);
	//	pResponse->SetDeleteCount(num_delete);
	//	return pResponse;
	//}

	g_uint TransactionHandler::Insert(XNodeSet* pxNodeSet, WebContext* pWebContext, Map* pMap)
	{
		g_uint count = 0;
		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next()))
		{
			if(Insert(pxNode, pWebContext, pMap))
			{
				count++;
			}
		}
		return count;
	}

	bool TransactionHandler::Insert(XNode* pxInsert, WebContext* pWebContext, Map* pMap)
	{	
		XNode* pxLayer = pxInsert->GetFirstChild();
		if(pxLayer==NULL)
		{
			return false;
		}
		const char* name = pxLayer->GetName();
		Layer *pLayer = pMap->GetLayer(name);
		if(pLayer==NULL)
		{
			return false;
		}

		augeLayerType ltype = pLayer->GetType();
		if(ltype != augeLayerFeature)
		{
			return false;
		}

		FeatureLayer* pFeatureLayer = NULL;
		pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
		
		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pFeatureLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			return false;
		}

		Feature* pFeature = pFeatureClass->NewFeature();
		if(pFeature==NULL)
		{
			return false;
		}

		GField	*pField  = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		const char* fname = NULL;

		GValue* pValue = NULL;
		XNode* pxNode = NULL;
		XNodeSet* pxNodeSet = pxLayer->GetChildren();
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			fname = pxNode->GetName();
			pField = pFields->GetField(fname);
			if(pField!=NULL)
			{
				pValue = CreateValue(pxNode, pField);
				if(pValue!=NULL)
				{
					pFeature->SetValue(fname, pValue);
				}
			}
		}
		pxNodeSet->Release();

		FeatureInsertCommand* cmd = pFeatureClass->CreateInsertCommand();
		RESULTCODE rc = cmd->Insert(pFeature);
		
		AUGE_SAFE_RELEASE(pFeature);
		AUGE_SAFE_RELEASE(cmd);

		return !rc;
	}

	GValue* TransactionHandler::CreateValue(XNode* pxNode, GField* pField)
	{
		GValue* pValue = NULL;
		const char* str= NULL;
		switch(pField->GetType())
		{					 
		case augeFieldTypeShort:
			{
				str = pxNode->GetContent();
				pValue = new GValue((short)atoi(str));
			}
			break;
		case augeFieldTypeInt:
			{
				str = pxNode->GetContent();
				pValue = new GValue((int)atoi(str));
			}
			break;
		case augeFieldTypeLong:
			{
				str = pxNode->GetContent();
				pValue = new GValue((long)atoi(str));
			}
			break;
		case augeFieldTypeInt64:
			{
				str = pxNode->GetContent();
				pValue = new GValue((int64)atoi(str));
			}
			break;
		case augeFieldTypeFloat:
			{
				str = pxNode->GetContent();
				pValue = new GValue((float)atof(str));
			}
			break;
		case augeFieldTypeDouble:
			{
				str = pxNode->GetContent();
				pValue = new GValue((double)atof(str));
			}
			break;
		case augeFieldTypeChar:			 
			{
				str = pxNode->GetContent();
				pValue = new GValue(str[0]);
			}
			break;
		case augeFieldTypeString:
			{
				pValue = new GValue(pxNode->GetContent());
			}
			break;
		case augeFieldTypeTime:	
			{

			}
			break;
		case augeFieldTypeBool:			 
			{

			}
			break;
		case augeFieldTypeBLOB:			 
			{

			}
			break;
		case augeFieldTypeGeometry:
			{
				/*Geometry *pGeometry = pFeature->GetGeometry();
				if(pGeometry!=NULL)
				{
					const char* wkt = pGeometry->AsText(true);
					if(wkt!=NULL)
					{
						g_snprintf(str, AUGE_BUFFER_MAX,"%d",srid);

						fields.append(fname);
						values.append("st_geomfromtext(");
						values.append("'");
						values.append(wkt);
						values.append("',");
						values.append(str);
						values.append(")");
					}
				}*/
			}
			break;
		}

		return pValue;
	}

	g_uint TransactionHandler::Update(XNodeSet* pxNodeSet, WebContext* pWebContext, Map* pMap)
	{
		g_uint count = 0;
		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next()))
		{
			//if(Insert(pxNode, pWebContext, pMap))
			//{
			//	count++;
			//}
		}
		return count;
	}

	g_uint TransactionHandler::Delete(XNodeSet* pxNodeSet, WebContext* pWebContext, Map* pMap)
	{
		g_uint count = 0;
		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next()))
		{
			count += Delete(pxNode, pWebContext, pMap);
		}
		return count;
	}

	g_int TransactionHandler::Delete(XNode* pxDelete, WebContext* pWebContext, Map* pMap)
	{
		int count = 0;

		XAttribute* pxAttr = ((XElement*)pxDelete)->GetAttribute("name");
		if(pxAttr==NULL)
		{
			return false;
		}
		const char* name = pxAttr->GetValue();
		Layer *pLayer = pMap->GetLayer(name);
		if(pLayer==NULL)
		{
			return false;
		}

		augeLayerType ltype = pLayer->GetType();
		if(ltype != augeLayerFeature)
		{
			return false;
		}

		FeatureLayer* pFeatureLayer = NULL;
		pFeatureLayer = static_cast<FeatureLayer*>(pLayer);

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pFeatureLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			return false;
		}

		GFilter* pFilter = NULL;
		XNode* pxFilter = pxDelete->GetFirstChild("Filter");
		if(pxFilter!=NULL)
		{
			FilterFactory *factory = augeGetFilterFactoryInstance();
			FilterReader  *reader  = factory->CreateFilerReader(pFeatureClass->GetFields());
			pFilter = reader->Read((XElement*)pxFilter);
		}

		count = pFeatureClass->RemoveFeature(pFilter);
		if(pFilter!=NULL)
		{
			pFilter->Release();
			pFilter = NULL;
		}

		return count;
	}

	//////////////////////////////////////////////////////////////////////////
	// Exectue By DataSource
	//////////////////////////////////////////////////////////////////////////
	g_uint TransactionHandler::Insert(XNodeSet* pxNodeSet, WebContext* pWebContext, FeatureWorksapce* pWorkspace)
	{
		g_uint count = 0;
		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next()))
		{
			if(Insert(pxNode, pWebContext, pWorkspace))
			{
				count++;
			}
		}
		return count;
	}

	g_uint TransactionHandler::Update(XNodeSet* pxNodeSet, WebContext* pWebContext, FeatureWorksapce* pWorkspace)
	{
		g_uint count = 0;
		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next()))
		{
			//if(Insert(pxNode, pWebContext, pMap))
			//{
			//	count++;
			//}
		}
		return count;
	}

	g_uint TransactionHandler::Delete(XNodeSet* pxNodeSet, WebContext* pWebContext, FeatureWorksapce* pWorkspace)
	{
		g_uint count = 0;
		XNode *pxNode = NULL;
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next()))
		{
			count += Delete(pxNode, pWebContext, pWorkspace);
		}
		return count;
	}

	g_int TransactionHandler::Delete(XNode* pxDelete, WebContext* pWebContext, FeatureWorksapce* pWorkspace)
	{
		int count = 0;

		XAttribute* pxAttr = ((XElement*)pxDelete)->GetAttribute("name");
		if(pxAttr==NULL)
		{
			return 0;
		}
		const char* name = pxAttr->GetValue();
		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pWorkspace->OpenFeatureClass(name);
		if(pFeatureClass==NULL)
		{
			return 0;
		}

		GFilter* pFilter = NULL;
		XNode* pxFilter = pxDelete->GetFirstChild("Filter");
		if(pxFilter!=NULL)
		{
			FilterFactory *factory = augeGetFilterFactoryInstance();
			FilterReader  *reader  = factory->CreateFilerReader(pFeatureClass->GetFields());
			pFilter = reader->Read((XElement*)pxFilter);
		}

		count = pFeatureClass->RemoveFeature(pFilter);
		if(pFilter!=NULL)
		{
			pFilter->Release();
			pFilter = NULL;
		}
		pFeatureClass->Release();

		return count;
	}

	bool TransactionHandler::Insert(XNode* pxInsert, WebContext* pWebContext, FeatureWorksapce* pWorkspace)
	{	
		XNode* pxType = pxInsert->GetFirstChild();
		if(pxType==NULL)
		{
			return false;
		}
		const char* name = pxType->GetName();
		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pWorkspace->OpenFeatureClass(name);
		if(pFeatureClass==NULL)
		{
			return false;
		}

		Feature* pFeature = pFeatureClass->NewFeature();
		if(pFeature==NULL)
		{
			return false;
		}

		GField	*pField  = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		const char* fname = NULL;

		GValue* pValue = NULL;
		XNode* pxNode = NULL;
		XNodeSet* pxNodeSet = pxType->GetChildren();
		pxNodeSet->Reset();
		while((pxNode=pxNodeSet->Next())!=NULL)
		{
			fname = pxNode->GetName();
			pField = pFields->GetField(fname);
			if(pField!=NULL)
			{
				pValue = CreateValue(pxNode, pField);
				if(pValue!=NULL)
				{
					pFeature->SetValue(fname, pValue);
				}
			}
		}
		pxNodeSet->Release();

		FeatureInsertCommand* cmd = pFeatureClass->CreateInsertCommand();
		RESULTCODE rc = cmd->Insert(pFeature);

		AUGE_SAFE_RELEASE(pFeature);
		AUGE_SAFE_RELEASE(cmd);
		AUGE_SAFE_RELEASE(pFeatureClass);

		return !rc;
	}
}