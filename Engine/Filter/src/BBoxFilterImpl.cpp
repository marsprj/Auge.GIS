#include "BBoxFilterImpl.h"

namespace auge
{
	BBoxFilterImpl::BBoxFilterImpl():
	m_pPropertyName(NULL)
	{

	}

	BBoxFilterImpl::~BBoxFilterImpl()
	{
		if(m_pPropertyName!=NULL)
		{
			m_pPropertyName->Release();
			m_pPropertyName = NULL;
		}
	}

	augeFilterType BBoxFilterImpl::GetType()
	{
		return augeFilterSpatial;
	}

	augeSpatialOperator	BBoxFilterImpl::GetOperator()
	{
		return augeSpBBox;
	}

	bool BBoxFilterImpl::SetPropertyName(Expression* pPropertyName)
	{
		if(m_pPropertyName!=NULL)
		{
			m_pPropertyName->Release();
			m_pPropertyName = NULL;
		}
		m_pPropertyName = pPropertyName;
		//if(m_pPropertyName!=NULL)
		//{
		//	m_pPropertyName->AddRef();
		//}
		return true;
	}

	Expression* BBoxFilterImpl::GetPropertyName()
	{
		return m_pPropertyName;
	}

	bool BBoxFilterImpl::GetExtent(GEnvelope& extent)
	{
		extent = m_extent;
		return true;
	}

	void BBoxFilterImpl::SetExtent(GEnvelope& extent)
	{
		m_extent = extent;
	}

	void BBoxFilterImpl::SetExtent(double xmin, double ymin, double xmax, double ymax)
	{
		m_extent.Set(xmin, ymin, xmax, ymax);
	}

	void BBoxFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}	
	}

}
