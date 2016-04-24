#ifndef __AUGE_RASTER_MOSIAC_PROCESS_IMPL_H__
#define __AUGE_RASTER_MOSIAC_PROCESS_IMPL_H__

#include "AugeProcessor.h"
#include "AugeRaster.h"
#include <vector>

namespace auge
{
	class Raster;
	class GEnvelope;
	class FeatureClass;
	class GFilter;
	class Geometry;
	
	class RasterMosiacProcessorImpl  : public RasterMosiacProcessor
	{
	public:
		RasterMosiacProcessorImpl();
		virtual ~RasterMosiacProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		AddInputRaster(const char* sourceName, const char* rasterPath, const char* rasterName);

		virtual void		SetOutpuRasteraSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);

		virtual RESULTCODE	Execute();
		virtual void		Release();

		virtual Raster*		Mosiac(char** rasters, g_uint count, const char* poutRasterPath);

	private:
		//const char*			GetInputRasterSource();
		//const char*			GetInputRaster();
		//const char*			GetInputRasterPath();

		const char*			GetOutputRasterSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();

	private:
		void				CleanupRasters(Raster** ppinRasters, g_uint count);
		bool				ComputeMosiacExtent(GEnvelope& mextent, Raster** ppinRasters, g_uint count);
		augePixelType		GetPixelType(Raster** ppRaster, g_uint count);
		Raster*				GetRaster0(Raster** ppRaster, g_uint count);
		bool				MosiacCheck(Raster** ppinRasters, g_uint count);
		RESULTCODE			Mosiac(Raster* poutRaster, Raster* pinRaster);
		RESULTCODE			CopyBand(Raster* poutRaster, RasterBand* poutBand, RasterBand* pinBand);

		RESULTCODE			MosiacMem(Raster* pinRaster, Raster* poutRaster, void* buffer, g_uint64 buffer_size);
		RESULTCODE			CopyBandMem(RasterBand* poutBand, RasterBand* pinBand, Raster* poutRaster, void* buffer);

	private:
		std::vector<std::string>	m_in_raster_source;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_in_feature_source;
		std::string	m_in_feature_name;

		std::string	m_out_raster_source;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		g_uint	m_user;
	};
}

#endif //__AUGE_RASTER_MOSIAC_PROCESS_IMPL_H__
