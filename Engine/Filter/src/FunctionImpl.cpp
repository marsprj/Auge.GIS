#include "FunctionImpl.h"

namespace auge
{
	FunctionImpl::FunctionImpl()
	{

	}

	FunctionImpl::~FunctionImpl()
	{

	}

	augeExpressionType FunctionImpl::GetType()
	{
		return augeExpFunction;
	}

	bool FunctionImpl::AddExpression(Expression* pExpression)
	{
		return m_expressions.AddExpression(pExpression);
	}

	EnumExpression* FunctionImpl::GetExpressions()
	{
		return &m_expressions;
	}

	const char*	FunctionImpl::GetName()
	{
		return m_strName.c_str();
	}

	void FunctionImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}
