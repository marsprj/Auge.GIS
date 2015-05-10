#include "RasterDatasetImpl.h"

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

	const char*	RasterDatasetImpl::GetName()
	{
		if(m_pRaster==NULL)
		{
			return NULL;
		}
		return m_pRaster->GetName();
	}
	
	augeDataSetType	RasterDatasetImpl::GetType()
	{	
		return augeDataSetRaster;
	}

	Raster*	RasterDatasetImpl::GetRaster()
	{
		return m_pRaster;
	}

	void RasterDatasetImpl::Create(Raster* pRaster)
	{
		if(m_pRaster!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRaster);
		}
		m_pRaster = pRaster;
	}

}
