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
		//! @brief	��ȡ�ֶ���
		//! @return	�ֶ���
		virtual const char*		GetName();
		//! @brief	��ȡ�ֶα���
		//! @return	�ֶα���
		virtual const char*		GetAlias();
		virtual const char*		GetPassword();
		virtual const char*		GetEmail();
		virtual Role*			GetRole();
		virtual void			Release();
	};
}

#endif //__AUGE_USER_IMPL_H__
