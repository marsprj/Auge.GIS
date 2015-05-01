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
		//! @brief	��ȡ�ֶ���
		//! @return	�ֶ���
		virtual const char*		GetName();
		//! @brief	��ȡ�ֶα���
		//! @return	�ֶα���
		virtual const char*		GetAlias();
		virtual augeRoleType	GetType();
	};
}

#endif //__AUGE_ROLE_IMPL_H__
