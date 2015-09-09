#ifndef __AUGE_RASTER_CLIP_PROCESS_IMPL_H__
#define __AUGE_RASTER_CLIP_PROCESS_IMPL_H__

#include "AugeProcessor.h"
#include "AugeGeometry.h"
#include <vector>

namespace auge
{
	class Raster;
	class GEnvelope;
	class FeatureClass;
	class GFilter;
	class Geometry;

	class RasterClipProcessorImpl  : public RasterClipProcessor
	{
	public:
		RasterClipProcessorImpl();
		virtual ~RasterClipProcessorImpl();
	public:
		virtual void		SetUser(g_uint user);

		virtual void		SetInputRasterSource(const char* sourceName);
		virtual void		SetInputRaster(const char* rasterName);
		virtual void		SetInputPath(const char* rasterPath);

		virtual	void		SetInputFeatureSource(const char* sourceName);
		virtual	void		SetInputFeatureClass(const char* className);
		virtual void		SetInputFilter(GFilter* pFilter);

		virtual void		SetOutpuRasteraSource(const char* sourceName);
		virtual void		SetOutputRaster(const char* rasterName);
		virtual void		SetOutputPath(const char* rasterPath);

		virtual RESULTCODE	Execute();
		virtual void		Release();

	private:
		const char*			GetInputRasterSource();
		const char*			GetInputRaster();
		const char*			GetInputRasterPath();

		const char*			GetInputFeatureSource();
		const char*			GetInputFeatureClass();
		GFilter*			GetInputFilter();


		const char*			GetOutputRasterSource();
		const char*			GetOutputRaster();
		const char*			GetOutputRasterPath();

		Raster*				Clip(Raster* pinRaster, GEnvelope& extent, FeatureClass* pinFeatureClass, GFilter* pFilter);
		RESULTCODE			Clip(Raster* poutRaster, Raster* pinRaster, GEnvelope& extent, Geometry* pGeometry);
		RESULTCODE			Scan(g_uint scanline, Raster* poutRaster, Raster* pinRaster,  GRect& rect, GEnvelope& extent, Geometry* pGeometry);
		RESULTCODE			ComputeScanlineIntersects(double* xx, g_uint& xc, double mapY, GEnvelope& extent, Geometry* pGeometry);
		RESULTCODE			ComputeScanlineIntersects(double* xx, g_uint& xc, double mapY, GEnvelope& extent, WKBPolygon* pWKBPolygon);
		RESULTCODE			ComputeScanlineIntersects(double* xx, g_uint& xc, double mapY, GEnvelope& extent, WKBMultiPolygon* pWKBMultiPolygon);

		RESULTCODE			ClipScanLine(Raster* poutRaster, Raster* pinRaster, g_uint sx, g_uint ex, g_uint y);
		RESULTCODE			ClipScanLine_Int16(Raster* poutRaster, Raster* pinRaster, g_uint sx, g_uint ex, g_uint y);
	private:
		std::string	m_in_raster_source;
		std::string	m_in_raster_name;
		std::string m_in_raster_path;

		std::string	m_in_feature_source;
		std::string	m_in_feature_name;

		std::string	m_out_raster_source;
		std::string	m_out_raster_name;
		std::string m_out_raster_path;

		GFilter* m_pFilter;

		g_uint	m_user;
	};
}

#endif //__AUGE_RASTER_CLIP_PROCESS_IMPL_H__
