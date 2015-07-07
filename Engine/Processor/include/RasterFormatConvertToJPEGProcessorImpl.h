#ifndef __AUGE_RASTER_FORMAT_CONVERT_TO_JPEG_IMPL_H__
#define __AUGE_RASTER_FORMAT_CONVERT_TO_JPEG_IMPL_H__

#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include <string>

namespace auge
{
	class RasterFormatConvertToJPEGProcessorImpl : public RasterFormatConvertToJPEGProcessor
	{
	public:
		RasterFormatConvertToJPEGProcessorImpl();
		virtual ~RasterFormatConvertToJPEGProcessorImpl();
	public:
		virtual void		SetInputRectangle(GEnvelope& rect);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* path);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		RESULTCODE			Fill  (Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_1(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_2(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_3(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);
		RESULTCODE			Fill_4(Raster* poutRaster, Raster* pinRaster, GRect rastertRect);

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();

		const char*			GetOutputPath();

	private:
		GEnvelope	m_rect;

		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_path;
	};

}


#endif //__AUGE_RASTER_FORMAT_CONVERT_TO_JPEG_IMPL_H__
