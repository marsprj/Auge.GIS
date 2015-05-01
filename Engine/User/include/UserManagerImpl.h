#ifndef __AUGE_USER_MANAGER_IMPL_H__
#define __AUGE_USER_MANAGER_IMPL_H__

#include "AugeUser.h"

namespace auge
{
	class UserManagerImpl : public UserManager
	{
	public:
		UserManagerImpl();
		virtual ~UserManagerImpl();
	public:
		virtual User*			CreateUser(const char* name, const char* alias, const char* password, const char* email, const char* role);
		virtual RESULTCODE		RemoveUser(const char* name);
		virtual User*			GetUser(const char* name);
		virtual EnumUser*		GetUsers();

		virtual Role*			GetRole(const char* name);
		virtual EnumRole*		GetRoles();
	};
}

#endif //__AUGE_USER_MANAGER_IMPL_H__
