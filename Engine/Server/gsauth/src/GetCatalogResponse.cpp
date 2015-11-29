#include "GetCatalogRequest.h"
#include "GetCatalogResponse.h"
#include "AugeService.h"
#include "AugeUser.h"
#include "AugeField.h"
#include "AugeFeature.h"
#include "AugeCarto.h"

namespace auge
{
	GetCatalogResponse::GetCatalogResponse(GetCatalogRequest* pRequest)
	{
		m_pUser = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetCatalogResponse::~GetCatalogResponse()
	{
		if(m_pUser!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUser);
		}
	}

	RESULTCODE GetCatalogResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();

		XDocument* pxDoc = BuildDocument();


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
	
	void GetCatalogResponse::SetUser(User* pUser)
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

	XDocument* GetCatalogResponse::BuildDocument()
	{
		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxParent = pxDoc->CreateRootNode("Catalog", NULL, NULL);							

		// Data
		BuildDataNode(pxParent);
		BuildMapsNode(pxParent);
		BuildServicesNode(pxParent);
		BuildOperatorsNode(pxParent); 

		return pxDoc;
	}

	void GetCatalogResponse::BuildDataNode(XElement* pxParent)
	{
		XElement* pxData = pxParent->AddChild("Data", NULL);
		BuildVectorDataNode(pxData);
		BuildRasterDataNode(pxData);
		BuildTileDataNode(pxData);
	}

	void GetCatalogResponse::BuildVectorDataNode(XElement* pxParent)
	{
		XElement* pxVector = pxParent->AddChild("Vector", NULL);

		g_uint user_id = m_pUser->GetID();

		Workspace* pWorkspace = NULL;
		FeatureWorkspace* pFeatureWorkspace = NULL;
		EnumWorkspace* pEnumWorkspace = NULL;

		ConnectionManager* pConnectionManager = NULL;
		pConnectionManager = augeGetConnectionManagerInstance();

		pEnumWorkspace = pConnectionManager->GetWorkspaces(user_id, augeWorkspaceFeature);
		if(pEnumWorkspace!=NULL)
		{
			pEnumWorkspace->Reset();
			while((pWorkspace=pEnumWorkspace->Next())!=NULL)
			{
				BuildDataSourceNode(pxVector, pWorkspace);
			}

			pEnumWorkspace->Release();
		}
	}
	
	void GetCatalogResponse::BuildDataSourceNode(XElement* pxParent, Workspace* pWorkspace)
	{
		XElement	*pxNode = NULL;
		XElement	*pxDataSource  = NULL;

		pxDataSource = pxParent->AddChild("DataSource", NULL);

		pxNode = pxDataSource->AddChild("Name");
		pxNode->AddChildText(pWorkspace->GetName());
		pxNode = pxDataSource->AddChild("Engine");
		pxNode->AddChildText(pWorkspace->GetEngine()->GetID());
		pxNode = pxDataSource->AddChild("ConnectionString");
		pxNode->AddChildText(pWorkspace->GetConnectionString());
	}

	//void GetCatalogResponse::BuildVectorNode(XElement* pxParent)
	//{
	//	XElement* pxVector = pxParent->AddChild("Vector", NULL);

	//	g_uint user_id = m_pUser->GetID();

	//	Workspace* pWorkspace = NULL;
	//	FeatureWorkspace* pFeatureWorkspace = NULL;
	//	EnumWorkspace* pEnumWorkspace = NULL;
	//	
	//	ConnectionManager* pConnectionManager = NULL;
	//	pConnectionManager = augeGetConnectionManagerInstance();

	//	pEnumWorkspace = pConnectionManager->GetWorkspaces(user_id);
	//	if(pEnumWorkspace!=NULL)
	//	{
	//		pEnumWorkspace->Reset();
	//		while((pWorkspace=pEnumWorkspace->Next())!=NULL)
	//		{
	//			pFeatureWorkspace = dynamic_cast<FeatureWorkspace*>(pWorkspace);
	//			if(pFeatureWorkspace!=NULL)
	//			{
	//				if(!pFeatureWorkspace->IsOpen())
	//				{
	//					if(pFeatureWorkspace->Open()==AG_SUCCESS)
	//					{
	//						BuildFeatureWorkspaceNode(pxVector, pFeatureWorkspace);
	//					}
	//				}
	//				else
	//				{
	//					BuildFeatureWorkspaceNode(pxVector, pFeatureWorkspace);
	//				}
	//			}
	//		}

	//		pEnumWorkspace->Release();
	//	}
	//}

