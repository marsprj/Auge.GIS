#include "UserImpl.h"

namespace auge
{
	UserImpl::UserImpl()
	{
	}

	UserImpl::~UserImpl()
	{
	}

	const char*	UserImpl::GetName()
	{
		return NULL;
	}

	const char*	UserImpl::GetAlias()
	{
		return NULL;
	}

	const char*	UserImpl::GetPassword()
	{
		return NULL;
	}

	const char*	UserImpl::GetEmail()
	{
		return NULL;
	}

	Role* UserImpl::GetRole()
	{
		return NULL;
	}

	void UserImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}
