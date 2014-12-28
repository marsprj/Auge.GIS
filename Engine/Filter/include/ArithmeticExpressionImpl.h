#ifndef __AUGE_EXPRESSION_ARITHMETIC_IMPL_H__
#define __AUGE_EXPRESSION_ARITHMETIC_IMPL_H__

#include "AugeFilter.h"
#include <string>

namespace auge
{
	class ArithmeticExpressionImpl : public ArithmeticExpression
	{
	public:
		ArithmeticExpressionImpl();
		virtual ~ArithmeticExpressionImpl();
	public:
		virtual	augeExpressionType			GetType();

		virtual		augeArithmeticOperator	GetOperator();
		virtual		Expression*				GetLeft();
		virtual		Expression*				GetRight();
		virtual		void					Release();

	private:
		augeArithmeticOperator	m_operator;
		Expression*	m_pLeft;
		Expression*	m_pRight;
	};

}

#endif	//__AUGE_EXPRESSION_ARITHMETIC_IMPL_H__