	void GetCatalogResponse::BuildFeatureWorkspaceNode(XElement* pxParent, FeatureWorkspace* pFeatureWorkspace)
	{
		DataSet* pDataset = NULL;
		EnumDataSet* pEnumDataset = NULL;
		FeatureClass* pFeatureClass = NULL;

		XElement* pxWorkspace = pxParent->AddChild("Datasource", NULL);
		pxWorkspace->SetAttribute("Name", pFeatureWorkspace->GetName(), NULL, true);

		pEnumDataset = pFeatureWorkspace->GetFeatureClasses();
		if(pEnumDataset!=NULL)
		{
			pEnumDataset->Reset(); 
			while((pDataset=pEnumDataset->Next())!=NULL)
			{
				pFeatureClass = static_cast<FeatureClass*>(pDataset);
				BuildFeatureWorkspaceNode(pxWorkspace, pFeatureClass);
			}
			pEnumDataset->Release();
		}
	}

	void GetCatalogResponse::BuildFeatureWorkspaceNode(XElement* pxParent, FeatureClass* pFeatureClass)
	{
		GField*  pField = NULL;
		GFields* pFields = NULL;
		GField*	 pGeomField = NULL;
		g_uint	 nFields = 0;

		XElement* pxField  = NULL;
		XElement* pxFields = NULL;
		XElement* pxFeatureClass = pxParent->AddChild("FeatureClass", NULL);
		XElement* pxNode = NULL;

		char str[AUGE_MSG_MAX];
		
		pFields = pFeatureClass->GetFields();
		pGeomField = pFields->GetGeometryField();

		pxFeatureClass->SetAttribute("Name", pFeatureClass->GetName(), NULL);

		// GeometryField
		if(pGeomField!=NULL)
		{
			GeometryDef* pGeometryDef = pGeomField->GetGeometryDef();
			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

			pxNode = pxFeatureClass->AddChild("GeometryType", NULL);
			pxNode->AddChildText(pGeometryFactory->Encode(pGeometryDef->GeometryType()));

			// srid
			g_sprintf(str, "EPSG:%d", pGeometryDef->GetSRID());
			pxNode->SetAttribute("SRID", str, NULL);
			
			pxNode = pxFeatureClass->AddChild("Extent", NULL);
			GEnvelope& extent = pFeatureClass->GetExtent();
			g_sprintf(str, "%8.3f", extent.m_xmin);
			pxNode->SetAttribute("xmin", str, NULL);
			g_sprintf(str, "%8.3f", extent.m_ymin);
			pxNode->SetAttribute("ymin", str, NULL);
			g_sprintf(str, "%8.3f", extent.m_xmax);
			pxNode->SetAttribute("xmax", str, NULL);
			g_sprintf(str, "%8.3f", extent.m_ymax);
			pxNode->SetAttribute("ymax", str, NULL);
		}

		// Fields
		nFields = pFields->Count();		
		pxFields = pxFeatureClass->AddChild("Fields", NULL);
		for(g_uint i=0; i<nFields; i++)
		{
			pField = pFields->GetField(i);
			BuildFeatureFieldNode(pxFields, pField);
		}

	}

	void GetCatalogResponse::BuildFeatureFieldNode(XElement* pxFields, GField* pField)
	{
		XElement* pxNode = NULL;
		XElement* pxField = pxFields->AddChild("Field", NULL);
		
		pxField->SetAttribute("Name", pField->GetName(), NULL);
		pxField->SetAttribute("Type", pField->GetTypeName(), NULL);
	}

	void GetCatalogResponse::BuildRasterDataNode(XElement* pxParent)
	{
		XElement* pxRaster = pxParent->AddChild("Raster", NULL);

		g_uint user_id = m_pUser->GetID();

		Workspace* pWorkspace = NULL;
		FeatureWorkspace* pFeatureWorkspace = NULL;
		EnumWorkspace* pEnumWorkspace = NULL;

		ConnectionManager* pConnectionManager = NULL;
		pConnectionManager = augeGetConnectionManagerInstance();

		pEnumWorkspace = pConnectionManager->GetWorkspaces(user_id, augeWorkspaceRaster);
		if(pEnumWorkspace!=NULL)
		{
			pEnumWorkspace->Reset();
			while((pWorkspace=pEnumWorkspace->Next())!=NULL)
			{
				BuildDataSourceNode(pxRaster, pWorkspace);
			}

			pEnumWorkspace->Release();
		}
	}

