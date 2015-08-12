#include "DescribeTileStoreRequest.h"
#include "DescribeTileStoreResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeCarto.h"

namespace auge
{
	DescribeTileStoreResponse::DescribeTileStoreResponse(DescribeTileStoreRequest* pRequest)
	{
		m_pTileStore = NULL;
		m_pTileStores = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pWebContext = NULL;
	}

	DescribeTileStoreResponse::~DescribeTileStoreResponse()
	{
		if(m_pTileStores!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTileStores);
		}
		if(m_pTileStore!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTileStore);
		}
	}

	RESULTCODE DescribeTileStoreResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = NULL;
		if(m_pTileStore!=NULL)
		{
			pxDoc = Write(pWriter, m_pTileStore);
		}
		else
		{
			pxDoc = Write(pWriter, m_pTileStores);
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();
	}

	XDocument* DescribeTileStoreResponse::Write(WebWriter* pWriter, EnumTileStore* pTileStores)
	{
		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxTileStore  = NULL;
		XElement	*pxTileStores = pxDoc->CreateRootNode("TileStores", NULL, NULL);
		TileStore	*pTileStore = NULL;

		pTileStores->Reset();
		while((pTileStore=pTileStores->Next())!=NULL)
		{
			pxTileStore = pxTileStores->AddChild("TileStore", NULL);
			AddTileStoreNode(pxTileStore, pTileStore);
		}

		return pxDoc;

	}

	XDocument* DescribeTileStoreResponse::Write(WebWriter* pWriter, TileStore* pTileStore)
	{
		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxTileStore = pxDoc->CreateRootNode("TileStore", NULL, NULL);

		AddTileStoreNode(pxTileStore, pTileStore);

		return pxDoc;
	}

	void DescribeTileStoreResponse::AddTileStoreNode(XElement* pxTileStore, TileStore* pTileStore)
	{
		char str[AUGE_PATH_MAX];
		const char* tile_name = pTileStore->GetName();

		XElement* pxTitle = pxTileStore->AddChild("Title",NULL);
		pxTitle->AddChildText(tile_name);
		XElement* pxIdentifier = pxTileStore->AddChild("Identifier",NULL);
		pxIdentifier->AddChildText(tile_name);
		XElement* pxAbstract = pxTileStore->AddChild("Abstract",NULL);
		pxAbstract->AddChildText(tile_name);

		GEnvelope& extent = pTileStore->GetExtent();
		XElement* pxWGS84BoundingBox = pxTileStore->AddChild("WGS84BoundingBox", NULL);
		pxWGS84BoundingBox->SetAttribute("crs","urn:ogc:def:crs:OGC:2:84",NULL);
		g_snprintf(str, AUGE_PATH_MAX, "%f %f", extent.m_xmin, extent.m_ymin);
		XElement* pxLowerCorner = pxWGS84BoundingBox->AddChild("LowerCorner", NULL);
		pxLowerCorner->AddChildText(str);
		g_snprintf(str, AUGE_PATH_MAX, "%f %f", extent.m_xmax, extent.m_ymax);
		XElement* pxUpperCorner = pxWGS84BoundingBox->AddChild("UpperCorner", NULL);
		pxUpperCorner->AddChildText(str);

		XElement* pxStyle = pxTileStore->AddChild("Style",NULL);
		pxStyle->SetAttribute("IsDefault", "true",NULL);
		pxTitle = pxStyle->AddChild("Title", NULL);
		pxTitle->SetChildText("Default Style");
		pxIdentifier = pxStyle->AddChild("Identifier", NULL);
		pxIdentifier->AddChildText("Default");

		XElement* pxFormat = pxTileStore->AddChild("Format",NULL);
		pxFormat->AddChildText("image/png");

		XElement* pxTileMatrixSetLink = pxTileStore->AddChild("TileMatrixSetLink",NULL); 
		XElement* pxTileMatrixSet = pxTileMatrixSetLink->AddChild("TileMatrixSet",NULL);
		//pxTileMatrixSet->AddChildText("GoogleCRS84Quad");
		pxTileMatrixSet->AddChildText(pTileStore->GetTileTypeAsString());

		XElement* pxResourceURL = pxTileStore->AddChild("ResourceURL", NULL);
		pxResourceURL->SetAttribute("format", "image/png", NULL);
		pxResourceURL->SetAttribute("resourceType", "tile", NULL);
		
	}

	//void DescribeTileStoreResponse::AddFeatureClassNode(XElement* pxClass, FeatureClass* pFeatureClass)
	//{
	//	GField* pField = pFeatureClass->GetFields()->GetGeometryField();
	//	XElement* pxGeometry = pxClass->AddChild("Geometry");
	//	if(pField!=NULL)
	//	{
	//		GeometryDef* pGeometryDef = pField->GetGeometryDef();
	//		if(pGeometryDef!=NULL)
	//		{
	//			augeGeometryType type = pGeometryDef->GeometryType();
	//			GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
	//			const char* geomType = pGeometryFactory->Encode(type);

	//			XElement* pxElem = pxGeometry->AddChild("Type");
	//			pxElem->AddChildText(geomType);
	//			 
	//			char str[AUGE_NAME_MAX] = {0};
	//			g_sprintf(str, "%d", pGeometryDef->GetSRID());
	//			pxElem = pxGeometry->AddChild("SRID");
	//			pxElem->AddChildText(str);
	//		}
	//	}

	//	AddThumbnailNode(pxClass, pFeatureClass);
	//}

	//void DescribeTileStoreResponse::AddThumbnailNode(XElement* pxClass, FeatureClass* pFeatureClass)
	//{
	//	char img_local_path[AUGE_PATH_MAX] = {0};
	//	memset(img_local_path, 0, AUGE_PATH_MAX);		
	//	auge_make_path(img_local_path, NULL, m_pWebContext->GetCacheMapPath(), pFeatureClass->GetUUID(), "png");
	//	if(g_access(img_local_path, 4))
	//	{
	//		DrawThumbnail(pFeatureClass, img_local_path);
	//	}

	//	char thumbnail[AUGE_PATH_MAX];
	//	memset(thumbnail,0,AUGE_PATH_MAX);
	//	auge_make_path(thumbnail, NULL, NULL, pFeatureClass->GetUUID(), "png");
	//	char xlink[AUGE_PATH_MAX];
	//	memset(xlink,0,AUGE_PATH_MAX);
	//	g_snprintf(xlink, AUGE_PATH_MAX, "http://%s:%s/ows/thumbnail/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), thumbnail);
	//	XElement* pxThumbnail = pxClass->AddChild("Thumbnail", NULL);
	//	pxThumbnail->SetAttribute("xlink",xlink,NULL);
	//}

	//void DescribeTileStoreResponse::DrawThumbnail(FeatureClass* pFeatureClass, const char* img_local_path)
	//{
	//	g_uint width  = AUGE_THUMBNAIL_WIDTH;
	//	g_uint height = AUGE_THUMBNAIL_HEIGHT;


	//	Canvas* pCanvas = NULL;
	//	CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
	//	GLogger* pLogger = augeGetLoggerInstance();

	//	Style* pStyle = NULL;
	//	FeatureLayer* pFeatureLayer = NULL;
	//	CartoManager* pCartoManager = augeGetCartoManagerInstance();
	//	StyleFactory* pStyleFactory = augeGetStyleFactoryInstance();

	//	pFeatureLayer = pCartoFactory->CreateFeatureLayer();
	//	pFeatureLayer->SetFeatureClass(pFeatureClass);

	//	GField* pField = pFeatureClass->GetFields()->GetGeometryField();
	//	augeGeometryType type = pField->GetGeometryDef()->GeometryType();
	//	pStyle = pStyleFactory->CreateFeatureStyle(type, 10000);		

	//	GColor bgColor(255,255,255,255);
	//	pCanvas = pCartoFactory->CreateCanvas2D(width, height);
	//	pCanvas->DrawBackground(bgColor);

	//	GEnvelope extent = pFeatureClass->GetExtent();
	//	pCanvas->SetViewer(extent);
	//	pCanvas->DrawLayer(pFeatureLayer, pStyle);
	//	pCanvas->Save(img_local_path);
	//	pCanvas->Release();;
	//}


	void DescribeTileStoreResponse::SetTileStores(EnumTileStore* pTileStores)
	{
		if(m_pTileStores!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTileStores);
		}
		m_pTileStores = pTileStores;
		if(m_pTileStore!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTileStore);
		}
	}

	void DescribeTileStoreResponse::SetTileStore(TileStore* pTileStore)
	{
		if(m_pTileStore!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTileStore);
		}
		if(m_pTileStores!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTileStores);
		}
		m_pTileStore = pTileStore;
	}

	void DescribeTileStoreResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}
