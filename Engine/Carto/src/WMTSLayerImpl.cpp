#include "WMTSLayerImpl.h"

namespace auge
{
	WMTSLayerImpl::WMTSLayerImpl()
	{
		m_srid = AUGE_DEFAULT_SRID;
		m_min_scale = -1.0;
		m_max_scale = -1.0;
	}

	WMTSLayerImpl::~WMTSLayerImpl()
	{

	}

	const char*	WMTSLayerImpl::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void WMTSLayerImpl::SetName(const char* szName)
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

	augeLayerType WMTSLayerImpl::GetType()
	{
		return augeLayerWMTS;
	}

	g_int WMTSLayerImpl::GetSRID()
	{
		return m_srid;
	}

	GEnvelope& WMTSLayerImpl::GetExtent()
	{
		return m_extent;
	}

	bool WMTSLayerImpl::IsQueryable()
	{
		return false;
	}

	void WMTSLayerImpl::SetQueryable(bool flag)
	{
		
	}

	bool WMTSLayerImpl::IsVisiable()
	{
		return m_visible;
	}

	bool WMTSLayerImpl::IsVisiable(double scale)
	{
		if(scale<0)
		{
			return true;
		}

		return ((scale>m_min_scale) && (scale<m_max_scale));
	}

	void WMTSLayerImpl::SetVisiable(bool flag)
	{
		m_visible = flag;
	}

	const char*	WMTSLayerImpl::GetURL()
	{
		return m_url.empty() ? NULL : m_url.c_str();
	}

	RESULTCODE WMTSLayerImpl::SetURL(const char* url)
	{
		if(url==NULL)
		{
			m_url.clear();
			return AG_FAILURE;
		}
		m_url = url;
		return AG_SUCCESS;
	}

	double WMTSLayerImpl::GetMinScale()
	{
		return m_min_scale;
	}

	void WMTSLayerImpl::SetMinScale(double scale)
	{
		m_min_scale = scale;
	}

	double WMTSLayerImpl::GetMaxScale()
	{
		return m_max_scale;
	}

	void WMTSLayerImpl::SetMaxScale(double scale)
	{
		m_max_scale = scale;
	}

}