	void GetCatalogResponse::BuildTileDataNode(XElement* pxParent)
	{
		XElement* pxRaster = pxParent->AddChild("Tile", NULL);

		g_uint user_id = m_pUser->GetID();
		
		Workspace* pWorkspace = NULL;
		FeatureWorkspace* pFeatureWorkspace = NULL;
		EnumWorkspace* pEnumWorkspace = NULL;

		ConnectionManager* pConnectionManager = NULL;
		pConnectionManager = augeGetConnectionManagerInstance();

		pEnumWorkspace = pConnectionManager->GetWorkspaces(user_id, augeWorkspaceTile);
		if(pEnumWorkspace!=NULL)
		{
			pEnumWorkspace->Reset();
			while((pWorkspace=pEnumWorkspace->Next())!=NULL)
			{
				BuildDataSourceNode(pxRaster, pWorkspace);
			}

			pEnumWorkspace->Release();
		}
	}

	void GetCatalogResponse::BuildMapsNode(XElement* pxParent)
	{
		XElement* pxMaps = pxParent->AddChild("Maps", NULL);

		Map* pMap = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		EnumMap* pEnumMap = pCartoManager->GetMaps(m_pUser->GetID());
		if(pEnumMap!=NULL)
		{
			pEnumMap->Reset();
			while((pMap=pEnumMap->Next())!=NULL)
			{
				Map* pMap2 = pCartoManager->LoadMap(m_pUser->GetID(), pMap->GetName());
				if(pMap2!=NULL)
				{
					BuildMapNode(pxMaps, pMap2);
					pMap2->Release();
				}
			}

			pEnumMap->Release();
		}
	}

