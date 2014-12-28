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


	Expression* IsLikeFilterImpl::GetPropertyName()
	{
		return m_pPropertyName;
	}

	Literal* IsLikeFilterImpl::GetLiteral()
	{
		return m_pLiteral;
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

