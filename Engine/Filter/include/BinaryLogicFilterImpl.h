#ifndef __AUGE_FILTER_BINARY_LOGIC_IMPL_H__
#define __AUGE_FILTER_BINARY_LOGIC_IMPL_H__

#include "AugeFilter.h"
#include "EnumFilterImpl.h"
#include <string>

namespace auge
{
	class BinaryLogicFilterImpl : public BinaryLogicFilter
	{
	public:
		BinaryLogicFilterImpl();
		virtual ~BinaryLogicFilterImpl();

	public:	
		virtual		augeFilterType		GetType();
		virtual		void				SetOperator(augeLogicalOperator	oper);
		virtual		augeLogicalOperator	GetOperator();
		virtual		bool				AddFilter(GFilter* pFilter);
		virtual		EnumFilter*			GetFilters();
		virtual		void				Release();

	private:
		EnumFilterImpl	m_filters;
		augeLogicalOperator	m_operator;
	};

}

#endif	//__AUGE_FILTER_BINARY_LOGIC_IMPL_H__
