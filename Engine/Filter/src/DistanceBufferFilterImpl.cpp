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

	augeFilterType DistanceBufferFilterImpl::GetType()
	{
		return augeFilterSpatial;
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

	void DistanceBufferFilterImpl::SetDistance(double distance)
	{
		m_distance = distance;
	}

	void DistanceBufferFilterImpl::SetUnit(augeUnitType unit)
	{
		m_unit = unit;
	}



	void DistanceBufferFilterImpl::SetOperator(augeSpatialOperator oper)
	{
		m_operator = oper;
	}

	void DistanceBufferFilterImpl::SetPropertyName(PropertyName* pPropName)
	{
		if(m_pPropertyName!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pPropertyName);
		}
		m_pPropertyName = pPropName;
	}

	void DistanceBufferFilterImpl::SetExtent(GEnvelope& extent)
	{
		m_extent = extent;
	}

	void DistanceBufferFilterImpl::SetGeometry(Geometry* pGeometry)
	{
		if(m_pGeometry==NULL)
		{
			AUGE_SAFE_RELEASE(m_pGeometry);
		}
		m_pGeometry = pGeometry;
	}


}