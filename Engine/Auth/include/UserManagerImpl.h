#ifndef __AUGE_USER_MANAGER_IMPL_H__
#define __AUGE_USER_MANAGER_IMPL_H__

#include "AugeUser.h"
#include "UserImpl.h"

namespace auge
{
	class UserManagerImpl : public UserManager
	{
	public:
		UserManagerImpl();
		virtual ~UserManagerImpl();
	public:
		virtual RESULTCODE		Initialize(GConnection* pConnection);

		virtual User*			CreateUser(const char* name, const char* alias, const char* password, const char* email, const char* role);
		virtual RESULTCODE		RemoveUser(const char* name);
		virtual User*			GetUser(const char* name);
		virtual EnumUser*		GetUsers();

		virtual Role*			GetRole(const char* name);
		virtual EnumRole*		GetRoles();

	public:
		RESULTCODE CreateRoleTable();
		RESULTCODE InitRoleTable();
		RESULTCODE CreateUserTable();
		RESULTCODE InitUserTable();

	private:
		GConnection	*m_pConnection;
		UserImpl	*m_pUser;	
	};
}

#endif //__AUGE_USER_MANAGER_IMPL_H__
