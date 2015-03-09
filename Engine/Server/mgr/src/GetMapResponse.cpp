#include "GetMapRequest.h"
#include "GetMapResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"

namespace auge
{ 
	GetMapResponse::GetMapResponse(GetMapRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetMapResponse::~GetMapResponse()
	{
	}

	RESULTCODE GetMapResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();

		Map* pMap = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Maps", NULL, NULL);
		
		const char* name = m_pRequest->GetName();
		if(name==NULL)
		{
			g_uint count = pCartoManager->GetCount();
			for(g_uint i=0; i<count; i++)
			{
				pMap = pCartoManager->LoadMap(i);
				if(pMap!=NULL)
				{
					AddMapNode(pxRoot, pMap);
					pMap->Release();
				}
			}
		}
		else
		{
			pMap = pCartoManager->LoadMap(name);
			if(pMap!=NULL)
			{
				AddMapNode(pxRoot, pMap);
				pMap->Release();
			}
		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return AG_SUCCESS;
	}

	void GetMapResponse::AddMapNode(XElement* pxRoot, Map* pMap)
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

	void GetMapResponse::AddBoundingBoxNode(XElement* pxRoot, GEnvelope& extent)
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

	void GetMapResponse::AddLayerNode(XElement* pxLayers, Layer* pLayer)
	{
		XElement *pxNode = NULL;
		XElement *pxLayer = NULL;

		pxLayer = pxLayers->AddChild("Layer");
		pxNode = pxLayer->AddChild("Name");
		pxNode->AddChildText(pLayer->GetName());

		AddBoundingBoxNode(pxLayer, pLayer->GetExtent()); 
	}
}