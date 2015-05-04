#ifndef __AUGE_ENUM_USER_IMPL_H__
#define __AUGE_ENUM_USER_IMPL_H__

#include "AugeUser.h"
#include <vector>

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
	public:
		void	Add(User* pUser);
		void	Cleanup();

	private:
		std::vector<User*>	m_Users;
		std::vector<User*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_USER_IMPL_H__
