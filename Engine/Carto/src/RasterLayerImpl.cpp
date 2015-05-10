#include "RasterLayerImpl.h"
#include "AugeRaster.h"

namespace auge
{
	RasterLayerImpl::RasterLayerImpl():
	m_srid(4326),
	m_pRasterDataset(NULL),
	m_pRaster(NULL)
	{
		
	}

	RasterLayerImpl::~RasterLayerImpl()
	{
		if(m_pRaster!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRaster);
		}
		if(m_pRasterDataset!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRasterDataset);
		}
	}

	augeLayerType RasterLayerImpl::GetType()
	{
		return augeLayerRaster;
	}

	void RasterLayerImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}

	const char* RasterLayerImpl::GetName()
	{
		return m_name.c_str();
	}

	void RasterLayerImpl::SetName(const char* szName)
	{
		if(szName)
		{
			m_name = szName;
		}
		else
		{
			m_name.clear();
		}
	}

	GEnvelope& RasterLayerImpl::GetExtent()
	{
		if(m_pRaster)
		{
			m_extent = m_pRaster->GetExtent();
		}
		return m_extent;
	}

	g_int RasterLayerImpl::GetSRID()
	{
		return m_srid;
	}

	bool RasterLayerImpl::IsQueryable()
	{
		return false;
	}

	void RasterLayerImpl::SetQueryable(bool flag)
	{
	}

	bool RasterLayerImpl::IsVisiable()
	{
		return m_visiable;
	}

	void RasterLayerImpl::SetVisiable(bool flag)
	{
		m_visiable = flag;
	}

	RESULTCODE RasterLayerImpl::SetStyle(Style* pStyle)
	{
		return AG_SUCCESS;
	}

	Style* RasterLayerImpl::GetStyle()
	{
		return NULL;
	}

	RESULTCODE RasterLayerImpl::SetRaster(Raster* pRaster)
	{
		if(m_pRaster)
		{
			AUGE_SAFE_RELEASE(m_pRaster);
		}
		m_pRaster = pRaster;
		return AG_SUCCESS;
	}

	Raster* RasterLayerImpl::GetRaster()
	{
		return m_pRaster;
	}

	RESULTCODE RasterLayerImpl::SetRasterDataset(RasterDataset* pRasterDataset)
	{
		if(m_pRasterDataset!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRasterDataset);
		}
		m_pRasterDataset = pRasterDataset;
		return AG_SUCCESS;
	}

	RasterDataset* RasterLayerImpl::GetRasterDataset()
	{
		return m_pRasterDataset;
	}
}