#include "ArithmeticExpressionImpl.h"

namespace auge
{
	ArithmeticExpressionImpl::ArithmeticExpressionImpl():
	m_operator(augeAriOprAdd),
		m_pLeft(NULL),
		m_pRight(NULL)
	{

	}

	ArithmeticExpressionImpl::~ArithmeticExpressionImpl()
	{
		if(m_pLeft!=NULL)
		{
			m_pLeft->Release();
			m_pLeft = NULL;
		}
		if(m_pRight != NULL)
		{
			m_pRight->Release();
			m_pRight = NULL;
		}	
	}

	augeExpressionType ArithmeticExpressionImpl::GetType()
	{
		return augeExpArithmetic;
	}

	augeArithmeticOperator ArithmeticExpressionImpl::GetOperator()
	{
		return m_operator;
	}

	Expression* ArithmeticExpressionImpl::GetLeft()
	{
		return m_pLeft;
	}

	Expression* ArithmeticExpressionImpl::GetRight()
	{
		return m_pRight;
	}

	void ArithmeticExpressionImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}
