#include "GetThumbnailRequest.h"
#include "GetThumbnailResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeXML.h"
#include "AugeProcessor.h"

namespace auge
{
	GetThumbnailResponse::GetThumbnailResponse(GetThumbnailRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pRaster = NULL;
		m_pRasters = NULL;
	}

	GetThumbnailResponse::~GetThumbnailResponse()
	{
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
		if(m_pRaster!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRaster);
		}
		if(m_pRasters!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRasters);
		}
	}

	void GetThumbnailResponse::SetRaster(Raster* pRaster)
	{
		m_pRaster = pRaster;
	}

	void GetThumbnailResponse::SetRasters(EnumRaster* pRasters)
	{
		m_pRasters = pRasters;
	}

	RESULTCODE GetThumbnailResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		
		
		Raster* pRaster = m_pRaster;

		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Rasters",NULL,NULL);

		if(m_pRasters!=NULL)
		{
			WriteRasters(pxRoot);
		}
		else
		{
			WriteRaster(pxRoot);
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

	void GetThumbnailResponse::WriteRaster(XElement* pxParent)
	{
		if(m_pRaster!=NULL)
		{
			XElement* pxRaster = pxParent->AddChild("Raster", NULL);
			AddRasterNode(pxRaster, m_pRaster);
		}
	}

	void GetThumbnailResponse::WriteRasters(XElement* pxParent)
	{
		Raster* pRaster = NULL;
		m_pRasters->Reset();
		while((pRaster=m_pRasters->Next())!=NULL)
		{
			XElement* pxRaster = pxParent->AddChild("Raster", NULL);
			AddRasterNode(pxRaster, pRaster);
		}
	}

	void GetThumbnailResponse::AddRasterNode(XElement* pxRaster, Raster* pRaster)
	{
		WebContext* pWebContext = augeGetWebContextInstance();
		const char* raster_uuid = pRaster->GetUUID();

		char thumbnail_path[AUGE_PATH_MAX];
		memset(thumbnail_path, 0, AUGE_PATH_MAX);
		auge_make_path(thumbnail_path, NULL, pWebContext->GetThumbnailPath(), raster_uuid, AUGE_THUMBNAIL_SUFFIX);

		if(g_access(thumbnail_path,4))
		{	//如果文件不存在
			const char* file_local_path = pRaster->GetPath();
			RasterThumbnailProcessor* pProcessor = NULL;
			GProcessorFactory* pFactory = augeGetGeoProcessorFactoryInstance();
			pProcessor = pFactory->CreateRasterThumbnailProcessor();
			pProcessor->SetInputRaster(file_local_path);
			pProcessor->SetOutputRaster(thumbnail_path);
			pProcessor->SetThumbSize(AUGE_THUMBNAIL_SIZE);
			pProcessor->Execute();
			pProcessor->Release();
		}

		XElement*	pxName = pxRaster->AddChild("Name",NULL);
		pxName->AddChildText(pRaster->GetName());

		char web_path[AUGE_PATH_MAX];
		memset(web_path,0,AUGE_PATH_MAX);
		g_snprintf(web_path, AUGE_PATH_MAX, "/ows/thumbnail/%s.%s", raster_uuid, AUGE_THUMBNAIL_SUFFIX);
		XElement*	pxThumbnail = pxRaster->AddChild("Thumbnail",NULL);
		pxThumbnail->SetAttribute("xlink",web_path,NULL);
	}

}