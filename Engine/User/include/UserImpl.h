#ifndef __AUGE_USER_IMPL_H__
#define __AUGE_USER_IMPL_H__

#include "AugeUser.h"

namespace auge
{
	class UserImpl : public User
	{
	public:
		UserImpl();
		virtual ~UserImpl();
	public:
		//! @brief	获取字段名
		//! @return	字段名
		virtual const char*		GetName();
		//! @brief	获取字段别名
		//! @return	字段别名
		virtual const char*		GetAlias();
		virtual const char*		GetPassword();
		virtual const char*		GetEmail();
		virtual Role*			GetRole();
		virtual void			Release();
	};
}

#endif //__AUGE_USER_IMPL_H__
