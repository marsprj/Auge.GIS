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

	augeDataSetType	RasterDatasetImpl::GetType()
	{	
		return augeDataSetRaster;
	}

	Raster*	RasterDatasetImpl::GetRaster()
	{
		//if(m_pRaster==NULL)
		//{
		//	RasterIO* pRasterIO = augeGetRasterIOInstance();
		//	m_pRaster = pRasterIO->Read(m_path.c_str());
		//}
		return m_pRaster;
	}

	const char* RasterDatasetImpl::GetName()
	{
		return m_pRaster->GetName();
	}

	const char*	RasterDatasetImpl::GetAlias()
	{
		return m_pRaster->GetAlias();
	}

	const char* RasterDatasetImpl::GetFormat()
	{
		return m_pRaster->GetFormat();
	}

	g_uint RasterDatasetImpl::GetWidth()
	{
		return m_pRaster->GetWidth();
	}

	g_uint RasterDatasetImpl::GetHeight()
	{
		return m_pRaster->GetHeight();
	}

	g_int RasterDatasetImpl::GetSRID()
	{
		return m_pRaster->GetSRID();
	}

	g_uint RasterDatasetImpl::GetBandCount()
	{
		return m_pRaster->GetBandCount();
	}

	GEnvelope& RasterDatasetImpl::GetExtent()
	{
		return m_pRaster->GetExtent();
	}

	const char*	RasterDatasetImpl::GetPath()
	{
		return m_pRaster->GetPath();
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
