#ifndef __AUGE_DEM_HILL_SHADE_IMPL_H__
#define __AUGE_DEM_HILL_SHADE_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class DemHillshadeProcessorImpl : public DemHillshadeProcessor
	{
	public:
		DemHillshadeProcessorImpl();
		virtual ~DemHillshadeProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		SetInputDataSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);
		virtual void		SetInputPath(const char* rasterPath);

		virtual void		SetOutputDataSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);

		virtual void		SetAzimuth(float azimuth);		//太阳方位角
		virtual void		SetZenith(float zenith);		//太阳高度角
		virtual void		SetZFactor(float zscale);		//Z比例因子

		virtual RESULTCODE	Execute();
		virtual void		Release();

		virtual Raster*		Hillshade(Raster* pinRaster);

	private:
		const char*			GetInputDataSource();
		const char*			GetInputRaster();
		const char*			GetInputRasterPath();

		const char*			GetOutputDataSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();

	private:
		
		RESULTCODE			Hillshade_Byte(RasterBand* pinBand, RasterBand* poutBand);
		RESULTCODE			Hillshade_Short(RasterBand* pinBand, RasterBand* poutBand);
		RESULTCODE			Hillshade_Double(RasterBand* pinBand, RasterBand* poutBand);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		float		m_azimuth;
		float		m_zenith;
		float		m_zfactor;

		g_uint				m_user;
	};
}


#endif //__AUGE_DEM_HILL_SHADE_IMPL_H__
