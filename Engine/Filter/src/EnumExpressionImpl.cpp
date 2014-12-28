#include "EnumExpressionImpl.h"

namespace auge
{
	EnumExpressionImpl::EnumExpressionImpl()
	{
		m_iter = m_expressions.begin();
	}

	EnumExpressionImpl::~EnumExpressionImpl()
	{
		Cleanup();
	}

	Expression* EnumExpressionImpl::Next()
	{
		if(m_iter==m_expressions.end())
		{
			return NULL;
		}
		Expression* pExpression = *m_iter;
		return pExpression;
	}

	void EnumExpressionImpl::Reset()
	{
		m_iter = m_expressions.begin();
	}

	void EnumExpressionImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

	void EnumExpressionImpl::Cleanup()
	{
		Expression *pExpression = NULL;
		EXPRESSION_VECTOR::iterator iter;
		for(iter=m_expressions.begin(); iter!=m_expressions.end(); iter++)
		{
			pExpression = *iter;
			if(pExpression!=NULL)
			{
				pExpression->Release();
			}
		}
		m_expressions.clear();
	}

	bool EnumExpressionImpl::AddExpression(Expression* pExpression)
	{
		if(pExpression==NULL)
		{
			return false;
		}
		m_expressions.push_back(pExpression);
		return true;
	}

}
