#include "RegisterLayerRequest.h"
#include "RegisterLayerResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{ 
	RegisterLayerResponse::RegisterLayerResponse(RegisterLayerRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pLayer = NULL;
	}

	RegisterLayerResponse::~RegisterLayerResponse()
	{
		Cleanup();
	}

	void RegisterLayerResponse::Cleanup()
	{
		if(m_pLayer!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pLayer);
		}
	}

	void RegisterLayerResponse::SetLayer(Layer* pLayer)
	{
		Cleanup();
		m_pLayer = pLayer;
	}

	void RegisterLayerResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE RegisterLayerResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = new XDocument();
		WriteLayer(pxDoc);

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

	void RegisterLayerResponse::WriteLayer(XDocument* pxDoc)
	{
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		char str[AUGE_MSG_MAX];

		pxRoot = pxDoc->CreateRootNode(m_pRequest->GetRequest(), NULL, NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", m_pRequest->GetVersion(), NULL);		

		AddLayerNode(pxRoot, m_pLayer);
	}

	void RegisterLayerResponse::AddLayerNode(XElement* pxRoot, Layer* pLayer)
	{
		char str[AUGE_NAME_MAX];
		const char* lname = pLayer->GetName();
		//XElement* pxRoot = pxLayer->AddChild("Layer", NULL);	
		//pxRoot->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
		//g_sprintf(str,"%d",pLayer->GetID());
		//pxRoot->SetAttribute("id", str,NULL);
		XElement* pxNode = pxRoot->AddChild("ID",NULL);
		g_sprintf(str,"%d",pLayer->GetID());
		pxNode->SetChildText(str);
		pxNode = pxRoot->AddChild("Name",NULL);
		pxNode->SetChildText(lname, true);
		pxNode = pxRoot->AddChild("Title",NULL);
		pxNode->SetChildText(lname, true);
		pxNode = pxRoot->AddChild("Abstract",NULL);
		g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
		pxNode = pxRoot->AddChild("CRS",NULL);

		GEnvelope extent = pLayer->GetExtent();
		if(!extent.IsValid())
		{
			extent.Set(-180.f,-90.0f,180.0f,90.0f);
		}
		AddLayerGeographicBoundingNode(pxRoot, extent);
		AddLayerBoundingNode(pxRoot, extent, pLayer->GetSRID());

		switch(pLayer->GetType())
		{
		case augeLayerFeature:
			{	
				pxNode = pxRoot->AddChild("Type",NULL);
				pxNode->AddChildText("Feature");
				FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
				AddLayerGeomTypeNode(pxRoot, pFeatureLayer);
			}
			break;
		case augeLayerRaster:
			{
				pxNode = pxRoot->AddChild("Type",NULL);
				pxNode->AddChildText("Raster");
			}
			break;
		case augeLayerQuadServer:
			{
				pxNode = pxRoot->AddChild("Type",NULL);
				pxNode->AddChildText("Tile");
				QuadServerLayer* pQuadServerLayer = static_cast<QuadServerLayer*>(pLayer);
				AddWebURLNode(pxRoot, pQuadServerLayer->GetURL());
			}
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Add Map
	//////////////////////////////////////////////////////////////////////////
	//void RegisterLayerResponse::SetLayerNode(XElement* pxLayer, const char* name, const char* title, const char* abstract, g_int srid, bool queryable)
	//{
	//	return NULL;
	//}

	void RegisterLayerResponse::AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
	{
		XElement* pxNode = NULL;
		XElement* pxBounding = NULL;
		char str[AUGE_NAME_MAX];

		pxBounding = pxParent->AddChild("BoundingBox", NULL);
		g_sprintf(str, "CRS:%d", srid);
		pxBounding->SetAttribute("CRS", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxBounding->SetAttribute("minx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxBounding->SetAttribute("maxx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxBounding->SetAttribute("miny", str,NULL);		
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxBounding->SetAttribute("maxy", str,NULL);
	}

	void RegisterLayerResponse::AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid)
	{
		XElement* pxNode = NULL;
		XElement* pxBounding = NULL;
		char str[AUGE_NAME_MAX];

		pxBounding = pxParent->AddChild("Viewer", NULL);
		g_sprintf(str, "CRS:%d", srid);
		pxBounding->SetAttribute("CRS", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxBounding->SetAttribute("minx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxBounding->SetAttribute("maxx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxBounding->SetAttribute("miny", str,NULL);		
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxBounding->SetAttribute("maxy", str,NULL);
	}

	void RegisterLayerResponse::AddLayerGeographicBoundingNode(XElement* pxParent, GEnvelope& extent)
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

	void RegisterLayerResponse::AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer)
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

	//void RegisterLayerResponse::AddMapNode(XElement* pxRoot, Map* pMap)
	//{
	//	char str[AUGE_MSG_MAX];

	//	XElement *pxNode = NULL;
	//	XElement *pxExtent = NULL;
	//	XElement *pxMap  = pxRoot->AddChild("Map", NULL);
	//	pxNode = pxMap->AddChild("Name");
	//	pxNode->AddChildText(pMap->GetName());

	//	// SRID
	//	g_sprintf(str, "%d", pMap->GetSRID());
	//	pxNode = pxMap->AddChild("Srid");
	//	pxNode->AddChildText(str);

	//	// Extent
	//	//AddBoundingBoxNode(pxMap, pMap->GetExtent());		
	//	AddLayerBoundingNode(pxMap, pMap->GetExtent(), pMap->GetSRID());

	//	// Layers
	//	XElement* pxLayers = pxMap->AddChild("Layers");

	//	Layer* pLayer = NULL;
	//	g_int count = pMap->GetLayerCount();
	//	for(g_int i=0; i<count; i++)
	//	{
	//		pLayer = pMap->GetLayer(i);
	//		if(pLayer!=NULL)
	//		{
	//			AddLayerNode(pxLayers, pLayer);
	//		}
	//	}

	//	XElement* pxThumbnail = pxMap->AddChild("Thumbnail");
	//	AddThumbnailNode(pxThumbnail, pMap); 

	//}

	//void RegisterLayerResponse::AddThumbnailNode(XElement* pxThumbnail, Map* pMap)
	//{
	//	char thumbnail[AUGE_PATH_MAX];
	//	memset(thumbnail,0,AUGE_PATH_MAX);
	//	g_snprintf(thumbnail, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), pMap->GetThumbnail());
	//	pxThumbnail->SetAttribute("xlink",thumbnail,NULL);
	//}

	//void RegisterLayerResponse::AddMapNode(XDocument* pxDoc, Map* pMap)
	//{
	//	char str[AUGE_MSG_MAX];

	//	XElement *pxNode = NULL;
	//	XElement *pxExtent = NULL;
	//	XElement *pxMap  = pxDoc->CreateRootNode("Map", NULL, NULL);
	//	pxNode = pxMap->AddChild("Name");
	//	pxNode->AddChildText(pMap->GetName());

	//	// SRID
	//	g_sprintf(str, "%d", pMap->GetSRID());
	//	pxNode = pxMap->AddChild("Srid");
	//	pxNode->AddChildText(str);

	//	// Extent
	//	AddBoundingBoxNode(pxMap, pMap->GetExtent());		

	//	// Layers
	//	XElement* pxLayers = pxMap->AddChild("Layers");

	//	Layer* pLayer = NULL;
	//	g_int count = pMap->GetLayerCount();
	//	for(g_int i=0; i<count; i++)
	//	{
	//		pLayer = pMap->GetLayer(i);
	//		if(pLayer!=NULL)
	//		{
	//			AddLayerNode(pxLayers, pLayer);
	//		}
	//	}
	//}

	void RegisterLayerResponse::AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent)
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


	//void RegisterLayerResponse::AddLayerNode(XElement* pxLayers, Layer* pLayer)
	//{

	//	XElement *pxNode = NULL;
	//	XElement *pxLayer = NULL;

	//	pxLayer = pxLayers->AddChild("Layer");
	//	pxNode = pxLayer->AddChild("Name");
	//	pxNode->AddChildText(pLayer->GetName());

	//	AddBoundingBoxNode(pxLayer, pLayer->GetExtent()); 
	//}

	void RegisterLayerResponse::AddWebURLNode(XElement* pxLayer, const char* url)
	{
		XElement* pxNode = NULL;
		pxNode = pxLayer->AddChild("URL");
		pxNode->AddChildText(url);

	}
}