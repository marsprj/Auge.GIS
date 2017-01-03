#include "DescribeTaskRequest.h"
#include "DescribeTaskResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeUser.h"

#include "CsTask.h"
#include "EnumTask.h"

namespace auge
{ 
	DescribeTaskResponse::DescribeTaskResponse(DescribeTaskRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pTask = NULL;
		m_pTasks = NULL;
	}

	DescribeTaskResponse::~DescribeTaskResponse()
	{
		Cleanup();
	}

	void DescribeTaskResponse::Cleanup()
	{
		if(m_pTasks!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTasks);
		}
		if(m_pTask!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTask);
		}
	}

	void DescribeTaskResponse::SetTask(CsTask* pTask)
	{
		Cleanup();
		m_pTask = pTask;
	}

	void DescribeTaskResponse::SetTasks(EnumTask* pTasks)
	{
		Cleanup();
		m_pTasks = pTasks;
	}

	void DescribeTaskResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE DescribeTaskResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = new XDocument();
		if(m_pTasks!=NULL)
		{
			WriteTasks(pxDoc);
		}
		else if(m_pTask)
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

	void DescribeTaskResponse::WriteMap(XDocument* pxDoc)
	{
		CsTask* pTask = m_pTask;
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		char str[AUGE_MSG_MAX];

		pxRoot = pxDoc->CreateRootNode("Task", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", "1.0.0", NULL);

		// Name
		pxNode = pxRoot->AddChild("Name");
		pxNode->AddChildText(pxRoot->GetName(),true);
		// description
		pxNode = pxRoot->AddChild("Description");
		pxNode->AddChildText(pTask->GetDescription(),true);
		// Owner
		g_uint user_id = pTask->GetOwner();
		UserManager* pUserManager = augeGetUserManagerInstance();
		User* pUser = pUserManager->GetUser(user_id);
		pxNode = pxRoot->AddChild("Owner");	
		if(pUser!=NULL)
		{
			pxNode->AddChildText(pUser->GetName());
		}

		// Map
		g_uint map_id = pTask->GetMapID();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map* pMap = pCartoManager->GetMap(map_id);

		XElement* pxMap = pxRoot->AddChild("Map");

		g_sprintf(str,"%d",pMap->GetID());
		XElement* pxID = pxMap->AddChild("ID", NULL);
		pxID->SetChildText(str);
		XElement* pxName = pxMap->AddChild("Name", NULL);
		pxName->SetChildText(pMap->GetName(), true);
		// SRID
		g_sprintf(str, "%d", pMap->GetSRID());
		pxNode = pxMap->AddChild("Srid");
		pxNode->AddChildText(str);

		// Extent
		//AddBoundingBoxNode(pxMap, pMap->GetExtent());		
		AddLayerBoundingNode(pxMap, pMap->GetExtent(), pMap->GetSRID());
		AddViewerNode(pxMap, pMap->GetViewer(), pMap->GetSRID());
		XElement* pxThumbnail = pxMap->AddChild("Thumbnail");
		AddThumbnailNode(pxThumbnail, pMap); 

		// WMS_Capabilities-->Capability
		XElement* pxCapability = pxMap->AddChild("Capability", NULL);
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

				XElement* pxLayer_2 = NULL;
				switch(pLayer->GetType())
				{
				case augeLayerFeature:
				case augeLayerRaster:
					pxLayer_2 = pxLayer->AddChild("Layer", NULL);
					break;
				case augeLayerQuadServer:
				case augeLayerWMTS:
					pxLayer_2 = pxCapability->AddChild("Layer", NULL);
					break;
				}

				pxLayer_2->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
				pxLayer_2->SetAttribute("visible", pLayer->IsVisiable()?"1":"0", NULL);
				g_sprintf(str,"%d",pLayer->GetID());
				pxLayer_2->SetAttribute("id", str,NULL);
				pxNode = pxLayer_2->AddChild("Name",NULL);
				pxNode->SetChildText(lname, true);
				pxNode = pxLayer_2->AddChild("Title",NULL);
				pxNode->SetChildText(lname, true);
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
				case augeLayerWMTS:
					{
						XElement* pxLayerType = pxLayer_2->AddChild("Type");
						pxLayerType->AddChildText("WMTS");

						WMTSLayer* pWMTSLayer = static_cast<WMTSLayer*>(pLayer);
						AddWebURLNode(pxLayer_2, pWMTSLayer->GetURL());
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
	//void DescribeTaskResponse::SetLayerNode(XElement* pxLayer, const char* name, const char* title, const char* abstract, g_int srid, bool queryable)
	//{
	//	return NULL;
	//}
	 
	void DescribeTaskResponse::AddStyleNode(XElement* pxParent, Style* pStyle)
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

	void DescribeTaskResponse::AddLayerBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
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

	void DescribeTaskResponse::AddViewerNode(XElement* pxParent, GEnvelope& extent, int srid)
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

	void DescribeTaskResponse::AddLayerGeographicBoundingNode(XElement* pxParent, GEnvelope& extent)
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

	void DescribeTaskResponse::AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer)
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
	void DescribeTaskResponse::WriteTasks(XDocument* pxDoc)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		CsTask* pTask = NULL;
		Map* pMap = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		XElement	*pxNode = NULL;
		
		XElement	*pxRoot = pxDoc->CreateRootNode("Tasks", NULL, NULL);
		m_pTasks->Reset();
		while((pTask=m_pTasks->Next())!=NULL)
		{
			AddTaskNode(pxRoot, pTask);
		}
	}

	void DescribeTaskResponse::AddTaskNode(XElement* pxRoot, CsTask* pTask)
	{
		char str[AUGE_MSG_MAX];

		XElement *pxNode = NULL;
		XElement *pxExtent = NULL;
		XElement *pxTask  = pxRoot->AddChild("Task", NULL);

		// Name
		pxNode = pxTask->AddChild("Name");
		pxNode->AddChildText(pTask->GetName(),true);
		// description
		pxNode = pxTask->AddChild("Description");
		pxNode->AddChildText(pTask->GetDescription(),true);
		// Owner
		g_uint user_id = pTask->GetOwner();
		UserManager* pUserManager = augeGetUserManagerInstance();
		User* pUser = pUserManager->GetUser(user_id);
		pxNode = pxRoot->AddChild("Owner");	
		if(pUser!=NULL)
		{
			pxNode->AddChildText(pUser->GetName());
		}

		// Map
		g_uint map_id = pTask->GetMapID();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		Map* pMap = pCartoManager->GetMap(map_id);
		if(pMap!=NULL)
		{
			XElement *pxMap  = pxTask->AddChild("Map", NULL);
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
	}

	void DescribeTaskResponse::AddThumbnailNode(XElement* pxThumbnail, Map* pMap)
	{
		char thumbnail[AUGE_PATH_MAX];
		memset(thumbnail,0,AUGE_PATH_MAX);
		//g_snprintf(thumbnail, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), pMap->GetThumbnail());
		g_snprintf(thumbnail, AUGE_PATH_MAX, "/ows/thumbnail/%s", pMap->GetThumbnail());
		pxThumbnail->SetAttribute("xlink",thumbnail,NULL);
	}

	void DescribeTaskResponse::AddMapNode(XDocument* pxDoc, Map* pMap)
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

	void DescribeTaskResponse::AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent)
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


	void DescribeTaskResponse::AddLayerNode(XElement* pxLayers, Layer* pLayer)
	{

		XElement *pxNode = NULL;
		XElement *pxLayer = NULL;

		pxLayer = pxLayers->AddChild("Layer");
		pxNode = pxLayer->AddChild("Name");
		pxNode->AddChildText(pLayer->GetName());

		AddBoundingBoxNode(pxLayer, pLayer->GetExtent()); 
	}

	void DescribeTaskResponse::AddWebURLNode(XElement* pxLayer, const char* url)
	{
		XElement* pxNode = NULL;
		pxNode = pxLayer->AddChild("URL");
		pxNode->AddChildText(url);

	}
}