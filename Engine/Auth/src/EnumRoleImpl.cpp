#include "EnumRoleImpl.h"

namespace auge
{
	EnumRoleImpl::EnumRoleImpl()
	{

	}

	EnumRoleImpl::~EnumRoleImpl()
	{
		Cleanup();
	}

	void EnumRoleImpl::Reset()
	{
		m_iter = m_roles.begin();
	}

	Role* EnumRoleImpl::Next()
	{
		if(m_iter==m_roles.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumRoleImpl::Release()
	{
		delete this;
	}

	void EnumRoleImpl::Add(Role* pRole)
	{
		if(pRole)
		{
			m_roles.push_back(pRole);
		}
	}

	void EnumRoleImpl::Cleanup()
	{
		std::vector<Role*>::iterator iter;
		for(iter=m_roles.begin(); iter!=m_roles.end(); iter++)
		{
			(*iter)->Release();
		}
		m_roles.clear();
	}
}