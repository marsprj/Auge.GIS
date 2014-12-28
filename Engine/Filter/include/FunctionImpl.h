#ifndef __AUGE_EXPRESSION_FUNCTION_IMPL_H__
#define __AUGE_EXPRESSION_FUNCTION_IMPL_H__

#include "AugeFilter.h"
#include "EnumExpressionImpl.h"
#include <string>

namespace auge
{
	class FunctionImpl : public Function
	{
	public:
		FunctionImpl();
		virtual ~FunctionImpl();
	public:
		virtual	augeExpressionType		GetType();

		virtual		bool				AddExpression(Expression* pExpression);
		virtual		EnumExpression*	GetExpressions();
		virtual		const char*			GetName();
		virtual		void				Release();
	private:
		EnumExpressionImpl			m_expressions;
		std::string						m_strName;
	};

}

#endif	//__AUGE_EXPRESSION_FUNCTION_IMPL_H__
