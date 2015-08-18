#include "DescribeLayerRequest.h"
#include "DescribeLayerResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeField.h"

namespace auge
{ 
	DescribeLayerResponse::DescribeLayerResponse(DescribeLayerRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pLayer = NULL;
	}

	DescribeLayerResponse::~DescribeLayerResponse()
	{
		Cleanup();
	}

	void DescribeLayerResponse::Cleanup()
	{
		if(m_pLayer!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pLayer);
		}
	}

	void DescribeLayerResponse::SetLayer(Layer* pLayer)
	{
		Cleanup();
		m_pLayer = pLayer;
	}
	
	void DescribeLayerResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE DescribeLayerResponse::Write(WebWriter* pWriter)
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

	void DescribeLayerResponse::WriteLayer(XDocument* pxDoc)
	{
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		char str[AUGE_MSG_MAX];

		g_uint srid = 0;
		Layer* pLayer = m_pLayer;
		
		//XElement* pxLayer = pxDoc->CreateRootNode("Layer_Capabilities", NULL, NULL);
		////pxLayer->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		////pxLayer->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		////pxLayer->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		//pxLayer->SetAttribute("version", m_pRequest->GetVersion(), NULL);

		const char* lname = pLayer->GetName();
		XElement* pxLayer = pxDoc->CreateRootNode("Layer", NULL, NULL);//pxLayer->AddChild("Layer", NULL);				
		pxLayer->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
		pxLayer->SetAttribute("visible", pLayer->IsVisiable()?"1":"0", NULL);
		g_sprintf(str,"%d",pLayer->GetID());
		pxLayer->SetAttribute("id", str,NULL);
		pxNode = pxLayer->AddChild("Name",NULL);
		pxNode->SetChildText(lname);
		pxNode = pxLayer->AddChild("Title",NULL);
		pxNode->SetChildText(lname);
		pxNode = pxLayer->AddChild("Abstract",NULL);
		g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
		pxNode = pxLayer->AddChild("CRS",NULL);

		switch(pLayer->GetType())
		{
		case augeLayerFeature:
			{
				XElement* pxLayerType = pxLayer->AddChild("Type");
				pxLayerType->AddChildText("Feature");

				FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
				// FeatureClass
				FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
				srid = pFeatureClass->GetSRID();
				AddFeatureNode(pxLayer, pFeatureClass);				
				// Style
				Style* pStyle = pFeatureLayer->GetStyle();
				if(pStyle!=NULL)
				{
					AddStyleNode(pxLayer, pStyle);
				}
			}
			break;
		case augeLayerRaster:
			{
				XElement* pxLayerType = pxLayer->AddChild("Type");
				pxLayerType->AddChildText("Raster");
				RasterLayer* pRasterLayer = static_cast<RasterLayer*>(pLayer);				
				Raster* pRaster = pRasterLayer->GetRaster();
				srid = pRasterLayer->GetSRID();

				AddRasterNode(pxLayer, pRaster);

			}
			break;
		case augeLayerQuadServer:
			{
				XElement* pxLayerType = pxLayer->AddChild("Type");
				pxLayerType->AddChildText("QuadServer");

				QuadServerLayer* pQuadServerLayer = static_cast<QuadServerLayer*>(pLayer);
				AddWebURLNode(pxLayer, pQuadServerLayer->GetURL());
			}
			break;
		}
		GEnvelope& extent = pLayer->GetExtent();
		if(!extent.IsValid())
		{
			extent.Set(-180.f,-90.0f,180.0f,90.0f);
		}
		AddLayerGeographicBoundingNode(pxLayer, extent);
		AddLayerBoundingNode(pxLayer, extent, srid);
	}

	//////////////////////////////////////////////////////////////////////////
	// Add Map
	//////////////////////////////////////////////////////////////////////////
	//void DescribeLayerResponse::SetLayerNode(XElement* pxLayer, const char* name, const char* title, const char* abstract, g_int srid, bool queryable)
	//{
	//	return NULL;
	//}
	 
	void DescribeLayerResponse::AddStyleNode(XElement* pxParent, Style* pStyle)
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

	void DescribeLayerResponse::AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
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

	void DescribeLayerResponse::AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid)
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

	void DescribeLayerResponse::AddLayerGeographicBoundingNode(XElement* pxParent, GEnvelope& extent)
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

	//////////////////////////////////////////////////////////////////////////
	// Add Maps
	//////////////////////////////////////////////////////////////////////////


	void DescribeLayerResponse::AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent)
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


	void DescribeLayerResponse::AddLayerNode(XElement* pxLayers, Layer* pLayer)
	{

		XElement *pxNode = NULL;
		XElement *pxLayer = NULL;

		pxLayer = pxLayers->AddChild("Layer");
		pxNode = pxLayer->AddChild("Name");
		pxNode->AddChildText(pLayer->GetName());

		AddBoundingBoxNode(pxLayer, pLayer->GetExtent()); 
	}

	void DescribeLayerResponse::AddWebURLNode(XElement* pxLayer, const char* url)
	{
		XElement* pxNode = NULL;
		pxNode = pxLayer->AddChild("URL");
		pxNode->AddChildText(url);

	}


	void DescribeLayerResponse::AddFeatureNode(XElement* pxLayer, FeatureClass* pFeatureClass)
	{
		XElement* pxClass = pxLayer->AddChild("Feature");
		if(pFeatureClass==NULL)
		{
			return;
		}

		XElement* pxNode = pxClass->AddChild("ClassName", NULL);
		pxNode->AddChildText(pFeatureClass->GetName());

		pxNode = pxClass->AddChild("GeometryType", NULL);
		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		if(pField!=NULL)
		{
			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
			const char* type = pGeometryFactory->Encode(pField->GetGeometryDef()->GeometryType());
			pxNode->AddChildText(type);
		}

		char str[AUGE_NAME_MAX];
		g_sprintf(str, "%d", pFeatureClass->GetCount());
		pxNode = pxClass->AddChild("FeatureCount", NULL);
		pxNode->AddChildText(str);
	}

	void DescribeLayerResponse::AddRasterNode(XElement* pxLayer, Raster* pRaster)
	{
		XElement* pxRaster = pxLayer->AddChild("Raster", NULL);

		if(pRaster==NULL)
		{
			return;
		}

		char str[AUGE_NAME_MAX];
		// 名称
		XElement* pXNode = pxRaster->AddChild("RasterName", NULL);
		pXNode->AddChildText(pRaster->GetName());		

		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		//波段
		g_sprintf(str, "%d", pRaster->GetBandCount());
		pXNode = pxRaster->AddChild("Bands", NULL);
		pXNode->AddChildText(str);

		// 格式
		pXNode = pxRaster->AddChild("Format", NULL);
		pXNode->AddChildText(pRaster->GetFormat());
		
		//像素类型
		pXNode = pxRaster->AddChild("PixelType", NULL);
		pXNode->AddChildText(pRasterFactory->Encoding(pRaster->GetPixelType()));

		//像素宽度
		g_sprintf(str, "%d", pRaster->GetPixelSize());
		pXNode = pxRaster->AddChild("PixelSize", NULL);
		pXNode->AddChildText(str);

		//分辨率
		g_sprintf(str, "%f", pRaster->GetResolution_X());		
		pXNode = pxRaster->AddChild("Resolution_X", NULL);
		pXNode->AddChildText(str);
		g_sprintf(str, "%f", pRaster->GetResolution_Y());		
		pXNode = pxRaster->AddChild("GetResolution_Y", NULL);
		pXNode->AddChildText(str);

		//宽度
		g_sprintf(str, "%d", pRaster->GetWidth());		
		pXNode = pxRaster->AddChild("Width", NULL);
		pXNode->AddChildText(str);
		//高度
		g_sprintf(str, "%d", pRaster->GetHeight());		
		pXNode = pxRaster->AddChild("Height", NULL);
		pXNode->AddChildText(str);
	}
}