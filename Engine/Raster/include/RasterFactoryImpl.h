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
		virtual Raster*			CreateRaster(const char* name, augePixelType pixelType, GEnvelope& extent, Raster* pinRaster);
		virtual Raster*			CreateRaster(const char* name, augePixelType pixelType, g_uint bands, GEnvelope& extent, g_uint width, g_uint height, const char*  spatialRef);
		virtual Raster*			CreateRaster(const char* name, const char* path, const char* format, augePixelType pixelType, g_uint bands, GEnvelope& extent, g_uint width, g_uint height, const char*  spatialRef);
		
		virtual Raster*			CreateRasterPNG(const char* name, g_uint width, g_uint height);
		virtual Raster*			CreateRasterJPG(const char* name, g_uint width, g_uint height);
		virtual Raster*			CreateRaster(const char* name, augePixelType pixelType, g_uint width, g_uint height, g_int bands, g_uint srid, GEnvelope& extent);
		virtual Raster*			CreateRaster(const char* name, const char* alias, const char* format, const char* path, g_uint bands, g_int srid, g_uint width, g_uint height, double xmin, double ymin, double xmax, double ymax, const char* uuid);
		virtual Raster*			CreateRaster(const char* name, const char* alias, const char* format, const char* path, g_uint bands, g_int srid, g_uint width, g_uint height, double xmin, double ymin, double xmax, double ymax, const char* uuid,double size, const char* unit);
		

		virtual const char*		Encoding(augePixelType type);
	};
}

#endif //__AUGE_RASTER_FACTORY_IMPL_H__
