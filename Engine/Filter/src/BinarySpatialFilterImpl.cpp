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

Expression* BinarySpatialFilterImpl::GetPropertyName()
{
	return m_pPropertyName;
}

bool BinarySpatialFilterImpl::GetExtent(GEnvelope& extent)
{
	m_envelope = extent;
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

}
