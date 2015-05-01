#include "UserManagerImpl.h"

namespace auge
{
	UserManager* augeGetUserManagerInstance()
	{
		static UserManagerImpl g_userManager;
		return &g_userManager;
	}

	UserManagerImpl::UserManagerImpl()
	{

	}

	UserManagerImpl::~UserManagerImpl()
	{

	}

	User* UserManagerImpl::CreateUser(const char* name, const char* alias, const char* password, const char* email, const char* role)
	{
		return NULL;
	}

	RESULTCODE UserManagerImpl::RemoveUser(const char* name)
	{
		return AG_SUCCESS;
	}

	User* UserManagerImpl::GetUser(const char* name)
	{
		return NULL;
	}

	EnumUser* UserManagerImpl::GetUsers()
	{
		return NULL;
	}

	Role* UserManagerImpl::GetRole(const char* name)
	{
		return NULL;
	}

	EnumRole* UserManagerImpl::GetRoles()
	{
		return NULL;
	}

}