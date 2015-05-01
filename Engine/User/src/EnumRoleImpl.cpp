#include "EnumRoleImpl.h"

namespace auge
{
	EnumRoleImpl::EnumRoleImpl()
	{

	}

	EnumRoleImpl::~EnumRoleImpl()
	{

	}

	void EnumRoleImpl::Reset()
	{

	}

	Role* EnumRoleImpl::Next()
	{
		return NULL;
	}

	void EnumRoleImpl::Release()
	{
		delete this;
	}
}