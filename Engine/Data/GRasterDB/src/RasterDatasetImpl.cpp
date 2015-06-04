#include "RasterDatasetImpl.h"
#include "AugeRaster.h"
#include "AugeCore.h"

namespace auge
{
	RasterDatasetImpl::RasterDatasetImpl()
	{
		m_pRaster = NULL;
	}

	RasterDatasetImpl::~RasterDatasetImpl()
	{
		if(m_pRaster!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRaster);
		}
	}

	augeDataSetType	RasterDatasetImpl::GetType()
	{	
		return augeDataSetRaster;
	}

	Raster*	RasterDatasetImpl::GetRaster()
	{
		if(m_pRaster==NULL)
		{
			RasterIO* pRasterIO = augeGetRasterIOInstance();
			m_pRaster = pRasterIO->Read(m_path.c_str());
		}
		return m_pRaster;
	}

	const char* RasterDatasetImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	RasterDatasetImpl::GetAlias()
	{
		return m_alias.c_str();
	}
	
	const char* RasterDatasetImpl::GetFormat()
	{
		return m_format.c_str();
	}

	g_uint RasterDatasetImpl::GetWidth()
	{
		return m_width;
	}

	g_uint RasterDatasetImpl::GetHeight()
	{
		return m_height;
	}

	g_int RasterDatasetImpl::GetSRID()
	{
		return m_srid;
	}

	g_uint RasterDatasetImpl::GetBandCount()
	{
		return m_nband;
	}

	GEnvelope& RasterDatasetImpl::GetExtent()
	{
		return m_extent;
	}

	const char*	RasterDatasetImpl::GetPath()
	{
		return m_path.c_str();
	}

	//void RasterDatasetImpl::Create(Raster* pRaster)
	//{
	//	if(m_pRaster!=NULL)
	//	{
	//		AUGE_SAFE_RELEASE(m_pRaster);
	//	}
	//	m_pRaster = pRaster;
	//}

	void RasterDatasetImpl::Create(WorkspaceRasterDB* pWorkspace,g_uint gid,const char* name,const char* alias,const char* format,const char* path,g_uint nband,g_int srid,g_uint width,g_uint height,double xmin,double ymin,double xmax,double ymax,const char* uuid)
	{
		m_id = gid;
		m_nband = nband;
		m_srid = srid;
		m_width = width;
		m_height = height;
		m_name = name;
		m_alias = alias;
		m_format = format;
		m_path = path;
		m_uuid = uuid;
		m_extent.Set(xmin, ymin, xmax, ymax);
		m_pWoskspace = pWorkspace;
	}
}
