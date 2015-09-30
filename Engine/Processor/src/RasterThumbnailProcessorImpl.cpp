#include "RasterThumbnailProcessorImpl.h"

namespace auge
{
	RasterThumbnailProcessorImpl::RasterThumbnailProcessorImpl()
	{

	}

	RasterThumbnailProcessorImpl::~RasterThumbnailProcessorImpl()
	{

	}

	void RasterThumbnailProcessorImpl::SetInputRaster(const char* rasterPath)
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

	void RasterThumbnailProcessorImpl::SetOutputRaster(const char* rasterPath)
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

	const char*	RasterThumbnailProcessorImpl::GetInputRaster()
	{
		return m_in_raster_path.empty() ? NULL : m_in_raster_path.c_str();
	}

	const char*	RasterThumbnailProcessorImpl::GetOutputRaster()
	{
		return m_out_raster_path.empty() ? NULL : m_out_raster_path.c_str();
	}

	RESULTCODE RasterThumbnailProcessorImpl::Execute()
	{
		return AG_SUCCESS;
	}

	void RasterThumbnailProcessorImpl::Release()
	{
		delete this;
	}

	void RasterThumbnailProcessorImpl::SetUser(g_uint user)
	{
		m_user = user;
	}

}