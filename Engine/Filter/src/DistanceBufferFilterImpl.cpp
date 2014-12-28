#include "DistanceBufferFilterImpl.h"

namespace auge
{
	DistanceBufferFilterImpl::DistanceBufferFilterImpl():
	m_pPropertyName(NULL),
	m_pGeometry(NULL),
	m_operator(augeSpIntersects),
	m_distance(1.0f),
	m_unit(augeMeters)
	{

	}

	DistanceBufferFilterImpl::~DistanceBufferFilterImpl()
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

	Expression* DistanceBufferFilterImpl::GetPropertyName()
	{
		return m_pPropertyName;
	}

	bool DistanceBufferFilterImpl::GetExtent(GEnvelope& extent)
	{
		m_extent = extent;
		return true;
	}

	Geometry* DistanceBufferFilterImpl::GetGeometry()
	{
		return m_pGeometry;
	}

	augeSpatialOperator	DistanceBufferFilterImpl::GetOperator()
	{
		return m_operator;
	}

	void DistanceBufferFilterImpl::Release()
	{
		delete this;
	}	

	double DistanceBufferFilterImpl::GetDistance()
	{
		return m_distance;
	}

	augeUnitType DistanceBufferFilterImpl::GetUnit()
	{
		return m_unit;
	}

}