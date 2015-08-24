#include "QuadServerLayerImpl.h"

namespace auge
{
	QuadServerLayerImpl::QuadServerLayerImpl()
	{
		m_srid = AUGE_DEFAULT_SRID;
		m_min_scale = -1.0;
		m_max_scale = -1.0;
	}

	QuadServerLayerImpl::~QuadServerLayerImpl()
	{

	}

	const char*	QuadServerLayerImpl::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void QuadServerLayerImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = szName;
		}
	}

	augeLayerType QuadServerLayerImpl::GetType()
	{
		return augeLayerQuadServer;
	}

	g_int QuadServerLayerImpl::GetSRID()
	{
		return m_srid;
	}

	GEnvelope& QuadServerLayerImpl::GetExtent()
	{
		return m_extent;
	}

	bool QuadServerLayerImpl::IsQueryable()
	{
		return false;
	}

	void QuadServerLayerImpl::SetQueryable(bool flag)
	{
		
	}

	bool QuadServerLayerImpl::IsVisiable()
	{
		return m_visible;
	}

	bool QuadServerLayerImpl::IsVisiable(double scale)
	{
		if(scale<0)
		{
			return true;
		}

		return ((scale>m_min_scale) && (scale<m_max_scale));
	}

	void QuadServerLayerImpl::SetVisiable(bool flag)
	{
		m_visible = flag;
	}

	const char*	QuadServerLayerImpl::GetURL()
	{
		return m_url.empty() ? NULL : m_url.c_str();
	}

	RESULTCODE QuadServerLayerImpl::SetURL(const char* url)
	{
		if(url==NULL)
		{
			m_url.clear();
			return AG_FAILURE;
		}
		m_url = url;
		return AG_SUCCESS;
	}

	double QuadServerLayerImpl::GetMinScale()
	{
		return m_min_scale;
	}

	void QuadServerLayerImpl::SetMinScale(double scale)
	{
		m_min_scale = scale;
	}

	double QuadServerLayerImpl::GetMaxScale()
	{
		return m_max_scale;
	}

	void QuadServerLayerImpl::SetMaxScale(double scale)
	{
		m_max_scale = scale;
	}

}