#ifndef __AUGE_FILTER_BINARY_SPATIAL_IMPL_H__
#define __AUGE_FILTER_BINARY_SPATIAL_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class BinarySpatialFilterImpl : public BinarySpatialFilter
	{
	public:
		BinarySpatialFilterImpl();
		virtual ~BinarySpatialFilterImpl();

	public:	
		virtual		augeFilterType		GetType();
		virtual		Expression*			GetPropertyName();
		virtual		bool				GetExtent(GEnvelope& extent);
		virtual		Geometry*			GetGeometry();
		virtual		augeSpatialOperator	GetOperator() ;

		virtual		void				Release();

	public:
		void		SetOperator(augeSpatialOperator oper);
		void		SetPropertyName(PropertyName* pPropName);
		void		SetExtent(GEnvelope& extent);
		void		SetGeometry(Geometry* pGeometry);

	private:
		Expression			*m_pPropertyName;
		Geometry			*m_pGeometry;
		GEnvelope			m_envelope;
		augeSpatialOperator	m_operator;
	};

}

#endif	//__AUGE_FILTER_BINARY_SPATIAL_IMPL_H__
