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
		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);
		virtual void		SetInputPath(const char* rasterPath);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);
		
		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();
		const char*			GetInputRasterPath();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();

		bool				Greylize(Raster* pRaster);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		g_uint	m_user;
	};

}


#endif //__AUGE_RASTER_GRAY_IMPL_H__
