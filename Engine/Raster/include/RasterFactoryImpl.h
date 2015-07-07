#ifndef __AUGE_RASTER_FACTORY_IMPL_H__
#define __AUGE_RASTER_FACTORY_IMPL_H__

#include "AugeRaster.h"

namespace auge
{
	class RasterFactoryImpl : public RasterFactory
	{
	public:
		RasterFactoryImpl();
		virtual ~RasterFactoryImpl();
	public:
		virtual Raster*			CreateRaster(const char* name, GEnvelope& extent, Raster* pinRaster);
		virtual Raster*			CreateRasterPNG(const char* name, g_uint width, g_uint height);
		virtual Raster*			CreateRasterJPG(const char* name, g_uint width, g_uint height);
		virtual Raster*			CreateRaster(const char* name, augePixelType pixelType, g_uint width, g_uint height, g_int bands, g_uint srid, GEnvelope& extent);
	};
}

#endif //__AUGE_RASTER_FACTORY_IMPL_H__
