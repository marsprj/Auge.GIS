#ifndef __AUGE_RASTER_IMPL_H__
#define __AUGE_RASTER_IMPL_H__

#include "AugeRaster.h"
#include "AugeGeometry.h"
#include <gdal/gdal_priv.h>
#include <string>

namespace auge
{
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
		virtual augePixelType	GetPixelType();

		virtual GEnvelope&		GetExtent();
		virtual const char*		GetPath();

	public:
		void	Create(const char* name, GDALDataset* poDataset, WorkspaceRaster* pWorkspace, const char* path);

	private:
		std::string  m_name;
		std::string  m_path;

		GDALDataset* m_poDataset;
		GEnvelope	 m_extent;
		WorkspaceRaster* m_pWorkspace;

		double		 m_geo_transform[6];
	};
}

#endif //__AUGE_RASTER_IMPL_H__
