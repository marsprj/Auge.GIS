#ifndef __AUGE_ROLE_IMPL_H__
#define __AUGE_ROLE_IMPL_H__

#include "AugeUser.h"
#include <string>

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
		virtual void			Release();

	public:
		bool	Create(int gid, const char* name, const char* alias);
		void	SetName(const char* name);
		void	SetAlias(const char* alias);

	private:
		std::string m_name;
		std::string m_alias;
	};
}

#endif //__AUGE_ROLE_IMPL_H__
