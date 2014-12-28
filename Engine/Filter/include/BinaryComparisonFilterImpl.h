#ifndef __AUGE_FILTER_BINARY_COMPARISON_IMPL_H__
#define __AUGE_FILTER_BINARY_COMPARISON_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class BinaryComparisonFilterImpl : public BinaryComparisonFilter
	{
	public:
		BinaryComparisonFilterImpl();
		virtual ~BinaryComparisonFilterImpl();

	public:	
		virtual		augeFilterType		GetType();
		virtual		augeComparisonOperator GetOperator();
		virtual		Expression*		GetExpression1();
		virtual		Expression*		GetExpression2();
		virtual		void				SetExpression1(Expression* pExpression);
		virtual		void				SetExpression2(Expression* pExpression);
		virtual		bool				IsMatchCase();
		virtual		void				Release();

	private:
		Expression*	m_pExpression1;
		Expression*	m_pExpression2;
		bool			m_isMatchCase;
		augeComparisonOperator	m_operator;
	};

}

#endif	//__AUGE_FILTER_BINARY_COMPARISON_IMPL_H__
