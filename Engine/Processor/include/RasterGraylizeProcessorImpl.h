#ifndef __AUGE_RASTER_GRAY_IMPL_H__
#define __AUGE_RASTER_GRAY_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class RasterGraylizeProcessorImpl : public RasterGraylizeProcessor
	{
	public:
		RasterGraylizeProcessorImpl();
		virtual ~RasterGraylizeProcessorImpl();
	public:
		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();

		bool				Greylize(Raster* pRaster);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
	};

}


#endif //__AUGE_RASTER_GRAY_IMPL_H__
