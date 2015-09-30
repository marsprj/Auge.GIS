#ifndef __AUGE_RASTER_THUMBNAIL_IMPL_H__
#define __AUGE_RASTER_THUMBNAIL_IMPL_H__

#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterFormatConvertToJPEGProcessorImpl : public RasterFormatConvertToJPEGProcessor
	{
	public:
		RasterFormatConvertToJPEGProcessorImpl();
		virtual ~RasterFormatConvertToJPEGProcessorImpl();

	public:
		virtual void		SetUser(g_uint user);
		virtual void		SetInputRectangle(GEnvelope& rect);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);
		virtual void		SetInputPath(const char* rasterPath);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);

		virtual void		SetRed(g_uint i);
		virtual void		SetGreen(g_uint i);
		virtual void		SetBlue(g_uint i);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		RESULTCODE			Fill  (Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_1(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_2(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_3(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_4(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);

		RESULTCODE			Fill_Int16(Raster* poutRaster, RasterBand* pinBand, GRect rastertRect);

		RESULTCODE			Fill(RasterBand* poutBand, RasterBand* pinBand, GRect rastertRect);
		RESULTCODE			Fill_Byte(RasterBand* poutBand, RasterBand* pinBand, GRect rastertRect);
		RESULTCODE			Fill_Int16(RasterBand* poutBand, RasterBand* pinBand, GRect rastertRect);

		void				GetMinMax(short& i_min, short& i_max, short* i_data, int w, int h);

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();
		const char*			GetInputPath();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();
		const char*			GetOutputPath();

		//const char*			GetOutputPath();

	private:
		GEnvelope	m_rect;

		std::string	m_in_source_name;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		g_uint		m_red;
		g_uint		m_green;
		g_uint		m_blue;

		g_uint		m_user;
	};

}


#endif //__AUGE_RASTER_THUMBNAIL_IMPL_H__
