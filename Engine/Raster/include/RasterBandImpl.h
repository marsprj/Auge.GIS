#ifndef __AUGE_RASTER_BAND_IMPL_H__
#define __AUGE_RASTER_BAND_IMPL_H__

#include "AugeRaster.h"
#include <gdal/gdal_priv.h>

namespace auge
{
	class RasterImpl;

	class RasterBandImpl : public RasterBand
	{
	public:
		RasterBandImpl();
		virtual ~RasterBandImpl();
	public:
		virtual g_uint			GetWidth();
		virtual g_uint			GetHeight();
		virtual GEnvelope&		GetExtent();

		virtual augePixelType	GetPixelType();
		virtual g_uint			GetPixelSize();

		virtual g_uint			GetDataSize();
		virtual void*			GetData();

		virtual void*			GetData(int x, int y);
		virtual void*			GetData(double x, double y);
		
		virtual RESULTCODE		Read(void* buffer, g_uint x, g_uint y, g_int width, g_uint height);

		virtual void			Release();

	public:
		void	Create(GDALRasterBand*	poRasterBand, RasterImpl* pRaster);

	private:
		g_uint	GetPixelSize(GDALDataType type);

	private:
		RasterImpl*		m_pRaster;
		GDALRasterBand*	m_poRasterBand;
		void*			m_data;
		g_uint			m_pixel_size;
		g_uint			m_data_size;
	};

}


#endif //__AUGE_RASTER_BAND_IMPL_H__
