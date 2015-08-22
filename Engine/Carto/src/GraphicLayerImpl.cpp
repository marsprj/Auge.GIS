#include "GraphicLayerImpl.h"

namespace auge
{
	GraphicLayerImpl::GraphicLayerImpl()
	{
		m_queryable = true;
		m_visiable = true;
		m_pStyle = NULL;
		m_pFeatureClass = NULL;
		m_srid = AUGE_DEFAULT_SRID;

		m_min_scale = -1.0;
		m_max_scale = -1.0;
	}

	GraphicLayerImpl::~GraphicLayerImpl()
	{
		if(m_pFeatureClass!=NULL)
		{
			m_pFeatureClass->Release();
			m_pFeatureClass = NULL;
		}
		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
			m_pStyle = NULL;
		}
	}

	augeLayerType GraphicLayerImpl::GetType()
	{
		return augeLayerGraphic;
	}

	g_int GraphicLayerImpl::GetSRID()
	{
		return m_srid;
	}

	void GraphicLayerImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}

	const char* GraphicLayerImpl::GetName()
	{
		return m_name.c_str();
	}

	void GraphicLayerImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			m_name.clear();
		}
		m_name = szName;
	}

	RESULTCODE GraphicLayerImpl::SetFeatureClass(FeatureClass* pFeatureClass)
	{
		if(m_pFeatureClass!=NULL)
		{
			m_pFeatureClass->Release();
			m_pFeatureClass = NULL;
		}
		m_pFeatureClass = pFeatureClass;
		//if(m_pFeatureClass!=NULL)
		//{
		//	m_pFeatureClass->AddRef();
		//}
		return AG_SUCCESS;
	}

	FeatureClass* GraphicLayerImpl::GetFeatureClass()
	{
		return m_pFeatureClass;
	}

	GEnvelope& GraphicLayerImpl::GetExtent()
	{
		if(m_pFeatureClass!=NULL)
		{
			m_extent = m_pFeatureClass->GetExtent();
		}
		return m_extent;
	}

	bool GraphicLayerImpl::IsQueryable()
	{
		return m_queryable;
	}

	void GraphicLayerImpl::SetQueryable(bool flag)
	{
		m_queryable = flag;
	}

	bool GraphicLayerImpl::IsVisiable()
	{
		return m_visiable;
	}

	bool GraphicLayerImpl::IsVisiable(double scale)
	{
		if(scale<0)
		{
			return true;
		}

		return ((scale>m_min_scale) && (scale<m_max_scale));
	}

	void GraphicLayerImpl::SetVisiable(bool flag)
	{
		m_visiable = flag;
	}

	RESULTCODE GraphicLayerImpl::SetStyle(Style* pStyle)
	{
		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
		}
		m_pStyle = pStyle;
		return AG_SUCCESS;
	}

	Style* GraphicLayerImpl::GetStyle()
	{
		return m_pStyle;
	}

	double GraphicLayerImpl::GetMinScale()
	{
		return m_min_scale;
	}

	void GraphicLayerImpl::SetMinScale(double scale)
	{
		m_min_scale = scale;
	}

	double GraphicLayerImpl::GetMaxScale()
	{
		return m_max_scale;
	}

	void GraphicLayerImpl::SetMaxScale(double scale)
	{
		m_max_scale = scale;
	}

}