#include "DescribeServiceRequest.h"
#include "DescribeServiceResponse.h"
#include "AugeService.h"
#include "AugeField.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	DescribeServiceResponse::DescribeServiceResponse(DescribeServiceRequest* pRequest)
	{
		m_pUser = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	DescribeServiceResponse::~DescribeServiceResponse()
	{
		if(m_pUser!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUser);
		}
	}

	void DescribeServiceResponse::SetUser(User* pUser)
	{
		if(m_pUser!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUser);
		}
		m_pUser = pUser;
		if(m_pUser!=NULL)
		{
			m_pUser->AddRef();
		}
	}

	RESULTCODE DescribeServiceResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		g_uint user_id = m_pUser->GetID();
		GLogger* pLogger = augeGetLoggerInstance();

		const char* encoding = m_pRequest->GetEncoding();

		Service* pService = NULL;
		EnumService *pServices = NULL;
		ServiceManager* pServiceManager = augeGetServiceManagerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		
		const char* name = m_pRequest->GetName();
		if(name==NULL)
		{
			XElement	*pxRoot = pxDoc->CreateRootNode("IMS_Services", NULL, NULL);			
			XElement	*pxServices = NULL;
			XElement	*pxService = NULL;

			pServices = pServiceManager->LoadServices(user_id);
			pServices->Reset();

			while((pService=pServices->Next())!=NULL)
			{	
				pxService = pxRoot->AddChild("Service", NULL);
				AddServiceNode(pxService, pService);
			}
			pServices->Release();
		}
		else
		{
			pService = pServiceManager->LoadService(user_id, name);
			if(pService!=NULL)
			{
				XElement *pxService = pxDoc->CreateRootNode("Service", NULL, NULL);
				AddServiceNode(pxService, pService);
				pService->Release();
			}
		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, encoding,0);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return AG_SUCCESS;
	}

	bool DescribeServiceResponse::AddServiceNode(XElement* pxService, Service* pService)
	{
		//const char* name = pService->GetName();
		//const char* uri  = pService->GetURI();
		//XElement* pxService = pxParent->AddChild("Service", NULL);
		//pxService->SetAttribute("name", name, NULL);
		//pxService->SetAttribute("uri", uri, NULL);

		const char* name = pService->GetName();
		XElement* pxNode = pxService->AddChild("Name");
		pxNode->SetChildText(name,true);

		XElement* pxState = pxService->AddChild("State");
		pxState->SetChildText(pService->IsStarted() ? "Started" : "Stopped");
		
		AddMapNode(pxService, pService->GetMap());
		AddOperations(pxService);
		DescribeMap(pxService, pService->GetMap());

		return true;
	}

	bool DescribeServiceResponse::AddMapNode(XElement* pxService, Map* pMap)
	{
		XElement* pxMap = pxService->AddChild("Map", NULL);
		if(pMap!=NULL)
		{
			const char* name = pMap->GetName();
			//pxMap->SetAttribute("name", name, NULL);
			pxMap->SetChildText(name, true);

			g_uint srid = pMap->GetSRID();
			char temp[AUGE_MSG_MAX];
			g_snprintf(temp, AUGE_MSG_MAX, "%d", srid);
			XElement* pxSrid = pxService->AddChild("SRID");
			pxSrid->SetChildText(temp);

			AddThumbnailNode(pxService, pMap);

			GEnvelope& extent = pMap->GetExtent();
			AddLayerBoundingNode(pxService, extent, srid);
		}
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// DescribeMap
	//////////////////////////////////////////////////////////////////////////
	void DescribeServiceResponse::DescribeMap(XElement* pxService, Map* pMap)
	{
		if(pMap==NULL)
		{
			return;
		}
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		char str[AUGE_MSG_MAX];
		
		//pxNode = pxService->AddChild("Name", NULL);
		//pxNode->AddChildText();
		//// WMS_Capabilities-->Capability-->Layer-->CRS
		//pxNode = pxService->AddChild("CRS", NULL);
		//pxNode->AddChildText("EPSG:4326");
		//// WMS_Capabilities-->Capability-->Layer-->EX_GeographicBoundingBox
		//GEnvelope extent = pMap->GetExtent();
		//if(!extent.IsValid())
		//{
		//	extent.Set(-180.f,-90.0f,180.0f,90.0f);
		//}
		//AddLayerGeographicBoundingNode(pxService, extent);
		//AddLayerBoundingNode(pxService, extent, pMap->GetSRID());
		
		// WMS_Capabilities-->Layers
		XElement* pxLayers = pxService->AddChild("Layers", NULL);

		// WMS_Capabilities-->Capability-->Layer-->Layer
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{ 
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				const char* lname = pLayer->GetName();
				XElement* pxLayer_2 = pxLayers->AddChild("Layer", NULL);				
				pxLayer_2->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
				pxLayer_2->SetAttribute("visible", pLayer->IsVisiable()?"1":"0", NULL);
				g_sprintf(str,"%d",pLayer->GetID());
				pxLayer_2->SetAttribute("id", str,NULL);
				pxNode = pxLayer_2->AddChild("Name",NULL);
				pxNode->SetChildText(lname, true);
				g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
				pxNode = pxLayer_2->AddChild("CRS",NULL);
				pxNode->SetChildText(str);

				switch(pLayer->GetType())
				{
				case augeLayerFeature:
					{
						XElement* pxLayerType = pxLayer_2->AddChild("Type");
						pxLayerType->AddChildText("Feature");

						FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
						AddLayerGeomTypeNode(pxLayer_2, pFeatureLayer);
					}
					break;
				case augeLayerRaster:
					{
						XElement* pxLayerType = pxLayer_2->AddChild("Type");
						pxLayerType->AddChildText("Raster");

						RasterLayer* pRasterLayer = static_cast<RasterLayer*>(pLayer);
					}
					break;
				case augeLayerQuadServer:
					{
						XElement* pxLayerType = pxLayer_2->AddChild("Type");
						pxLayerType->AddChildText("QuadServer");

						QuadServerLayer* pQuadServerLayer = static_cast<QuadServerLayer*>(pLayer);
						//AddWebURLNode(pxLayer_2, pQuadServerLayer->GetURL());
					}
					break;
				}
				GEnvelope& extent = pLayer->GetExtent();
				if(!extent.IsValid())
				{
					extent.Set(-180.f,-90.0f,180.0f,90.0f);
				}
				AddLayerGeographicBoundingNode(pxLayer_2, extent);
				AddLayerBoundingNode(pxLayer_2, extent, pMap->GetSRID());

			}
		}
	}

	void DescribeServiceResponse::AddLayerGeographicBoundingNode(XElement* pxParent, GEnvelope& extent)
	{
		XElement* pxNode = NULL;
		XElement* pxGeoBounding = NULL;
		char str[AUGE_NAME_MAX];

		pxGeoBounding = pxParent->AddChild("EX_GeographicBoundingBox", NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxNode = pxGeoBounding->AddChild("westBoundLongitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxNode = pxGeoBounding->AddChild("southBoundLatitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxNode = pxGeoBounding->AddChild("northBoundLatitude");
		pxNode->SetChildText(str);
	}

	void DescribeServiceResponse::AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
	{
		XElement* pxNode = NULL;
		XElement* pxBounding = NULL;
		char str[AUGE_NAME_MAX];

		pxBounding = pxParent->AddChild("BoundingBox", NULL);
		g_sprintf(str, "CRS:%d", srid);
		pxBounding->SetAttribute("CRS", str,NULL);

		if(!extent.IsValid())
		{
			return;
		}

		g_sprintf(str, "%.6f", extent.m_xmin);
		pxBounding->SetAttribute("minx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxBounding->SetAttribute("maxx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxBounding->SetAttribute("miny", str,NULL);		
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxBounding->SetAttribute("maxy", str,NULL);
	}

	void DescribeServiceResponse::AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer)
	{
		XElement* pxGeomType = pxLayer->AddChild("GeometryType", NULL);
		FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
		if(pFeatureClass!=NULL)
		{
			GField* pField = pFeatureClass->GetFields()->GetGeometryField();
			if(pField!=NULL)
			{
				GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
				const char* type = pGeometryFactory->Encode(pField->GetGeometryDef()->GeometryType());
				pxGeomType->AddChildText(type);
			}
		}
	}

	void DescribeServiceResponse::AddThumbnailNode(XElement* pxParent, Map* pMap)
	{
		char thumbnail[AUGE_PATH_MAX];
		memset(thumbnail,0,AUGE_PATH_MAX);
		//g_snprintf(thumbnail, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), pMap->GetThumbnail());
		g_snprintf(thumbnail, AUGE_PATH_MAX, "/ows/thumbnail/%s", pMap->GetThumbnail());
		XElement* pxThumbnail = pxParent->AddChild("Thumbnail");
		pxThumbnail->AddChildText(thumbnail);
	}

	void DescribeServiceResponse::AddOperations(XElement* pxService)
	{
		XElement* pxOperation = NULL;
		XElement* pxOperations = NULL;
		pxOperations = pxService->AddChild("Operations");
		pxOperation = pxOperations->AddChild("Operation");
		pxOperation->AddChildText("WMS");
		pxOperation = pxOperations->AddChild("Operation");
		pxOperation->AddChildText("WFS");
		pxOperation = pxOperations->AddChild("Operation");
		pxOperation->AddChildText("WMTS");
		pxOperation = pxOperations->AddChild("Operation");
		pxOperation->AddChildText("WPS");
	}
}