	void GetCatalogResponse::BuildMapNode(XElement* pxParent, Map* pMap)
	{
		char str[AUGE_MSG_MAX];
		XElement* pxNode = NULL;
		XElement* pxMap  = NULL;

		pxMap = pxParent->AddChild("Map", NULL);
		g_sprintf(str,"%d",pMap->GetID());
		pxMap->SetAttribute("ID", str, NULL);
		XElement* pxName = pxMap->AddChild("Name", NULL);
		pxName->SetChildText(pMap->GetName());
		// SRID
		g_sprintf(str, "%d", pMap->GetSRID());
		pxNode = pxMap->AddChild("Srid");
		pxNode->AddChildText(str);

		GEnvelope extent = pMap->GetExtent();
		if(!extent.IsValid())
		{
			extent.Set(-180.f,-90.0f,180.0f,90.0f);
		}
		AddBoundingNode(pxMap, extent, pMap->GetSRID());

		XElement* pxLayers = pxMap->AddChild("Layers", NULL);
		// WMS_Capabilities-->Capability-->Layer-->Layer
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{ 
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				const char* lname = pLayer->GetName();
				XElement* pxLayer = pxLayers->AddChild("Layer", NULL);				
				//pxLayer->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
				//pxLayer->SetAttribute("visible", pLayer->IsVisiable()?"1":"0", NULL);
				g_sprintf(str,"%d",pLayer->GetID());
				pxLayer->SetAttribute("id", str,NULL);
				pxNode = pxLayer->AddChild("Name",NULL);
				pxNode->SetChildText(lname);
				g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
				pxNode = pxLayer->AddChild("CRS",NULL);

				switch(pLayer->GetType())
				{
				case augeLayerFeature:
					{
						XElement* pxLayerType = pxLayer->AddChild("Type");
						pxLayerType->AddChildText("Feature");

						FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
						AddLayerGeomTypeNode(pxLayer, pFeatureLayer);
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
					}
					break;
				case augeLayerQuadServer:
					{
						XElement* pxLayerType = pxLayer->AddChild("Type");
						pxLayerType->AddChildText("QuadServer");

						QuadServerLayer* pQuadServerLayer = static_cast<QuadServerLayer*>(pLayer);
						//AddWebURLNode(pxLayer, pQuadServerLayer->GetURL());
					}
					break;
				}
				extent = pLayer->GetExtent();
				if(!extent.IsValid())
				{
					extent.Set(-180.f,-90.0f,180.0f,90.0f);
				}
				AddBoundingNode(pxLayer, extent, pMap->GetSRID());
			}
		}
	}

	void GetCatalogResponse::BuildServicesNode(XElement* pxParent)
	{
		XElement* pxServices = pxParent->AddChild("Services", NULL);

		Service* pService = NULL;
		EnumService* pEnumService = NULL;
		ServiceManager* pServiceManager = augeGetServiceManagerInstance();
		pEnumService = pServiceManager->GetServices(m_pUser->GetID());

		if(pEnumService!=NULL)
		{
			pEnumService->Reset();
			while((pService=pEnumService->Next())!=NULL)
			{
				AddServiceNode(pxServices, pService);
			}

			pEnumService->Release();
		}
	}

	void GetCatalogResponse::AddServiceNode(XElement* pxParent, Service* pService)
	{
		char str[AUGE_NAME_MAX];
		XElement* pxNode = NULL;
		XElement* pxService = pxParent->AddChild("Service", NULL);
		
		g_sprintf(str, "%d", pService->GetID());
		pxService->SetAttribute("ID", str, NULL);
		pxNode = pxService->AddChild("Name", NULL);
		pxNode->SetChildText(pService->GetName());
		
		pxNode = pxService->AddChild("URI", NULL);
		pxNode->SetChildText(pService->GetURI());

		pxNode = pxService->AddChild("Description", NULL);
		pxNode->SetChildText("");
	}

	void GetCatalogResponse::BuildOperatorsNode(XElement* pxParent)
	{
		XElement* pxOperators = pxParent->AddChild("Operators", NULL);

		AddOperatorNode(pxOperators, "FeatureProject",	"矢量数据投影");
		AddOperatorNode(pxOperators, "FeatureImport",	"矢量数据导入");
		AddOperatorNode(pxOperators, "CsvImport",		"导入Csv文件");
		AddOperatorNode(pxOperators, "RasterEdgeDetect","边缘提取");
		AddOperatorNode(pxOperators, "RasterExtractByRectangle",	"影像裁剪");
		AddOperatorNode(pxOperators, "RasterReverse",	"灰度反转");
		AddOperatorNode(pxOperators, "RasterGraylize",	"影像灰度化");
		AddOperatorNode(pxOperators, "RasterSmooth",	"影像平滑");
		AddOperatorNode(pxOperators, "RasterStretch",	"灰度拉伸");
		AddOperatorNode(pxOperators, "RasterSubtract",	"影像相减");
		AddOperatorNode(pxOperators, "RasterPixelBlend","像素融合");
		AddOperatorNode(pxOperators, "RasterThreshold",	"二值化");
		AddOperatorNode(pxOperators, "RasterHistogramEqualization",	"直方图均衡");
		AddOperatorNode(pxOperators, "RasterSepiaTone",	"旧照片效果");
		AddOperatorNode(pxOperators, "DemSlope",		"坡度");
		AddOperatorNode(pxOperators, "DemAspect",		"坡向");
		AddOperatorNode(pxOperators, "DemStretch",		"DEM灰度拉伸");
		AddOperatorNode(pxOperators, "DemHillshade",	"山体阴影");
		AddOperatorNode(pxOperators, "Delaunay",		"Delaunay三角网");
		AddOperatorNode(pxOperators, "GetArea",			"面积");
		AddOperatorNode(pxOperators, "GetLength",		"长度");
		AddOperatorNode(pxOperators, "Buffer",			"缓冲区");
		AddOperatorNode(pxOperators, "Centroid",		"中心点");
		AddOperatorNode(pxOperators, "Convexhull",		"凸包");
		AddOperatorNode(pxOperators, "MultiPointToPoints",	"多点转点");
		AddOperatorNode(pxOperators, "LineToPoints",	"线转点");
		AddOperatorNode(pxOperators, "PolygonToPoints",	"多边形转点");
		AddOperatorNode(pxOperators, "PolygonToLine",	"多边形转线");
		AddOperatorNode(pxOperators, "GenerateRandomPoints",	"随机点生成");
		AddOperatorNode(pxOperators, "GenerateRandomPointsInPolygon",	"多边形内随机点");
		AddOperatorNode(pxOperators, "BuildPyramid",	"栅格金字塔");
		AddOperatorNode(pxOperators, "KMean",			"kmeans聚类");
	}

	void GetCatalogResponse::AddOperatorNode(XElement* pxParent, const char* name, const char* description)
	{
		XElement* pxNode = NULL;
		XElement* pxOper = pxParent->AddChild("Operator", NULL);
		pxNode = pxOper->AddChild("Name");
		pxNode->SetChildText(name, true);
		pxNode = pxOper->AddChild("Description");
		pxNode->SetChildText(description, true);
	}

	void GetCatalogResponse::AddBoundingNode(XElement* pxParent, GEnvelope& extent, int srid)
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

	void GetCatalogResponse::AddLayerGeomTypeNode(XElement* pxLayer, FeatureLayer* pFeatureLayer)
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

	void GetCatalogResponse::AddStyleNode(XElement* pxParent, Style* pStyle)
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
}