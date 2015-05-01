#ifndef __AUGE_ENUM_USER_IMPL_H__
#define __AUGE_ENUM_USER_IMPL_H__

#include "AugeUser.h"

namespace auge
{
	class EnumUserImpl : public EnumUser
	{
	public:
		EnumUserImpl();
		virtual ~EnumUserImpl();
	public:
		virtual	void			Reset();
		virtual User*			Next();
		virtual void			Release();
	};
}

#endif //__AUGE_ENUM_USER_IMPL_H__
