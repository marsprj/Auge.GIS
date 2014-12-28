#include "PropertyNameImpl.h"

namespace auge
{

	PropertyNameImpl::PropertyNameImpl()
	{

	}

	PropertyNameImpl::~PropertyNameImpl()
	{

	}

	augeExpressionType PropertyNameImpl::GetType()
	{
		return augeExpPropertyName;
	}

	void PropertyNameImpl::SetName(const char* szName)
	{
		if(szName!=NULL)
		{
			m_strName = szName;
		}
	}

	const char*	PropertyNameImpl::GetName()
	{
		return m_strName.c_str();
	}

	void PropertyNameImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}