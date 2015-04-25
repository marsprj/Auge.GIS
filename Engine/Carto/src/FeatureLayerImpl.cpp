#include "FeatureLayerImpl.h"

namespace auge
{
	FeatureLayerImpl::FeatureLayerImpl()
	{
		m_queryable = true;
		m_visiable = true;
		m_pStyle = NULL;
		m_pFeatureClass = NULL;
		m_srid = 4326; 
	}

	FeatureLayerImpl::~FeatureLayerImpl()
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

	augeLayerType FeatureLayerImpl::GetType()
	{
		return augeLayerFeature;
	}

	g_int FeatureLayerImpl::GetSRID()
	{
		return m_srid;
	}

	void FeatureLayerImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}

	const char* FeatureLayerImpl::GetName()
	{
		return m_name.c_str();
	}

	void FeatureLayerImpl::SetName(const char* szName)
	{
		if(szName==NULL)
		{
			m_name.clear();
		}
		m_name = szName;
	}

	RESULTCODE FeatureLayerImpl::SetFeatureClass(FeatureClass* pFeatureClass)
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

	FeatureClass* FeatureLayerImpl::GetFeatureClass()
	{
		return m_pFeatureClass;
	}
	
	GEnvelope& FeatureLayerImpl::GetExtent()
	{
		if(m_pFeatureClass!=NULL)
		{
			m_extent = m_pFeatureClass->GetExtent();
		}
		return m_extent;
	}

	bool FeatureLayerImpl::IsQueryable()
	{
		return m_queryable;
	}

	void FeatureLayerImpl::SetQueryable(bool flag)
	{
		m_queryable = flag;
	}

	bool FeatureLayerImpl::IsVisiable()
	{
		return m_visiable;
	}

	void FeatureLayerImpl::SetVisiable(bool flag)
	{
		m_visiable = flag;
	}

	RESULTCODE FeatureLayerImpl::SetStyle(Style* pStyle)
	{
		if(m_pStyle!=NULL)
		{
			m_pStyle->Release();
		}
		m_pStyle = pStyle;
		return AG_SUCCESS;
	}

	Style* FeatureLayerImpl::GetStyle()
	{
		return m_pStyle;
	}

}