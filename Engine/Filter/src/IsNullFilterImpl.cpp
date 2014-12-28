#include "IsNullFilterImpl.h"

namespace auge
{
	IsNullFilterImpl::IsNullFilterImpl():
	m_pPropertyName(NULL)
	{

	}

	IsNullFilterImpl::~IsNullFilterImpl()
	{
		if(m_pPropertyName!=NULL)
		{
			m_pPropertyName->Release();
			m_pPropertyName = NULL;
		}
	}

	Expression* IsNullFilterImpl::GetPropertyName()
	{
		return m_pPropertyName;
	}

	void IsNullFilterImpl::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}

}
