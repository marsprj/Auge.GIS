#include "EnumRasterImpl.h"

namespace auge
{
	EnumRasterImpl::EnumRasterImpl()
	{
		m_iter = m_rasters.begin();
	}

	EnumRasterImpl::~EnumRasterImpl()
	{
		Raster* pRaster = NULL;
		std::vector<Raster*>::iterator iter;
		for(iter=m_rasters.begin(); iter!=m_rasters.end(); iter++)
		{
			pRaster = *iter;
			pRaster->Release();
		}
		m_rasters.clear();
	}

	void EnumRasterImpl::Reset()
	{
		m_iter = m_rasters.begin();
	}

	Raster* EnumRasterImpl::Next()
	{
		if(m_iter==m_rasters.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	bool EnumRasterImpl::Add(Raster* pRaster)
	{
		if(pRaster)
		{
			m_rasters.push_back(pRaster);
		}
		return true;
	}

	void EnumRasterImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}