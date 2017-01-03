#ifndef __AUGE_USER_H__
#define __AUGE_USER_H__

#include "AugeCore.h"

#ifdef WIN32 
#	ifdef AUGE_USER_EXPORTS
#		define AUGE_USER_API __declspec(dllexport)
#	else
#		define AUGE_USER_API __declspec(dllimport)
#	endif
#else
#	define AUGE_USER_API
#endif

namespace auge
{
	class GConnection;

	typedef enum
	{
		augeRoleAdmin	= 0,
		augeRoleNormal,
		augeRolePublic
	}augeRoleType;

	/**
	 * User
	 *
	 * 角色类
	 */
	class Role : public GObject
	{
	protected:
		Role(){}
		virtual ~Role(){}
	public:
		virtual	const char*		GetName() = 0;
		virtual const char*		GetAlias() = 0;
		virtual augeRoleType	GetType() = 0;
		virtual bool			IsAdmin() = 0;
		virtual void			Release() = 0;
	};

	/**
	 * User
	 *
	 * 用户类
	 */
	class User : public GObject
	{
	protected:
		User(){}
		virtual ~User(){}
	public:
		//! @brief	获取字段名
		//! @return	字段名
		virtual const char*		GetName() = 0;
		//! @brief	获取字段别名
		//! @return	字段别名
		virtual const char*		GetAlias() = 0;
		virtual const char*		GetPassword() = 0;
		virtual const char*		GetEmail() = 0;
		virtual Role*			GetRole() = 0;

		virtual bool			IsAuthenticated() = 0;
		virtual bool			IsLogined() = 0;
		virtual bool			IsAdmin() = 0;
		virtual bool			IsPublic() = 0;

		virtual void			Release() = 0;
	};

	class EnumUser
	{
	protected:
		EnumUser(){};
		virtual ~EnumUser(){}
	public:
		virtual	void			Reset() = 0;
		virtual User*			Next() = 0;
		virtual void			Release() = 0;
	};

	class EnumRole
	{
	protected:
		EnumRole(){}
		virtual ~EnumRole(){}
	public:
		virtual	void			Reset() = 0;
		virtual Role*			Next() = 0;
		virtual void			Release() = 0;
	};

	class UserManager
	{
	protected:
		UserManager(){}
		virtual ~UserManager(){}
	public:
		virtual RESULTCODE		Initialize(GConnection* pConnection) = 0;

		virtual User*			CreateUser(const char* name, const char* alias, const char* password, const char* email, const char* role) = 0;
		virtual RESULTCODE		RemoveUser(const char* name) = 0;
		virtual User*			GetUser(const char* name) = 0;
		virtual User*			GetUser(g_uint id) = 0;
		virtual EnumUser*		GetUsers(g_uint count=0, g_uint offset=0) = 0;
		virtual g_uint			GetUserCount() = 0;

		virtual EnumUser*		GetLoginedUsers(g_uint count=0, g_uint offset=0) = 0;
		virtual g_uint			GetLoginedUserCount() = 0;

		virtual RESULTCODE		Login() = 0;
		virtual RESULTCODE		Login(const char* name, const char* passwd) = 0;
		virtual RESULTCODE		Login(const char* name, const char* passwd, const char* remote_address) = 0;
		virtual RESULTCODE		Logout(const char* name) = 0;
		virtual bool			IsLogined(const char* name) = 0;

		virtual Role*			GetRole(const char* name) = 0;
		virtual EnumRole*		GetRoles() = 0;
	};

	extern "C"
	{
		AUGE_USER_API UserManager*	augeGetUserManagerInstance();
	}

}

//==============================================================================
//------------------------------------------------------------------------------
// 连接所需动态库
//------------------------------------------------------------------------------
#ifdef WIN32
#	ifndef AUGE_USER_EXPORTS
#		define _AUGE_USER_NAME "GAuth"
#		if defined(_DEBUG)
#			if defined(_UNICODE)
//#				define _AUGE_FILTER_CONFIGURATION "UD"
#				define _AUGE_FILTER_CONFIGURATION "D"
#			else
#				define _AUGE_FILTER_CONFIGURATION "D"
#			endif // _UNICODE
#		else
#			if defined(_UNICODE)
#				define _AUGE_FILTER_CONFIGURATION "U"
#			else
#				define _AUGE_FILTER_CONFIGURATION ""
#			endif // _UNICODE
#		endif // _DEBUG
#		pragma comment(lib, _AUGE_USER_NAME _AUGE_FILTER_CONFIGURATION ".lib")
#		pragma message("Automatically linking with " _AUGE_USER_NAME _AUGE_FILTER_CONFIGURATION ".dll")
#		undef _AUGE_FILTER_CONFIGURATION
#		undef _AUGE_USER_NAME
#	endif // AUGE_FILTER_EXPORTS 
#endif
//==============================================================================


#endif //__AUGE_USER_H__
