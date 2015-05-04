#include "RoleImpl.h"

namespace auge
{
	RoleImpl::RoleImpl()
	{
	}

	RoleImpl::~RoleImpl()
	{
	}

	const char*	RoleImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	RoleImpl::GetAlias()
	{
		return m_alias.c_str();
	}

	augeRoleType RoleImpl::GetType()
	{
		return augeRoleNormal;
	}

	bool RoleImpl::Create(int gid, const char* name, const char* alias)
	{
		SetID(gid);
		SetName(name);
		SetAlias(alias);
		return true;
	}

	void RoleImpl::SetName(const char* name)
	{
		if(name)
		{
			m_name = name;
		}
		else
		{
			m_name.clear();
		}
	}

	void RoleImpl::SetAlias(const char* alias)
	{
		if(alias)
		{
			m_alias = alias;
		}
		else
		{
			m_alias.clear();
		}
	}

	void RoleImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}
