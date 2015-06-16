#include "IsBetweenFilterImpl.h"

namespace auge
{
	IsBetweenFilterImpl::IsBetweenFilterImpl():
	m_pExpression(NULL),
	m_pLowerBound(NULL),
	m_pUpperBound(NULL)
	{
	}

	IsBetweenFilterImpl::~IsBetweenFilterImpl()
	{
		if(m_pExpression!=NULL)
		{
			m_pExpression->Release();
			m_pExpression = NULL;
		}
		if(m_pLowerBound!=NULL)
		{
			m_pLowerBound->Release();
			m_pLowerBound = NULL;
		}
		if(m_pUpperBound!=NULL)
		{
			m_pUpperBound->Release();
			m_pUpperBound = NULL;
		}
	}

	augeFilterType IsBetweenFilterImpl::GetType()
	{
		return augeFilterComparison;
	}

	augeComparisonOperator IsBetweenFilterImpl::GetOperator()
	{
		return augeComOprIsBetween;
	}

	void IsBetweenFilterImpl::SetOperator(augeComparisonOperator oper)
	{
		
	}

	Expression* IsBetweenFilterImpl::GetExpression()
	{
		return m_pExpression;
	}

	bool IsBetweenFilterImpl::SetExpression(Expression* pExpression)
	{
		if(pExpression==NULL)
		{
			return false;
		}
		if(m_pExpression!=NULL)
		{
			m_pExpression->Release();
			m_pExpression = NULL;
		}
		m_pExpression = pExpression;
		m_pExpression->AddRef();
		return true;
	}

	Expression* IsBetweenFilterImpl::GetLowerBound()
	{
		return m_pLowerBound;
	}

	bool IsBetweenFilterImpl::SetLowerBound(Expression* pLowerBound)
	{
		if(pLowerBound==NULL)
		{
			return false;
		}
		if(m_pLowerBound!=NULL)
		{
			m_pLowerBound->Release();
			m_pLowerBound = NULL;
		}
		m_pLowerBound = pLowerBound;
		m_pLowerBound->AddRef();
		return true;
	}

	Expression* IsBetweenFilterImpl::GetUpperBound()
	{
		return m_pUpperBound;
	}

	bool IsBetweenFilterImpl::SetUpperBound(Expression* pUpperBound)
	{
		if(pUpperBound==NULL)
		{
			return false;
		}
		if(m_pUpperBound!=NULL)
		{
			m_pUpperBound->Release();
			m_pUpperBound = NULL;
		}
		m_pUpperBound = pUpperBound;
		m_pUpperBound->AddRef();
		return true;
	}

	void IsBetweenFilterImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

}
