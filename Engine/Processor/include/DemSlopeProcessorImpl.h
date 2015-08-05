#ifndef __AUGE_DEM_SLOPE_IMPL_H__
#define __AUGE_DEM_SLOPE_IMPL_H__

#include "AugeCore.h"
#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <string>

namespace auge
{
	class DemSlopeProcessorImpl : public DemSlopeProcessor
	{
	public:
		DemSlopeProcessorImpl();
		virtual ~DemSlopeProcessorImpl();
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

	private:
		Raster*				Slope(Raster* pinRaster);
		RESULTCODE			Slope_Byte(RasterBand* pinBand, RasterBand* poutBand);
		RESULTCODE			Slope_Short(RasterBand* pinBand, RasterBand* poutBand);
		RESULTCODE			Slope_Double(RasterBand* pinBand, RasterBand* poutBand);

	private:
		std::string	m_in_source_name;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_out_source_name;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;
		
		g_uint				m_user;
	};

}


#endif //__AUGE_DEM_SLOPE_IMPL_H__
