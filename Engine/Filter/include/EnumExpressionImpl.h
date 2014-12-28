#ifndef __AUGE_ENUM_EXPRESSION_IMPL_H__
#define __AUGE_ENUM_EXPRESSION_IMPL_H__

#include "AugeFilter.h"
#include <vector>

namespace auge
{

	class EnumExpressionImpl : public EnumExpression
	{
	public:
		EnumExpressionImpl();
		virtual ~EnumExpressionImpl();
	public:
		virtual		Expression*		Next();
		virtual		void			Reset();
		virtual		void			Release();	

	public:
		bool		AddExpression(Expression* pExpression);

	private:
		void		Cleanup();

	private:
		typedef	std::vector<Expression*>	EXPRESSION_VECTOR;
		EXPRESSION_VECTOR			m_expressions;
		EXPRESSION_VECTOR::iterator	m_iter;
	};

}

#endif	//__AUGE_ENUM_EXPRESSION_IMPL_H__
