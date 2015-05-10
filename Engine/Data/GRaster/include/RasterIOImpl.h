#ifndef __AUGE_RASTER_IO_IMPL_H__
#define __AUGE_RASTER_IO_IMPL_H__

#include "AugeRaster.h"

namespace auge
{
	class RasterIOImpl : public RasterIO
	{
	public:
		RasterIOImpl();
		virtual ~RasterIOImpl();
	public:
		virtual Raster*	Read(const char* path);
		virtual Raster*	Read(const char* path, const char* name);
	};
}

#endif //__AUGE_RASTER_IO_IMPL_H__
