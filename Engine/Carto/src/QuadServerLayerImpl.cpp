#include "QuadServerLayerImpl.h"

namespace auge
{
	QuadServerLayerImpl::QuadServerLayerImpl()
	{
		m_srid = 4326;
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

}