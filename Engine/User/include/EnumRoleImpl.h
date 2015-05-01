#ifndef __AUGE_ENUM_ROLE_IMPL_H__
#define __AUGE_ENUM_ROLE_IMPL_H__

#include "AugeUser.h"
#include <vector>

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

	public:
		void	Add(Role* pRole);
		void	Cleanup();

	private:
		std::vector<Role*>	m_roles;
		std::vector<Role*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_ROLE_IMPL_H__
