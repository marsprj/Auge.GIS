#include "RasterImpl.h"

namespace auge
{
	RasterImpl::RasterImpl()
	{
		m_poDataset = NULL;
		m_pWorkspace = NULL;
		memset(m_geo_transform, 0, sizeof(double)*6);
	}

	RasterImpl::~RasterImpl()
	{
		if(m_poDataset!=NULL)
		{
			GDALClose(m_poDataset);
			m_poDataset = NULL;
		}
	}

	const char* RasterImpl::GetName()
	{
		return m_name.c_str();
	}

	augeDataSetType RasterImpl::GetType()
	{
		return augeDataSetRaster;
	}

	g_uint RasterImpl::GetWidth()
	{
		return m_poDataset->GetRasterXSize();
	}

	g_uint RasterImpl::GetHeight()
	{
		return m_poDataset->GetRasterYSize();
	}

	g_uint RasterImpl::GetBandCount()
	{
		return m_poDataset->GetRasterCount();
	}

	augePixelType RasterImpl::GetPixelType()
	{
		if(m_poDataset==NULL)
		{
			return augePixelUnknown;
		}

		int nBands = m_poDataset->GetRasterCount();
		if(nBands==0)
		{
			return augePixelUnknown;
		}

		GDALRasterBand* poBand = m_poDataset->GetRasterBand(0);
		return (augePixelType)poBand->GetRasterDataType();
	}

	GEnvelope& RasterImpl::GetExtent()
	{
		return m_extent;
	}

	void RasterImpl::Create(const char* name, GDALDataset* poDataset, WorkspaceRaster* pWorkspace, const char* path)
	{
		m_name = name;
		m_path = path;
		m_poDataset = poDataset;
		m_pWorkspace = pWorkspace;

		m_poDataset->GetGeoTransform(m_geo_transform);

		double x0 = m_geo_transform[0];
		double y0 = m_geo_transform[3];
		double x1 = x0 + m_geo_transform[1] * m_poDataset->GetRasterXSize();
		double y1 = y0 + m_geo_transform[5] * m_poDataset->GetRasterYSize();
		m_extent.Set(x0,y0,x1,y1);
	}

	const char*	RasterImpl::GetPath()
	{
		return m_path.c_str();
	}
}