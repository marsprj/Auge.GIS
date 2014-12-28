#ifndef __AUGE_FILTER_BBOX_IMPL_H__
#define __AUGE_FILTER_BBOX_IMPL_H__

#include "AugeFilter.h"
#include "AugeGeometry.h"

namespace auge
{
	class BBoxFilterImpl : public BBoxFilter
	{
	public:
		BBoxFilterImpl();
		virtual ~BBoxFilterImpl();

	public:
		virtual		augeFilterType		GetType();
		virtual		augeSpatialOperator	GetOperator();

		virtual		Expression*			GetPropertyName();
		virtual		bool				SetPropertyName(Expression* pPropertyName);

		virtual		bool				GetExtent(GEnvelope& extent);
		virtual		void				SetExtent(GEnvelope& extent);
		virtual		void				SetExtent(double xmin, double ymin, double xmax, double ymax);

		virtual		void				Release();

	private:
		Expression	*m_pPropertyName;
		GEnvelope		m_extent;
	};
}

#endif	//__AUGE_FILTER_BBOX_IMPL_H__
