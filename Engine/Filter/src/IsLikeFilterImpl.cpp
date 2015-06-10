#include "IsLikeFilterImpl.h"

namespace auge
{
	IsLikeFilterImpl::IsLikeFilterImpl():
	m_pPropertyName(NULL),
		m_pLiteral(NULL),
		m_strWildCard("*"),
		m_strSingleChar("#"),
		m_strEscapeChar("!")
	{

	}

	IsLikeFilterImpl::~IsLikeFilterImpl()
	{
		if(m_pPropertyName!=NULL)
		{
			m_pPropertyName->Release();
			m_pPropertyName = NULL;
		}
		if(m_pLiteral!=NULL)
		{
			m_pLiteral->Release();
			m_pLiteral = NULL;
		}
	}

	augeFilterType IsLikeFilterImpl::GetType()
	{
		return augeFilterComparison;
	}

	Expression* IsLikeFilterImpl::GetPropertyName()
	{
		return m_pPropertyName;
	}

	augeComparisonOperator IsLikeFilterImpl::GetOperator()
	{
		return augeComOprIsLike;
	}

	void IsLikeFilterImpl::SetOperator(augeComparisonOperator oper)
	{

	}

	bool IsLikeFilterImpl::SetPropertyName(Expression* pPropName)
	{
		if(pPropName==NULL)
		{
			return false;
		}
		if(m_pPropertyName!=NULL)
		{
			m_pPropertyName->Release();
			m_pPropertyName = NULL;
		}
		m_pPropertyName = pPropName;
		m_pPropertyName->AddRef();
		return true;
	}

	Literal* IsLikeFilterImpl::GetLiteral()
	{
		return m_pLiteral;
	}

	bool IsLikeFilterImpl::SetLiteral(Literal* pLiteral)
	{
		if(pLiteral==NULL)
		{
			return false;
		}
		if(m_pLiteral!=NULL)
		{
			m_pLiteral->Release();
			m_pLiteral = NULL;
		}
		m_pLiteral = pLiteral;
		m_pLiteral->AddRef();
		return true;
	}


	const char* IsLikeFilterImpl::GetWildCard()
	{
		return m_strWildCard.c_str();
	}

	const char*	IsLikeFilterImpl::GetSingleChar()
	{
		return m_strSingleChar.c_str();
	}

	const char*	IsLikeFilterImpl::GetEscapeChar()
	{
		return m_strEscapeChar.c_str();
	}

	void IsLikeFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}

