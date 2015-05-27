#include "BinarySpatialFilterImpl.h"

namespace auge
{

	BinarySpatialFilterImpl::BinarySpatialFilterImpl():
m_pPropertyName(NULL),
	m_pGeometry(NULL),
	m_operator(augeSpIntersects)
{

}

BinarySpatialFilterImpl::~BinarySpatialFilterImpl()
{
	if(m_pPropertyName!=NULL)
	{
		m_pPropertyName->Release();
		m_pPropertyName = NULL;
	}
	if(m_pGeometry!=NULL)
	{
		m_pGeometry->Release();
		m_pGeometry = NULL;
	}
}

augeFilterType BinarySpatialFilterImpl::GetType()
{
	return augeFilterSpatial;
}

Expression* BinarySpatialFilterImpl::GetPropertyName()
{
	return m_pPropertyName;
}

bool BinarySpatialFilterImpl::GetExtent(GEnvelope& extent)
{
	m_extent = extent;
	return true;
}

Geometry* BinarySpatialFilterImpl::GetGeometry()
{
	return m_pGeometry;
}

augeSpatialOperator	BinarySpatialFilterImpl::GetOperator()
{
	return m_operator;
}

void BinarySpatialFilterImpl::Release()
{
	if(!ReleaseRef())
	{
		delete this;
	}
}	

void BinarySpatialFilterImpl::SetOperator(augeSpatialOperator oper)
{
	m_operator = oper;
}

void BinarySpatialFilterImpl::SetPropertyName(PropertyName* pPropName)
{
	if(m_pPropertyName!=NULL)
	{
		AUGE_SAFE_RELEASE(m_pPropertyName);
	}
	m_pPropertyName = pPropName;
}

void BinarySpatialFilterImpl::SetExtent(GEnvelope& extent)
{
	m_extent = extent;
}

void BinarySpatialFilterImpl::SetGeometry(Geometry* pGeometry)
{
	if(m_pGeometry==NULL)
	{
		AUGE_SAFE_RELEASE(m_pGeometry);
	}
	m_pGeometry = pGeometry;
}

}
