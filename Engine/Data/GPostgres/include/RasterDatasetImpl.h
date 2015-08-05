#ifndef __AUGE_RASTER_DATASET_IMPL_H__
#define __AUGE_RASTER_DATASET_IMPL_H__

#include "AugeData.h"

namespace auge
{
	class Raster;
	class RasterFolderImpl;
	class WorkspacePgs;

	class RasterDatasetImpl : public RasterDataset
	{
	public:
		RasterDatasetImpl();
		virtual ~RasterDatasetImpl();
	public:
		virtual const char*			GetName();
		virtual augeDataSetType		GetType();

		virtual EnumRaster*			GetRasters();
		virtual Raster*				GetRaster(const char* name);
		virtual RESULTCODE			AddRaster(const char* name, Raster* pRaster);
		virtual RESULTCODE			AddRaster(const char* name, const char* file_path);
		virtual RESULTCODE			RemoveRaster(const char* name);
		virtual RESULTCODE			RemoveAllRaster();
		
		virtual bool				IsEmpty();
		virtual bool				HasRaster(const char* name);

		//virtual const char*		GetName();
		//virtual const char*		GetAlias();
		
		//virtual g_uint			GetWidth();
		//virtual g_uint			GetHeight();
		//virtual g_uint			GetBandCount();
		//virtual GEnvelope&		GetExtent();
		//virtual g_int			GetSRID();
		//virtual const char*		GetFormat();
		//virtual const char*		GetPath();

		virtual Raster*			GetRaster();

	public:
		void	Create(const char* name, RasterFolderImpl* pFolder, WorkspacePgs* pWorkspace);
		//void	Create( WorkspacePgs* pWorkspace,
		//				g_uint gid,
		//				const char* name,
		//				const char* alias,
		//				const char* format,
		//				const char* path,
		//				g_uint		nband,
		//				g_int		srid,
		//				g_uint		width,
		//				g_uint		height,
		//				double		xmin,
		//				double		ymin,
		//				double		xmax,
		//				double		ymax,
		//				const char* uuid);

	public:
		//Raster*	m_pRaster;

		//g_uint		m_nband;
		//g_int		m_srid;
		//g_uint		m_width;
		//g_uint		m_height;
		//std::string	m_name;
		//std::string	m_alias;
		//std::string	m_format;
		//std::string	m_path;
		//std::string	m_uuid;
		//GEnvelope	m_extent;

		std::string			m_name;
		RasterFolderImpl*	m_pFolder;
		WorkspacePgs*		m_pWoskspace;
	};
}

#endif //__AUGE_RASTER_DATASET_IMPL_H__
