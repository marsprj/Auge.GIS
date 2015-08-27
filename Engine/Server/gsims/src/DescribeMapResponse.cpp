#include "DescribeMapRequest.h"
#include "DescribeMapResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{ 
	DescribeMapResponse::DescribeMapResponse(DescribeMapRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pMap = NULL;
		m_pMaps = NULL;
	}

	DescribeMapResponse::~DescribeMapResponse()
	{
		Cleanup();
	}

	void DescribeMapResponse::Cleanup()
	{
		if(m_pMaps!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pMaps);
		}
		if(m_pMap!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pMap);
		}
	}

	void DescribeMapResponse::SetMap(Map* pMap)
	{
		Cleanup();
		m_pMap = pMap;
	}

	void DescribeMapResponse::SetMaps(EnumMap* pMaps)
	{
		Cleanup();
		m_pMaps = pMaps;
	}

	void DescribeMapResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE DescribeMapResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = new XDocument();
		if(m_pMaps!=NULL)
		{
			WriteMaps(pxDoc);
		}
		else if(m_pMap)
		{
			WriteMap(pxDoc);
		}

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
		
		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		return AG_SUCCESS;
	}

	void DescribeMapResponse::WriteMap(XDocument* pxDoc)
	{
		Map* pMap = m_pMap;
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		char str[AUGE_MSG_MAX];

		pxRoot = pxDoc->CreateRootNode("Map_Capabilities", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", "1.0.0", NULL);

		g_sprintf(str,"%d",pMap->GetID());
		XElement* pxID = pxRoot->AddChild("ID", NULL);
		pxID->SetChildText(str);
		XElement* pxName = pxRoot->AddChild("Name", NULL);
		pxName->SetChildText(pMap->GetName());
		// SRID
		g_sprintf(str, "%d", pMap->GetSRID());
		pxNode = pxRoot->AddChild("Srid");
		pxNode->AddChildText(str);

		// Extent
		//AddBoundingBoxNode(pxMap, pMap->GetExtent());		
		AddLayerBoundingNode(pxRoot, pMap->GetExtent(), pMap->GetSRID());
		AddViewerNode(pxRoot, pMap->GetViewer(), pMap->GetSRID());

		// WMS_Capabilities-->Capability
		XElement* pxCapability = pxRoot->AddChild("Capability", NULL);
		// WMS_Capabilities-->Capability-->Layer
		XElement* pxLayer = pxCapability->AddChild("Layer", NULL);
		pxNode = pxLayer->AddChild("Name", NULL);
		pxNode->AddChildText("Group");
		pxNode = pxLayer->AddChild("Abstract", NULL);
		pxNode->AddChildText("Group");
		pxNode = pxLayer->AddChild("Type", NULL);
		pxNode->AddChildText("Group");
		// WMS_Capabilities-->Capability-->Layer-->CRS
		pxNode = pxLayer->AddChild("CRS", NULL);
		pxNode->AddChildText("EPSG:4326");
		// WMS_Capabilities-->Capability-->Layer-->EX_GeographicBoundingBox
		GEnvelope extent = pMap->GetExtent();
		if(!extent.IsValid())
		{
			extent.Set(-180.f,-90.0f,180.0f,90.0f);
		}
		AddLayerGeographicBoundingNode(pxLayer, extent);
		AddLayerBoundingNode(pxLayer, extent, pMap->GetSRID());

		// WMS_Capabilities-->Capability-->Layer-->Layer
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{ 
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				const char* lname = pLayer->GetName();
				XElement* pxLayer_2 = pxLayer->AddChild("Layer", NULL);				
				pxLayer_2->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
				pxLayer_2->SetAttribute("visible", pLayer->IsVisiable()?"1":"0", NULL);
				g_sprintf(str,"%d",pLayer->GetID());
				pxLayer_2->SetAttribute("id", str,NULL);
				pxNode = pxLayer_2->AddChild("Name",NULL);
				pxNode->SetChildText(lname);
				pxNode = pxLayer_2->AddChild("Title",NULL);
				pxNode->SetChildText(lname);
				pxNode = pxLayer_2->AddChild("Abstract",NULL);
				g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
				pxNode = pxLayer_2->AddChild("CRS",NULL);
				
				switch(pLayer->GetType())
				{
				case augeLayerFeature:
					{
						XElement* pxLayerType = pxLayer_2->AddChild("Type");
						pxLayerType->AddChildText("Feature");

						FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
						AddLayerGeomTypeNode(pxLayer_2, pFeatureLayer);
						Style* pStyle = pFeatureLayer->GetStyle();
						if(pStyle!=NULL)
						{
							AddStyleNode(pxLayer_2, pStyle);
						}
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
						AddWebURLNode(pxLayer_2, pQuadServerLayer->GetURL());
					}
					break;
				}
				extent = pLayer->GetExtent();
				if(!extent.IsValid())
				{
					extent.Set(-180.f,-90.0f,180.0f,90.0f);
				}
				AddLayerGeographicBoundingNode(pxLayer_2, extent);
				AddLayerBoundingNode(pxLayer_2, extent, pMap->GetSRID());

			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Add Map
	//////////////////////////////////////////////////////////////////////////
	//void DescribeMapResponse::SetLayerNode(XElement* pxLayer, const char* name, const char* title, const char* abstract, g_int srid, bool queryable)
	//{
	//	return NULL;
	//}
	 
	void DescribeMapResponse::AddStyleNode(XElement* pxParent, Style* pStyle)
	{
		char str[AUGE_NAME_MAX];
		XElement* pxStyle = pxParent->AddChild("Style", NULL);
		g_sprintf(str, "%d", pStyle->GetID());
		pxStyle->SetAttribute("id",str,NULL);

		XElement* pxNode = pxStyle->AddChild("Name",NULL);
		pxNode->AddChildText(pStyle->GetName());
		pxNode = pxStyle->AddChild("Title",NULL);
		pxNode = pxStyle->AddChild("Abstract",NULL);
		XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
		pxLegendURL->SetAttribute("width","20", NULL);
		pxLegendURL->SetAttribute("height","20", NULL);
		pxNode = pxLegendURL->AddChild("Format",NULL);
		pxNode->SetChildText("image/png");
		pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
	}

	void DescribeMapResponse::AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
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

	void DescribeMapResponse::AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid)
	{
		XElement* pxNode = NULL;
		XElement* pxBounding = NULL;
		char str[AUGE_NAME_MAX];
		pxBounding = pxParent->AddChild("Viewer", NULL);
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

	void DescribeMapResponse::AddLayerGeographicBoundingNode(XElement* pxParent, GEnvelope& extent)
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

	void DescribeMapResponse::AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer)
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
	
	//////////////////////////////////////////////////////////////////////////
	// Add Maps
	//////////////////////////////////////////////////////////////////////////
	void DescribeMapResponse::WriteMaps(XDocument* pxDoc)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		Map* pMap = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		XElement	*pxNode = NULL;
		
		XElement	*pxRoot = pxDoc->CreateRootNode("Maps", NULL, NULL);
		m_pMaps->Reset();
		while((pMap=m_pMaps->Next())!=NULL)
		{
			AddMapNode(pxRoot, pMap);
		}
	}

	void DescribeMapResponse::AddMapNode(XElement* pxRoot, Map* pMap)
	{
		char str[AUGE_MSG_MAX];

		XElement *pxNode = NULL;
		XElement *pxExtent = NULL;
		XElement *pxMap  = pxRoot->AddChild("Map", NULL);
		pxNode = pxMap->AddChild("Name");
		pxNode->AddChildText(pMap->GetName());

		// SRID
		g_sprintf(str, "%d", pMap->GetSRID());
		pxNode = pxMap->AddChild("Srid");
		pxNode->AddChildText(str);

		// Extent
		//AddBoundingBoxNode(pxMap, pMap->GetExtent());		
		AddLayerBoundingNode(pxMap, pMap->GetExtent(), pMap->GetSRID());

		// Layers
		XElement* pxLayers = pxMap->AddChild("Layers");

		Layer* pLayer = NULL;
		g_int count = pMap->GetLayerCount();
		for(g_int i=0; i<count; i++)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				AddLayerNode(pxLayers, pLayer);
			}
		}

		XElement* pxThumbnail = pxMap->AddChild("Thumbnail");
		AddThumbnailNode(pxThumbnail, pMap); 

	}

	void DescribeMapResponse::AddThumbnailNode(XElement* pxThumbnail, Map* pMap)
	{
		char thumbnail[AUGE_PATH_MAX];
		memset(thumbnail,0,AUGE_PATH_MAX);
		g_snprintf(thumbnail, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), pMap->GetThumbnail());
		pxThumbnail->SetAttribute("xlink",thumbnail,NULL);
	}

	void DescribeMapResponse::AddMapNode(XDocument* pxDoc, Map* pMap)
	{
		char str[AUGE_MSG_MAX];

		XElement *pxNode = NULL;
		XElement *pxExtent = NULL;
		XElement *pxMap  = pxDoc->CreateRootNode("Map", NULL, NULL);
		pxNode = pxMap->AddChild("Name");
		pxNode->AddChildText(pMap->GetName());

		// SRID
		g_sprintf(str, "%d", pMap->GetSRID());
		pxNode = pxMap->AddChild("Srid");
		pxNode->AddChildText(str);

		// Extent
		AddBoundingBoxNode(pxMap, pMap->GetExtent());		

		// Layers
		XElement* pxLayers = pxMap->AddChild("Layers");

		Layer* pLayer = NULL;
		g_int count = pMap->GetLayerCount();
		for(g_int i=0; i<count; i++)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				AddLayerNode(pxLayers, pLayer);
			}
		}
	}

	void DescribeMapResponse::AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent)
	{
		XElement* pxNode = NULL;
		XElement* pxCorner = NULL;
		XElement* pxExtent = NULL;
		
		pxExtent = pxRoot->AddChild("BoundingBox");

		if(extent.IsValid())
		{
			char str[AUGE_MSG_MAX];
			g_sprintf(str, "%f %f", extent.m_xmin, extent.m_ymin);
			pxCorner = pxExtent->AddChild("LowerCorner");
			pxCorner->AddChildText(str);

			g_sprintf(str, "%f %f", extent.m_xmax, extent.m_ymax);
			pxCorner = pxExtent->AddChild("UpperCorner");
			pxCorner->AddChildText(str);
		}
		else
		{
			pxCorner = pxExtent->AddChild("LowerCorner");
			pxCorner->AddChildText("0 0");

			pxCorner = pxExtent->AddChild("UpperCorner");
			pxCorner->AddChildText("0 0");
		}		
	}


	void DescribeMapResponse::AddLayerNode(XElement* pxLayers, Layer* pLayer)
	{

		XElement *pxNode = NULL;
		XElement *pxLayer = NULL;

		pxLayer = pxLayers->AddChild("Layer");
		pxNode = pxLayer->AddChild("Name");
		pxNode->AddChildText(pLayer->GetName());

		AddBoundingBoxNode(pxLayer, pLayer->GetExtent()); 
	}

	void DescribeMapResponse::AddWebURLNode(XElement* pxLayer, const char* url)
	{
		XElement* pxNode = NULL;
		pxNode = pxLayer->AddChild("URL");
		pxNode->AddChildText(url);

	}
}