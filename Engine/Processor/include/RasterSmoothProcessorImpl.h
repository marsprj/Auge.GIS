#ifndef __AUGE_RASTER_SMOOTH_IMPL_H__
#define __AUGE_RASTER_SMOOTH_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterSmoothProcessorImpl : public RasterSmoothProcessor
	{
	public:
		RasterSmoothProcessorImpl();
		virtual ~RasterSmoothProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);

		virtual void		SetSmoother(augeRasterSmoother smoother);
		
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();

	private:
		Raster*				Smooth(Raster* pinRaster, int filter[][3], int denominator);
		RESULTCODE			Smooth_Byte(RasterBand* pinBand, RasterBand* poutBand, int filter[][3], int denominator);

		Raster*				Median(Raster* pinRaster);
		RESULTCODE			Median_Byte(RasterBand* pinBand, RasterBand* poutBand);
		g_byte				GetMedian(g_byte* values, g_uint size);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;

		augeRasterSmoother m_smoother;

		g_uint		m_user;
	};

}


#endif //__AUGE_RASTER_SMOOTH_IMPL_H__
