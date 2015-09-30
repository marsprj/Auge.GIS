#ifndef __AUGE_RASTER_THUMBNAIL_IMPL_H__
#define __AUGE_RASTER_THUMBNAIL_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterThumbnailProcessorImpl : public RasterThumbnailProcessor
	{
	public:
		RasterThumbnailProcessorImpl();
		virtual ~RasterThumbnailProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);
		virtual void		SetInputRaster(const char* rasterPath);
		virtual void		SetOutputRaster(const char* rasterPath);
		virtual RESULTCODE	Execute();
		virtual void		Release();

	public:
		const char*			GetInputRaster();
		const char*			GetOutputRaster();

	private:
		std::string m_in_raster_path;
		std::string m_out_raster_path;
		g_uint		m_user;
	};
}

#endif //__AUGE_RASTER_THUMBNAIL_IMPL_H__
