#include "CentroidHandler.h"
#include "CentroidRequest.h"
#include "CentroidResponse.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	CentroidHandler::CentroidHandler()
	{

	}

	CentroidHandler::~CentroidHandler()
	{

	}

	const char*	CentroidHandler::GetName()
	{
		return "Centroid";
	}

	const char*	CentroidHandler::GetDescription()
	{
		return "ÖÐÐÄµã";
	}

	WebRequest*	CentroidHandler::ParseRequest(rude::CGI& cgi)
	{
		CentroidRequest* pRequest = new CentroidRequest();
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

	WebRequest*	CentroidHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CentroidHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		CentroidRequest* pRequest = new CentroidRequest();
		return pRequest;
	}

	WebResponse* CentroidHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		CentroidRequest* pRequest = static_cast<CentroidRequest*>(pWebRequest);

		return pWebResponse;
	}

	WebResponse* CentroidHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		WebResponse* pWebResponse = NULL;
		CentroidRequest* pRequest = static_cast<CentroidRequest*>(pWebRequest);

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		FeatureClass* pinFeatureClass = NULL;
		pinFeatureClass = GetFeatureClass(pRequest,pWebContext,pUser);
		if(pinFeatureClass==NULL)
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		const char* outputSourceName = pRequest->GetOutputSourceName();
		const char* outputTypeName = pRequest->GetOutputTypeName();
		if(outputTypeName==NULL && outputSourceName==NULL)
		{
			// return result xml document
			pWebResponse = Execute_2_Client(pinFeatureClass, pRequest, pWebContext);
		}
		else
		{
			pWebResponse = Execute_2_Datasource(pinFeatureClass, pRequest, pWebContext, pUser);
		}

		End();

		return pWebResponse;
	}

	FeatureClass* CentroidHandler::GetFeatureClass(CentroidRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		FeatureClass* pFeatureClass = NULL;
		if(pWebRequest->GetInputSourceName())
		{
			pFeatureClass = GetFeatureClassBySource(pWebRequest, pWebContext, pUser);
		}
		else
		{
			pFeatureClass = GetFeatureClassByMap(pWebRequest, pWebContext, pUser);
		}
		return pFeatureClass;
	}

	FeatureClass* CentroidHandler::GetFeatureClassByMap(CentroidRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		FeatureClass* pFeatureClass = NULL;
		
		//GLogger *pLogger = augeGetLoggerInstance();
		//CentroidRequest* pRequest = static_cast<CentroidRequest*>(pWebRequest);
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

	FeatureClass* CentroidHandler::GetFeatureClassBySource(CentroidRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		CentroidRequest* pRequest = static_cast<CentroidRequest*>(pWebRequest);
		typeName = pRequest->GetInputTypeName();

		const char* sourceName = pRequest->GetInputSourceName();
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

	WebResponse* CentroidHandler::Execute_2_Client(FeatureClass* pinFeatureClass, CentroidRequest* pRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		FeatureCursor* pCursor = pinFeatureClass->Query(pRequest->GetFilter());
		if(pCursor!=NULL) 
		{ 
			CentroidResponse *pResponse = new CentroidResponse(pRequest);
			pResponse->SetWebContenxt(pWebContext);
			pResponse->SetFeatureCursor(pCursor);
			pWebResponse = pResponse;
		}
		else
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		return pWebResponse;
	}

	WebResponse* CentroidHandler::Execute_2_Datasource(FeatureClass* pinFeatureClass, CentroidRequest* pRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		const char* outputSourceName = pRequest->GetOutputSourceName();
		const char* outputTypeName = pRequest->GetOutputTypeName();

		Workspace* pWorkspace = NULL;
		FeatureWorkspace* pFeatureWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnManager->NewWorkspace(pUser->GetID(), outputSourceName);
		if(pWorkspace==NULL)
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		else
		{
			pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
			if(pFeatureWorkspace==NULL)
			{
				char msg[AUGE_MSG_MAX];
				memset(msg, 0, AUGE_MSG_MAX);
				g_sprintf(msg, "[%s] is not a Feature Datasource", outputSourceName);
				pError->SetError(msg);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(pError->GetLastError());
				pWebResponse = pExpResponse;
			}
			else
			{
				FeatureClass* poutFeatureClass = NULL;
				poutFeatureClass = pFeatureWorkspace->OpenFeatureClass( outputTypeName);
				if(poutFeatureClass!=NULL)
				{
					pFeatureWorkspace->Release();

					char msg[AUGE_MSG_MAX];
					memset(msg, 0, AUGE_MSG_MAX);
					g_sprintf(msg, "outputType [%s] already exists", outputTypeName);
					pError->SetError(msg);
					WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
					pExpResponse->SetMessage(pError->GetLastError());
					pWebResponse = pExpResponse;
				}
				else
				{
					poutFeatureClass = CreateOutputFeatureClass(pFeatureWorkspace, outputTypeName, pinFeatureClass->GetSRID());
					if(poutFeatureClass==NULL)
					{
						pFeatureWorkspace->Release();

						pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
						WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
						pExpResponse->SetMessage(pError->GetLastError());
						pWebResponse = pExpResponse;
					}
					else
					{
						FeatureCursor* pCursor = pinFeatureClass->Query(pRequest->GetFilter());
						if(pCursor==NULL) 
						{
							poutFeatureClass->Release();
							pFeatureWorkspace->Release();

							pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
							WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
							pExpResponse->SetMessage(pError->GetLastError());
							pWebResponse = pExpResponse;
						}
						else
						{	
							double x=0.0, y=0.0;
							char str[AUGE_PATH_MAX];
							WKBPoint* pWKBPoint = NULL;
							Geometry* pGeometry = NULL;
							Geometry* pCentroid = NULL;
							GValue*   pidValue = NULL;
							GValue*   pgeoValue = NULL;
							Feature* poutFeature = NULL;
							GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
							
							//poutFeature = poutFeatureClass->NewFeature();	
							//pCentroid = pGeometryFactory->CreateGeometryFromWKT("POINT(0 0)");	
							//pidValue = new GValue(0);
							//pgeoValue = new GValue(pCentroid);
							//pWKBPoint = (WKBPoint*)pCentroid->AsBinary();
							//poutFeature->SetValue("id", pidValue);
							//poutFeature->SetValue("shape", pgeoValue);

							FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();

							Feature* pFeature = NULL;
							while((pFeature=pCursor->NextFeature())!=NULL)
							{	
								pGeometry =  pFeature->GetGeometry();
								if(pGeometry!=NULL)
								{
									pGeometry->Centroid(x, y);
									g_snprintf(str, AUGE_PATH_MAX, "POINT(%f %f)", x, y);
									
									//pCentroid = pGeometryFactory->CreateGeometryFromWKT(str);									
									//pidValue->SetInt(pFeature->GetFID());
									//pgeoValue->SetValue(pCentroid);

									poutFeature = poutFeatureClass->NewFeature();	
									pCentroid = pGeometryFactory->CreateGeometryFromWKT(str);	
									pidValue = new GValue(pFeature->GetFID());
									pgeoValue = new GValue(pCentroid);
									poutFeature->SetValue("id", pidValue);
									poutFeature->SetValue("shape", pgeoValue);

									cmd->Insert(poutFeature);

									poutFeature->Release();
								}
							}

							//poutFeature->Release();
							poutFeatureClass->Refresh();

							cmd->Release();
							pCursor->Release();
							poutFeatureClass->Release();
							pFeatureWorkspace->Release();

							WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
							pSusResponse->SetRequest(pRequest->GetRequest());
							pWebResponse = pSusResponse;
						}
					}
				}
			}
		}

		return pWebResponse;
	}

	FeatureClass* CentroidHandler::CreateOutputFeatureClass(FeatureWorkspace* pFeatureWorkspace, const char* className, g_uint srid)
	{
		GFields	*pFields = NULL;
		FieldFactory* pFieldFactory = augeGetFieldFactoryInstance();
		pFields = pFieldFactory->CreateFields();

		GField	 *pField  = NULL;
		GField_2 *pField_2= NULL;
		GeometryDef* pGeometryDef = NULL;
		GeometryDef_2* pGeometryDef_2 = NULL;

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("id");
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("shape");
		pField_2->SetType(augeFieldTypeGeometry);
		pGeometryDef_2 = pField->GetGeometryDef()->GetGeometryDef_2();
		pGeometryDef_2->SetGeometryType(augeGTPoint);
		pGeometryDef_2->SetDimension(2);
		pGeometryDef_2->SetSRID(srid);
		pFields->Add(pField);

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pFeatureWorkspace->CreateFeatureClass(className, pFields);
		pFields->Release();

		return pFeatureClass;
	}
}
