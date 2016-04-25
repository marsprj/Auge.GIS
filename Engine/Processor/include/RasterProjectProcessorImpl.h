#ifndef __AUGE_RASTER_PROJECT_PROCESSOR_H__
#define __AUGE_RASTER_PROJECT_PROCESSOR_H__

#include <string>
#include "AugeProcessor.h"

#include "proj_api.h"
#include "SrsBase.h"

namespace auge
{
	class RasterWorkspace;

	class RasterProjectProcessorImpl : public RasterProjectProcessor
	{
	public:
		RasterProjectProcessorImpl();
		virtual ~RasterProjectProcessorImpl();
	public:

		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);
		virtual void		SetInputPath(const char* rasterPath);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);
		virtual	void		SetOutputSrid(g_uint srid);

		virtual RESULTCODE	Execute();
		virtual void		Release();

		virtual Raster*		Project(Raster* pRaster, g_uint srid);
		virtual RESULTCODE	Project(const char* inpath, const char* outpath, g_uint out_sird);

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();
		const char*			GetInputRasterPath();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();
		g_uint				GetOutputSrid();
		
	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		g_uint		m_out_srid;

		g_uint		m_user;
	};
}

#endif //__AUGE_RASTER_PROJECT_PROCESSOR_H__
