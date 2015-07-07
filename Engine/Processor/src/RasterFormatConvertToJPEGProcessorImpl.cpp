#include "RasterFormatConvertToJPEGProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"

namespace auge
{
	RasterFormatConvertToJPEGProcessorImpl::RasterFormatConvertToJPEGProcessorImpl()
	{

	}

	RasterFormatConvertToJPEGProcessorImpl::~RasterFormatConvertToJPEGProcessorImpl()
	{

	}

	void RasterFormatConvertToJPEGProcessorImpl::SetInputRectangle(GEnvelope& rect)
	{
		m_rect = rect;
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetInputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name.clear();
		}
		else
		{
			m_in_source_name = sourceName;
		}
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetInputRaster(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_in_raster_name.clear();
		}
		else
		{
			m_in_raster_name = rasterName;
		}
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetOutputDataSource(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_out_source_name.clear();
		}
		else
		{
			m_out_source_name = sourceName;
		}
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetOutputRaster(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_out_raster_name.clear();
		}
		else
		{
			m_out_raster_name = rasterName;
		}
	}

	void RasterFormatConvertToJPEGProcessorImpl::SetOutputPath(const char* path)
	{
		if(path==NULL)
		{
			m_out_path.clear();
		}
		else
		{
			m_out_path = path;
		}
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	const char*	RasterFormatConvertToJPEGProcessorImpl::GetOutputPath()
	{
		return m_out_path.empty() ? NULL : m_out_path.c_str();
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Execute()
	{
		GEnvelope&  inRect = m_rect;
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();
		const char* outPath = GetOutputPath();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(inSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		Raster* pinRaster = NULL;
		RasterDataset* pinRasterDataset = NULL;
		pinRasterDataset = pinRasterWorkspace->OpenRasterDataset(inRasterName);
		if(pinRasterDataset==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinRasterDataset->GetRaster();

		// recalculate the requested extent, in case of the input rect is out of the original extent
		GEnvelope extent;
		GEnvelope& raster_extent = pinRaster->GetExtent();
		if(inRect.IsValid())
		{
			extent = raster_extent.Intersect(inRect);
		}
		else
		{
			extent = raster_extent;
		}

		// convert map extent to image rect
		GRect rastertRect;
		pinRaster->GetRasterRect(rastertRect, extent);

		RESULTCODE rc = AG_SUCCESS;
		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();

		// Create output JPG Raster
		poutRaster = pRasterFactory->CreateRasterJPG(outRasterName, rastertRect.GetWidth(), rastertRect.GetWidth());
		if(poutRaster==NULL)
		{
			pinRasterDataset->Release();
			return AG_FAILURE;
		}

		Fill(poutRaster, pinRaster, rastertRect);

		if(outPath!=NULL)
		{				
			rc = poutRaster->Save(outPath, "jpeg");
			poutRaster->Release();
			pinRasterDataset->Release();
		}
		else
		{
			pWorkspace = pConnManager->GetWorkspace(outSourceName);
			if(pWorkspace==NULL)
			{
				return AG_FAILURE;
			}
			poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

			rc = poutRasterWorkspace->AddRaster(poutRaster);

			poutRaster->Release();
			pinRasterDataset->Release();
		}

		return rc;
	}

	void RasterFormatConvertToJPEGProcessorImpl::Release()
	{
		delete this;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		g_uint band_count = pinRaster->GetBandCount();

		if(band_count==1)
		{
			Fill_1(poutRaster, pinRaster, rastertRect);
		}
		else if(band_count==2)
		{
			Fill_2(poutRaster, pinRaster, rastertRect);
		}
		else if(band_count==3)
		{
			Fill_3(poutRaster, pinRaster, rastertRect);
		}
		else
		{
			Fill_4(poutRaster, pinRaster, rastertRect);
		}

		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_1(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_2(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_3(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		return AG_SUCCESS;
	}

	RESULTCODE RasterFormatConvertToJPEGProcessorImpl::Fill_4(Raster* poutRaster, Raster* pinRaster, GRect rastertRect)
	{
		return AG_SUCCESS;
	}
}
