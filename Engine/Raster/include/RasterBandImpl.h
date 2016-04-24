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

		virtual double			GetResolution_X();
		virtual double			GetResolution_Y();

		virtual augePixelType	GetPixelType();
		virtual g_uint			GetPixelSize();

		virtual g_uint			GetDataSize();
		virtual void*			GetData();

		virtual void*			GetData(int x, int y);
		virtual void*			GetData(double x, double y);

		virtual RESULTCODE		GetData(void* data, GRect rect);

		virtual bool			SetData(void* data);

		virtual bool			GetMinMaxValue(g_byte& minv, g_byte& maxv);
		virtual bool			GetMinMaxValue(short& minv, short& maxv);
		virtual bool			GetMinMaxValue(int& minv, int& maxv);
		virtual bool			GetMinMaxValue(float& minv, float& maxv);
		virtual bool			GetMinMaxValue(double& minv, double& maxv);
		
		virtual RESULTCODE		Read(void* buffer, g_uint x, g_uint y, g_int width, g_uint height);
		virtual RESULTCODE		Write(void* buffer, g_uint x, g_uint y, g_int width, g_uint height);

		virtual g_uint			GetPixelSize(GDALDataType type);

		virtual void			Release();

	public:
		void	Create(GDALRasterBand*	poRasterBand, RasterImpl* pRaster);

	private:
		

	private:
		RasterImpl*		m_pRaster;
		GDALRasterBand*	m_poRasterBand;
		void*			m_data;
		g_uint			m_pixel_size;
		g_uint			m_data_size;
	};

	const char* auge_raster_get_driver(const char* ext);
}


#endif //__AUGE_RASTER_BAND_IMPL_H__
