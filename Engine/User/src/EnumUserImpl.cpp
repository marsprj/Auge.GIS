#include "EnumUserImpl.h"

namespace auge
{
	EnumUserImpl::EnumUserImpl()
	{

	}

	EnumUserImpl::~EnumUserImpl()
	{

	}

	void EnumUserImpl::Reset()
	{

	}

	User* EnumUserImpl::Next()
	{
		return NULL;
	}

	void EnumUserImpl::Release()
	{
		delete this;
	}
}