#ifndef __AUGE_ENUM_ROLE_IMPL_H__
#define __AUGE_ENUM_ROLE_IMPL_H__

#include "AugeUser.h"

namespace auge
{
	class EnumRoleImpl : public EnumRole
	{
	public:
		EnumRoleImpl();
		virtual ~EnumRoleImpl();
	public:
		virtual	void			Reset();
		virtual Role*			Next();
		virtual void			Release();
	};
}

#endif //__AUGE_ENUM_ROLE_IMPL_H__
