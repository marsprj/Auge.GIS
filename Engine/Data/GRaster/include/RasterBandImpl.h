#ifndef __AUGE_RASTER_BAND_IMPL_H__
#define __AUGE_RASTER_BAND_IMPL_H__

#include "AugeRaster.h"
#include <gdal/gdal_priv.h>

namespace auge
{
	class RasterBandImpl : public RasterBand
	{
	public:
		RasterBandImpl();
		virtual ~RasterBandImpl();
	public:
		virtual g_uint			GetWidth();
		virtual g_uint			GetHeight();
		virtual augePixelType	GetPixelType();
		virtual void			Release();

	public:
		void	Create(GDALRasterBand*	poRasterBand);
	private:
		GDALRasterBand*	m_poRasterBand;
	};

}


#endif //__AUGE_RASTER_BAND_IMPL_H__
