#ifndef __AUGE_FILTER_ID_IMPL_H__
#define __AUGE_FILTER_ID_IMPL_H__

#include "AugeFilter.h"
#include <vector>

namespace auge
{
	class IDFilterImpl : public IDFilter
	{
	public:
		IDFilterImpl();
		virtual ~IDFilterImpl();
	public:
		virtual	augeFilterType		GetType();
		virtual	void				Add(g_uint id);
		virtual	g_uint				GetID(g_uint i);
		virtual	g_uint				Count();
		virtual void				Clear();

		virtual	augeComparisonOperator	GetOperator();
		virtual	void				SetOperator(augeComparisonOperator oper);

		virtual	void				Release();

	private:
		std::vector<g_uint>			m_ids;
		augeComparisonOperator	m_operator;
	};

}

#endif //__AUGE_FILTER_ID_IMPL_H__