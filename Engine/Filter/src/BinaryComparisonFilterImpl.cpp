#include "BinaryComparisonFilterImpl.h"

namespace auge
{
	BinaryComparisonFilterImpl::BinaryComparisonFilterImpl():
	m_pExpression1(NULL),
	m_pExpression2(NULL),
	m_isMatchCase(false),
	m_operator(augeComOprEqual)
	{

	}

	BinaryComparisonFilterImpl::~BinaryComparisonFilterImpl()
	{
		if(m_pExpression1!=NULL)
		{
			m_pExpression1->Release();
			m_pExpression1 = NULL;
		}
		if(m_pExpression2!=NULL)
		{
			m_pExpression2->Release();
			m_pExpression2 = NULL;
		}
	}

	augeFilterType BinaryComparisonFilterImpl::GetType()
	{
		return augeFilterComparison;
	}

	augeComparisonOperator BinaryComparisonFilterImpl::GetOperator()
	{
		return m_operator;
	}

	void BinaryComparisonFilterImpl::SetOperator(augeComparisonOperator oper)
	{
		m_operator = oper;
	}

	Expression* BinaryComparisonFilterImpl::GetExpression1()
	{
		return m_pExpression1;
	}

	Expression* BinaryComparisonFilterImpl::GetExpression2()
	{
		return m_pExpression2;
	}

	bool BinaryComparisonFilterImpl::IsMatchCase()
	{
		return m_isMatchCase;
	}

	void BinaryComparisonFilterImpl::SetExpression1(Expression* pExpression)
	{
		if(pExpression!=m_pExpression1)
		{
			if(m_pExpression1!=NULL)
			{
				m_pExpression1->Release();
			}
			m_pExpression1 = pExpression;
			if(m_pExpression1!=NULL)
			{
				m_pExpression1->AddRef();
			}
		}
	}

	void BinaryComparisonFilterImpl::SetExpression2(Expression* pExpression)
	{
		if(pExpression!=m_pExpression2)
		{
			if(m_pExpression2!=NULL)
			{
				m_pExpression2->Release();
			}
			m_pExpression2 = pExpression;
			if(m_pExpression2!=NULL)
			{
				m_pExpression2->AddRef();
			}
		}
	}

	void BinaryComparisonFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}
