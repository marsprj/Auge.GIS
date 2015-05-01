#ifndef __AUGE_ROLE_IMPL_H__
#define __AUGE_ROLE_IMPL_H__

#include "AugeUser.h"

namespace auge
{
	class RoleImpl : public Role
	{
	public:
		RoleImpl();
		virtual ~RoleImpl();
	public:
		//! @brief	获取字段名
		//! @return	字段名
		virtual const char*		GetName();
		//! @brief	获取字段别名
		//! @return	字段别名
		virtual const char*		GetAlias();
		virtual augeRoleType	GetType();
	};
}

#endif //__AUGE_ROLE_IMPL_H__
