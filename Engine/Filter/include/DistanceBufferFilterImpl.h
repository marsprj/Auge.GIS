#ifndef __AUGE_FILTER_DISTANCE_BINARY_SPATIAL_IMPL_H__
#define __AUGE_FILTER_DISTANCE_BINARY_SPATIAL_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class DistanceBufferFilterImpl : public DistanceBufferFilter
	{
	public:
		DistanceBufferFilterImpl();
		virtual ~DistanceBufferFilterImpl();

	public:	
		virtual		augeFilterType		GetType();
		virtual		Expression*			GetPropertyName();
		virtual		bool				GetExtent(GEnvelope& extent);
		virtual		Geometry*			GetGeometry();
		virtual		augeSpatialOperator	GetOperator() ;

		virtual		double				GetDistance();
		virtual		augeUnitType		GetUnit();

		virtual		void				Release();

	public:
		void		SetOperator(augeSpatialOperator oper);
		void		SetPropertyName(PropertyName* pPropName);
		void		SetExtent(GEnvelope& extent);
		void		SetGeometry(Geometry* pGeometry);
		void		SetDistance(double distance);
		void		SetUnit(augeUnitType unit);

	private:
		Expression			*m_pPropertyName;
		Geometry			*m_pGeometry;
		GEnvelope			m_extent;
		augeSpatialOperator	m_operator;


		double				m_distance;
		augeUnitType		m_unit;
	};

}

#endif	//__AUGE_FILTER_DISTANCE_BINARY_SPATIAL_IMPL_H__
