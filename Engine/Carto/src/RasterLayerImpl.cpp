#include "RasterLayerImpl.h"

namespace auge
{
	RasterLayerImpl::RasterLayerImpl():
	m_srid(4326)
	{
		
	}

	RasterLayerImpl::~RasterLayerImpl()
	{

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
		return NULL;
	}

	void RasterLayerImpl::SetName(const char* szName)
	{
		if(szName!=NULL)
		{
			m_name = szName;
		}
	}

	GEnvelope& RasterLayerImpl::GetExtent()
	{
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
}