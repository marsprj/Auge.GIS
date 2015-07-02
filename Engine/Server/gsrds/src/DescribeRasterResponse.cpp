#include "DescribeRasterRequest.h"
#include "DescribeRasterResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeXML.h"

namespace auge
{
	DescribeRasterResponse::DescribeRasterResponse(DescribeRasterRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pRasterDataset = NULL;
	}

	DescribeRasterResponse::~DescribeRasterResponse()
	{
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
		if(m_pRasterDataset!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRasterDataset);
		}
	}

	void DescribeRasterResponse::SetRasterDataset(RasterDataset* pRasterDataset)
	{
		m_pRasterDataset = pRasterDataset;
	}

	RESULTCODE DescribeRasterResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		
		char str[AUGE_NAME_MAX];
		Raster* pRaster = m_pRasterDataset->GetRaster();

		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Raster",NULL,NULL);

		XElement*	pxNode = pxRoot->AddChild("Name",NULL);
		pxNode->AddChildText(pRaster->GetName());

		pxNode = pxRoot->AddChild("Alias",NULL);
		pxNode->AddChildText(pRaster->GetAlias());
		
		pxNode = pxRoot->AddChild("Format",NULL);
		pxNode->AddChildText(pRaster->GetFormat());

		// BandCount
		g_sprintf(str,"%d",pRaster->GetBandCount());
		pxNode = pxRoot->AddChild("Bands",NULL);
		pxNode->AddChildText(str);

		//srid
		g_sprintf(str,"%d", pRaster->GetSRID()); 
		pxNode = pxRoot->AddChild("SRID",NULL);
		pxNode->AddChildText(str);
		//Width
		g_sprintf(str,"%d", pRaster->GetWidth());
		pxNode = pxRoot->AddChild("Width",NULL);
		pxNode->AddChildText(str);
		//Height
		g_sprintf(str,"%d", pRaster->GetHeight());
		pxNode = pxRoot->AddChild("Height",NULL);
		pxNode->AddChildText(str);
		
		//extent
		GEnvelope& extent = pRaster->GetExtent();
		XElement* pxBounding = pxRoot->AddChild("Boundingbox",NULL);
		g_sprintf(str,"%f %f",extent.m_xmin,extent.m_ymin);
		pxNode = pxBounding->AddChild("LowerLeft");
		pxNode->AddChildText(str);

		g_sprintf(str,"%f %f",extent.m_xmin,extent.m_ymin);
		pxNode = pxBounding->AddChild("UpperRight");
		pxNode->AddChildText(str);

		// path
		pxNode = pxRoot->AddChild("Path",NULL);
		pxNode->AddChildText(pRaster->GetPath());


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

}