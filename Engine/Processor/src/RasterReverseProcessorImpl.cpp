#include "RasterReverseProcessorImpl.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#include <math.h>

#include <fstream>

namespace auge
{
	RasterReverseProcessorImpl::RasterReverseProcessorImpl()
	{
		m_user = 0;
		m_in_raster_path = "/";
		m_out_raster_path = "/";
	}

	RasterReverseProcessorImpl::~RasterReverseProcessorImpl()
	{

	}
	
	void RasterReverseProcessorImpl::SetInputDataSource(const char* sourceName)
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

	void RasterReverseProcessorImpl::SetInputRaster(const char* rasterName)
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

	void RasterReverseProcessorImpl::SetOutputDataSource(const char* sourceName)
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

	void RasterReverseProcessorImpl::SetOutputRaster(const char* rasterName)
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

	const char*	RasterReverseProcessorImpl::GetInputDataSource()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	const char*	RasterReverseProcessorImpl::GetInputRaster()
	{
		return m_in_raster_name.empty() ? NULL : m_in_raster_name.c_str();
	}

	const char*	RasterReverseProcessorImpl::GetOutputDataSource()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	const char*	RasterReverseProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_name.empty() ? NULL : m_out_raster_name.c_str();
	}

	void RasterReverseProcessorImpl::SetInputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_in_raster_path.clear();
		}
		else
		{
			m_in_raster_path = rasterPath;
		}
	}

	void RasterReverseProcessorImpl::SetOutputPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_out_raster_path.clear();
		}
		else
		{
			m_out_raster_path = rasterPath;
		}
	}

	const char* RasterReverseProcessorImpl::GetInputRasterPath()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char* RasterReverseProcessorImpl::GetOutputRasterPath()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterReverseProcessorImpl::Execute()
	{
		const char* inSourceName = GetInputDataSource();
		const char* inRasterName = GetInputRaster();
		const char* inRasterPath = GetInputRasterPath();
		const char* outSourceName = GetOutputDataSource();
		const char* outRasterName = GetOutputRaster();
		const char* outRasterPath = GetOutputRasterPath();

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pinRasterWorkspace = NULL;
		RasterWorkspace* poutRasterWorkspace = NULL;

		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(m_user, inSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		pinRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		pWorkspace = pConnManager->GetWorkspace(m_user, outSourceName);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}
		poutRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		Raster* pinRaster = NULL;
		RasterFolder* pinFolder = NULL;

		Raster* poutRaster = NULL;
		RasterFolder* poutFolder = NULL;

		pinFolder = pinRasterWorkspace->GetFolder(inRasterPath);
		if(pinFolder==NULL)
		{
			return AG_FAILURE;
		}
		pinRaster = pinFolder->GetRasterDataset()->GetRaster(inRasterName);

		
		augePixelType pixelType =  pinRaster->GetPixelType();
		if(pixelType != augePixelByte)
		{
			pinFolder->Release();
			return AG_FAILURE;
		}

		poutRaster = Reverse(pinRaster);

		if(poutRaster!=NULL)
		{
			poutRaster->SetName(outRasterName);
		}
		poutFolder = poutRasterWorkspace->GetFolder(outRasterPath);
		if(poutFolder==NULL)
		{
			pinFolder->Release();
			poutRaster->Release();
			return AG_FAILURE;
		}
		RESULTCODE rc = poutFolder->GetRasterDataset()->AddRaster(outRasterName, poutRaster);
		poutRaster->Release();
		pinFolder->Release();
		poutFolder->Release();

		return rc;
	}

	void RasterReverseProcessorImpl::Release()
	{
		delete this;
	}

	void RasterReverseProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

	Raster* RasterReverseProcessorImpl::Reverse(Raster* pinRaster)
	{
		RasterBand* pBand  = NULL;
		Raster* poutRaster = NULL;

		int band_count = pinRaster->GetBandCount();
		for(int i=0; i<band_count; i++)
		{
			pBand = pinRaster->GetBand(i);
			Reverse(pBand);			
		}

		poutRaster = pinRaster;
		poutRaster->AddRef();
		return poutRaster;
	}

	RESULTCODE RasterReverseProcessorImpl::Reverse(RasterBand* Band)
	{
		g_uint width = Band->GetWidth();
		g_uint height= Band->GetHeight();

		g_byte v_h=0, v_v=0;
		g_byte* data = (g_byte*)Band->GetData();
		g_byte* ptr = data;
		
		for(g_uint i=0; i<height; i++)
		{	
			for(g_uint j=0; j<width; j++, ptr++)
			{
				*ptr = 255 - *ptr;
			}
		}
		Band->SetData(data);
		return AG_SUCCESS;
	}
}
