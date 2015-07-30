#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeCarto.h"

namespace auge
{
	GetDataSetResponse::GetDataSetResponse(GetDataSetRequest* pRequest)
	{
		m_pDataSet = NULL;
		m_pDataSets = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pWebContext = NULL;
	}

	GetDataSetResponse::~GetDataSetResponse()
	{
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		if(m_pDataSet!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSet);
		}
	}

	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		if(m_pDataSet!=NULL)
		{
			Write(pWriter, m_pDataSet);
		}
		else
		{
			Write(pWriter, m_pDataSets);
		}
	}

	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter, EnumDataSet* pDataSets)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("DataSets", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");

		XElement	*pxDataSet = NULL;
		XElement	*pxService  = NULL;
		pDataSets->Reset();
		DataSet* pDataSet = NULL;
		while((pDataSet=pDataSets->Next())!=NULL)
		{ 
			const char* datasetName = pDataSet->GetName();
			pxDataSet = pxRoot->AddChild("DataSet");				
			XElement* pxElement = pxDataSet->AddChild("Name");
			pxElement->SetChildText(datasetName, true);
			pxElement = pxDataSet->AddChild("Type");
			pxElement->SetChildText(augeGetDataSetType(pDataSet->GetType()));
			
			switch(pDataSet->GetType())
			{
			case augeDataSetFeature:
				{
					AddFeatureClassNode(pxDataSet, static_cast<FeatureClass*>(pDataSet));

					//	FeatureClass* pFeatureClass = static_cast<FeatureClass*>(pDataSet);
					//GField* pField = pFeatureClass->GetFields()->GetGeometryField();
					//XElement* pxGeometry = pxDataSet->AddChild("Geometry");
					//if(pField!=NULL)
					//{
					//	GeometryDef* pGeometryDef = pField->GetGeometryDef();
					//	if(pGeometryDef!=NULL)
					//	{
					//		augeGeometryType type = pGeometryDef->GeometryType();
					//		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
					//		const char* geomType = pGeometryFactory->Encode(type);

					//		XElement* pxElem = pxGeometry->AddChild("Type");
					//		pxElem->AddChildText(geomType);

					//		char str[AUGE_NAME_MAX] = {0};
					//		g_sprintf(str, "%d", pGeometryDef->GetSRID());
					//		pxElem = pxGeometry->AddChild("SRID");
					//		pxElem->AddChildText(str);
					//	}
					//}
				}
				break;
			case augeDataSetRaster:
				{

				}
				break;
			}
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	
	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter, DataSet* pDataSet)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxDataSet = pxDoc->CreateRootNode("DataSet", NULL, NULL);
		XElement	*pxService  = NULL;
				
		XElement* pxElement = pxDataSet->AddChild("Name");
		pxElement->SetChildText(pDataSet->GetName());
		pxElement = pxDataSet->AddChild("Type");
		pxElement->SetChildText(augeGetDataSetType(pDataSet->GetType()));

		switch(pDataSet->GetType())
		{
		case augeDataSetFeature:
			{
				AddFeatureClassNode(pxDataSet, static_cast<FeatureClass*>(pDataSet));
				//FeatureClass* pFeatureClass = static_cast<FeatureClass*>(pDataSet);
				//GField* pField = pFeatureClass->GetFields()->GetGeometryField();
				//XElement* pxGeometry = pxDataSet->AddChild("Geometry");
				//if(pField!=NULL)
				//{
				//	GeometryDef* pGeometryDef = pField->GetGeometryDef();
				//	if(pGeometryDef!=NULL)
				//	{
				//		augeGeometryType type = pGeometryDef->GeometryType();
				//		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				//		const char* geomType = pGeometryFactory->Encode(type);

				//		XElement* pxElem = pxGeometry->AddChild("Type");
				//		pxElem->AddChildText(geomType);

				//		char str[AUGE_NAME_MAX] = {0};
				//		g_sprintf(str, "%d", pGeometryDef->GetSRID());
				//		pxElem = pxGeometry->AddChild("SRID");
				//		pxElem->AddChildText(str);
				//	}
				//}
			}
			break;
		case augeDataSetRaster:
			{

			}
			break;
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetDataSetResponse::AddFeatureClassNode(XElement* pxClass, FeatureClass* pFeatureClass)
	{
		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		XElement* pxGeometry = pxClass->AddChild("Geometry");
		if(pField!=NULL)
		{
			GeometryDef* pGeometryDef = pField->GetGeometryDef();
			if(pGeometryDef!=NULL)
			{
				augeGeometryType type = pGeometryDef->GeometryType();
				GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				const char* geomType = pGeometryFactory->Encode(type);

				XElement* pxElem = pxGeometry->AddChild("Type");
				pxElem->AddChildText(geomType);
				 
				char str[AUGE_NAME_MAX] = {0};
				g_sprintf(str, "%d", pGeometryDef->GetSRID());
				pxElem = pxGeometry->AddChild("SRID");
				pxElem->AddChildText(str);
			}
		}

		AddThumbnailNode(pxClass, pFeatureClass);
	}

	void GetDataSetResponse::AddThumbnailNode(XElement* pxClass, FeatureClass* pFeatureClass)
	{
		char img_local_path[AUGE_PATH_MAX] = {0};
		memset(img_local_path, 0, AUGE_PATH_MAX);		
		auge_make_path(img_local_path, NULL, m_pWebContext->GetCacheMapPath(), pFeatureClass->GetUUID(), "png");
		if(g_access(img_local_path, 4))
		{
			DrawThumbnail(pFeatureClass, img_local_path);
		}

		char thumbnail[AUGE_PATH_MAX];
		memset(thumbnail,0,AUGE_PATH_MAX);
		auge_make_path(thumbnail, NULL, NULL, pFeatureClass->GetUUID(), "png");
		char xlink[AUGE_PATH_MAX];
		memset(xlink,0,AUGE_PATH_MAX);
		g_snprintf(xlink, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), thumbnail);
		XElement* pxThumbnail = pxClass->AddChild("Thumbnail", NULL);
		pxThumbnail->SetAttribute("xlink",xlink,NULL);
	}

	void GetDataSetResponse::DrawThumbnail(FeatureClass* pFeatureClass, const char* img_local_path)
	{
		g_uint width  = AUGE_THUMBNAIL_WIDTH;
		g_uint height = AUGE_THUMBNAIL_HEIGHT;


		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		Style* pStyle = NULL;
		FeatureLayer* pFeatureLayer = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		StyleFactory* pStyleFactory = augeGetStyleFactoryInstance();

		pFeatureLayer = pCartoFactory->CreateFeatureLayer();
		pFeatureLayer->SetFeatureClass(pFeatureClass);

		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		augeGeometryType type = pField->GetGeometryDef()->GeometryType();
		pStyle = pStyleFactory->CreateFeatureStyle(type);

		GColor bgColor(255,255,255,255);
		pCanvas = pCartoFactory->CreateCanvas2D(width, height);
		pCanvas->DrawBackground(bgColor);

		GEnvelope extent = pFeatureClass->GetExtent();
		pCanvas->SetViewer(extent);
		pCanvas->DrawLayer(pFeatureLayer, pStyle);
		pCanvas->Save(img_local_path);
		pCanvas->Release();;
	}


	void GetDataSetResponse::SetDataSets(EnumDataSet* pDataSets)
	{
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		m_pDataSets = pDataSets;
		if(m_pDataSet!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSet);
		}
	}

	void GetDataSetResponse::SetDataSet(DataSet* pDataSet)
	{
		if(m_pDataSet!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSet);
		}
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		m_pDataSet = pDataSet;
	}

	void GetDataSetResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}