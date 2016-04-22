#ifndef __AUGE_DEM_INUNDATION_IMPL_H__
#define __AUGE_DEM_INUNDATION_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class DemInundationProcessorImpl : public DemInundationProcessor
	{
	public:
		DemInundationProcessorImpl();
		virtual ~DemInundationProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);
		virtual void		SetInputPath(const char* rasterPath);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);

		virtual void		SetHeight(float height);

		virtual RESULTCODE	Execute();
		virtual Raster*		Inundate(Raster* pinRaster, float height);
		virtual void		Release();

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();
		const char*			GetInputRasterPath();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();

	private:
		RESULTCODE			Inundate_Byte(RasterBand* pinBand, RasterBand* poutBand, g_byte water_height);
		RESULTCODE			Inundate_Short(RasterBand* pinBand, RasterBand* poutBand, g_int16 water_height);
		RESULTCODE			Inundate_Float(RasterBand* pinBand, RasterBand* poutBand, float water_height);
		RESULTCODE			Inundate_Double(RasterBand* pinBand, RasterBand* poutBand, double water_height);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		float		m_height;

		g_uint				m_user;
	};
}


#endif //__AUGE_DEM_INUNDATION_IMPL_H__
