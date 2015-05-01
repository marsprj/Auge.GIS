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
		return NULL;
	}

	const char*	RoleImpl::GetAlias()
	{
		return NULL;
	}

	augeRoleType RoleImpl::GetType()
	{
		return augeRoleNormal;
	}
}
