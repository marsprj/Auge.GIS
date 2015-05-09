#ifndef __AUGE_RASTER_IMPL_H__
#define __AUGE_RASTER_IMPL_H__

#include "AugeRaster.h"
#include "AugeGeometry.h"
#include <gdal/gdal_priv.h>
#include <string>
#include <vector>

namespace auge
{
	class RasterBandImpl;
	class WorkspaceRaster;

	class RasterImpl : public Raster
	{
	public:
		RasterImpl();
		virtual ~RasterImpl();
	public:
		virtual const char*		GetName();
		virtual augeDataSetType	GetType();

		virtual g_uint			GetWidth();
		virtual g_uint			GetHeight();
		virtual g_uint			GetBandCount();
		virtual RasterBand*		GetBand(g_uint i);

		virtual augePixelType	GetPixelType();
		virtual g_uint			GetPixelSize();

		virtual GEnvelope&		GetExtent();
		virtual bool			GetMapPosition(g_uint rx, g_uint ry, double& mx, double& my);
		virtual bool			GetRasterPosition(double mx, double my, g_uint& rx, g_uint& ry);

		virtual const char*		GetPath();

	public:
		void	Create(const char* name, GDALDataset* poDataset, WorkspaceRaster* pWorkspace, const char* path);
		g_uint	GetPixelSize(GDALDataType type);

	private:
		void	Cleanup();

	private:
		std::string  m_name;
		std::string  m_path;

		GDALDataset* m_poDataset;
		GEnvelope	 m_extent;
		WorkspaceRaster* m_pWorkspace;

		g_uint		 m_pixel_size;
		double		 m_geo_transform[6];

		std::vector<RasterBandImpl*>	m_bands;
	};
}

#endif //__AUGE_RASTER_IMPL_H__
