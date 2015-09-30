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
		RESULTCODE			Fill  (Raster* poutRaster, Raster* pinRaster);
		RESULTCODE			Fill_1(Raster* poutRaster, Raster* pinRaster);
		RESULTCODE			Fill_2(Raster* poutRaster, Raster* pinRaster);
		RESULTCODE			Fill_3(Raster* poutRaster, Raster* pinRaster);
		RESULTCODE			Fill_4(Raster* poutRaster, Raster* pinRaster);

		RESULTCODE			Fill_Int16(Raster* poutRaster, RasterBand* pinBand);

		RESULTCODE			Fill(RasterBand* poutBand, RasterBand* pinBand);
		RESULTCODE			Fill_Byte(RasterBand* poutBand, RasterBand* pinBand);
		RESULTCODE			Fill_Int16(RasterBand* poutBand, RasterBand* pinBand);

		void				GetMinMax(short& i_min, short& i_max, short* i_data, int w, int h);
		float				GetScale(g_uint w, g_uint h);

	private:
		std::string m_in_raster_path;
		std::string m_out_raster_path;
		
		g_uint		m_red;
		g_uint		m_green;
		g_uint		m_blue;

		float		m_step;

		g_uint		m_user;
	};
}

#endif //__AUGE_RASTER_THUMBNAIL_IMPL_H__
