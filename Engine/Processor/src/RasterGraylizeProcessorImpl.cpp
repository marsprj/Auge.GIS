#include "RasterGraylizeProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"

namespace auge
{
	RasterGraylizeProcessorImpl::RasterGraylizeProcessorImpl()
	{
	}

	RasterGraylizeProcessorImpl::~RasterGraylizeProcessorImpl()
	{

	}
	
	void RasterGraylizeProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterGraylizeProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterGraylizeProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterGraylizeProcessorImpl::SetOutputRaster(const char* rasterName)
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
	
	const char*	RasterGraylizeProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterGraylizeProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterGraylizeProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterGraylizeProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	RESULTCODE RasterGraylizeProcessorImpl::Execute()
	{
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();

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

		pWorkspace = pConnManager->GetWorkspace(outSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		Raster* pinRaster = NULL;
		RasterDataset* pinRasterDataset = NULL;

		Raster* poutRaster = NULL;
		RasterDataset* poutRasterDataset = NULL;

		pinRasterDataset = pinRasterWorkspace->OpenRasterDataset(inRasterName);
		if(pinRasterDataset==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinRasterDataset->GetRaster();
		g_uint band_count = pinRaster->GetBandCount();
		if(band_count<3||band_count>4)
		{
			pinRasterDataset->Release();
			return AG_FAILURE;
		}


		if(!Greylize(pinRaster))
		{
			pinRasterDataset->Release();
			return AG_FAILURE;
		}
		
		
		RESULTCODE rc = poutRasterWorkspace->AddRaster(pinRaster);
		//poutRaster->Release();
		pinRasterDataset->Release();

		return rc;
	}

	void RasterGraylizeProcessorImpl::Release()
	{
		delete this;
	}

	bool RasterGraylizeProcessorImpl::Greylize(Raster* pRaster)
	{
		g_uint64 w = pRaster->GetWidth();
		g_uint64 h = pRaster->GetHeight();
		g_uint64 size = w * h;
	
		RasterBand* prBand = pRaster->GetBand(0);
		RasterBand* pgBand = pRaster->GetBand(1);
		RasterBand* pbBand = pRaster->GetBand(2);

		g_uchar* r = (g_uchar*)(prBand->GetData());
		g_uchar* g = (g_uchar*)(pgBand->GetData());
		g_uchar* b = (g_uchar*)(pbBand->GetData());

		//Gray = R*0.299 + G*0.587 + B*0.114
		for(g_uint64 i=0; i<size; i++)
		{
			r[i] = (r[i]*299 + g[i]*587 + b[i]*114) / 1000;
		}
		prBand->SetData(r);
		pgBand->SetData(r);
		pbBand->SetData(r);

		free(r);
		free(g);
		free(b);


		return true;
	}
}
