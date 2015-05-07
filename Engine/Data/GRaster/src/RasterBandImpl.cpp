#include "RasterBandImpl.h"

namespace auge
{	
	RasterBandImpl::RasterBandImpl()
	{
		m_poRasterBand = NULL;
	}

	RasterBandImpl::~RasterBandImpl()
	{

	}

	g_uint RasterBandImpl::GetWidth()
	{
		return m_poRasterBand->GetXSize();
	}

	g_uint RasterBandImpl::GetHeight()
	{
		return m_poRasterBand->GetYSize();
	}

	augePixelType RasterBandImpl::GetPixelType()
	{
		return (augePixelType)m_poRasterBand->GetRasterDataType();
	}

	void RasterBandImpl::Create(GDALRasterBand*	poRasterBand)
	{
		m_poRasterBand = poRasterBand;
	}

	void RasterBandImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}