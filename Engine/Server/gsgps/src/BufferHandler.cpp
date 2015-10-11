#include "BufferHandler.h"
#include "BufferRequest.h"
#include "BufferResponse.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeCarto.h"
#include "AugeUser.h"

#include "geos_c.h"

namespace auge
{
	BufferHandler::BufferHandler()
	{
		initGEOS(NULL, NULL);
	}

	BufferHandler::~BufferHandler()
	{
		finishGEOS();
	}

	const char*	BufferHandler::GetName()
	{
		return "Buffer";
	}

	const char*	BufferHandler::GetDescription()
	{
		return "缓冲区";
	}

	WebRequest*	BufferHandler::ParseRequest(rude::CGI& cgi)
	{
		BufferRequest* pRequest = new BufferRequest();
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

	WebRequest*	BufferHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	BufferHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		BufferRequest* pRequest = new BufferRequest();
		return pRequest;
	}

	WebResponse* BufferHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		BufferRequest* pRequest = static_cast<BufferRequest*>(pWebRequest);

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

	WebResponse* BufferHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		Begin(pUser);

		WebResponse* pWebResponse = NULL;
		BufferRequest* pRequest = static_cast<BufferRequest*>(pWebRequest);

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

	FeatureClass* BufferHandler::GetFeatureClass(BufferRequest* pWebRequest, WebContext* pWebContext, User* pUser)
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

	FeatureClass* BufferHandler::GetFeatureClassByMap(BufferRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		FeatureClass* pFeatureClass = NULL;

		//GLogger *pLogger = augeGetLoggerInstance();
		//BufferRequest* pRequest = static_cast<BufferRequest*>(pWebRequest);
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

	FeatureClass* BufferHandler::GetFeatureClassBySource(BufferRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		BufferRequest* pRequest = static_cast<BufferRequest*>(pWebRequest);
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

	FeatureClass* BufferHandler::CreateOutputFeatureClass(FeatureWorkspace* pFeatureWorkspace, const char* className, g_uint srid)
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
		pGeometryDef_2->SetGeometryType(augeGTMultiPolygon);
		pGeometryDef_2->SetDimension(2);
		pGeometryDef_2->SetSRID(srid);
		pFields->Add(pField);

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pFeatureWorkspace->CreateFeatureClass(className, pFields);
		pFields->Release();

		return pFeatureClass;
	}

	Geometry* BufferHandler::CreateBuffer(Feature* pFeature, BufferRequest* pRequest)
	{
		GeometryUtil* pGeometryUtil = augeGetGeometryUtilInstance();
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		double distance = 0.0;
		const char* distance_field = pRequest->GetDistanceField();
		if(distance_field!=NULL)
		{
			distance = GetDistance(pFeature->GetValue(distance_field));
		}
		else
		{
			distance = pRequest->GetDistance();
		}
		
		Geometry* pGeometry = NULL;
		Geometry* pBuffer = NULL;
		if(distance>0)
		{
			pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				g_uchar* wkb = pGeometry->AsBinary();
				g_uint   wkb_size = pGeometryUtil->GetWKBSize(wkb);

				GEOSGeometry* pgeosGeomtrySrc = GEOSGeomFromWKB_buf(wkb, wkb_size);
				if(pgeosGeomtrySrc!=NULL)
				{
					GEOSGeometry* pgeosBuffer = GEOSBuffer(pgeosGeomtrySrc, distance, 5);
					if(pgeosBuffer!=NULL)
					{	
						size_t wkbSize = 0;
						unsigned char* pwkbBuffer = NULL;
						pwkbBuffer = GEOSGeomToWKB_buf(pgeosBuffer, &wkbSize);
						if(pwkbBuffer!=NULL||wkbSize>0)
						{
							pBuffer = pGeometryFactory->CreateGeometryFromWKB(pwkbBuffer, true);
							////////////////////////////////////////////////////////////////
							//这里有内存泄露，需要名曲pwkbBuffer由谁释放
							//free(pwkbBuffer);
							////////////////////////////////////////////////////////////////

						}
						GEOSGeom_destroy(pgeosBuffer);
					}
					GEOSGeom_destroy(pgeosGeomtrySrc);
				}
			}
		}
		return pBuffer;
	}

	double BufferHandler::GetDistance(GValue* pValue)
	{
		if(pValue==NULL)
		{
			return -1.0f;
		}

		double distance = -1.0f;
		augeValueType type = pValue->GetType();
		switch(type)
		{
		case augeValueTypeInt:
			distance = pValue->GetInt();
			break;
		case augeValueTypeLong:
			distance = pValue->GetLong();
			break;
		case augeValueTypeInt64:
			distance = pValue->GetInt64();
			break;
		case augeValueTypeFloat:
			distance = pValue->GetFloat();
			break;
		case augeValueTypeDouble:
			distance = pValue->GetDouble();
			break;
		}
		return distance;
	}

	WebResponse* BufferHandler::Execute_2_Client(FeatureClass* pinFeatureClass, BufferRequest* pRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		FeatureCursor* pCursor = pinFeatureClass->Query(pRequest->GetFilter());
		if(pCursor!=NULL) 
		{ 
			BufferResponse *pResponse = new BufferResponse(pRequest);
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

	WebResponse* BufferHandler::Execute_2_Datasource(FeatureClass* pinFeatureClass, BufferRequest* pRequest, WebContext* pWebContext, User* pUser)
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
							Feature* poutFeature = NULL;
							FeatureInsertCommand* cmd = poutFeatureClass->CreateInsertCommand();

							Feature* pFeature = NULL;
							while((pFeature=pCursor->NextFeature())!=NULL)
							{	
								Geometry *pBuffer = CreateBuffer(pFeature, pRequest);
								if(pBuffer!=NULL)
								{
									poutFeature = poutFeatureClass->NewFeature();
									poutFeature->SetValue("id", new GValue(pFeature->GetFID()));
									poutFeature->SetValue("shape", new GValue(pBuffer));

									cmd->Insert(poutFeature);

									poutFeature->Release();
								}
								pFeature->Release();
							}
							cmd->Commit();
							cmd->Release();

							poutFeatureClass->Refresh();
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
}
