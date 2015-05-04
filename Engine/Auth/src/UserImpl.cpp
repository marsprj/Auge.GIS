#include "UserImpl.h"

namespace auge
{
	UserImpl::UserImpl()
	{
		m_pRole = NULL;
	}

	UserImpl::~UserImpl()
	{
		if(m_pRole!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRole);
		}
	}

	const char*	UserImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	UserImpl::GetAlias()
	{
		return m_alias.c_str();
	}

	const char*	UserImpl::GetPassword()
	{
		return m_passwd.c_str();
	}

	const char*	UserImpl::GetEmail()
	{
		return m_email.c_str();
	}

	Role* UserImpl::GetRole()
	{
		return m_pRole;
	}

	void UserImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool UserImpl::Create(int gid, const char* name, const char* alias, const char* passwd, const char* email)
	{
		SetID(gid);
		SetName(name);
		SetAlias(alias);
		SetPasswd(passwd);
		SetEmail(email);
		return true;
	}

	void UserImpl::SetName(const char* name)
	{
		if(name)
		{
			m_name = name;
		}
		else
		{
			m_name.clear();
		}
	}

	void UserImpl::SetAlias(const char* alias)
	{
		if(alias)
		{
			m_alias = alias;
		}
		else
		{
			m_alias.clear();
		}
	}

	void UserImpl::SetPasswd(const char* passwd)
	{
		if(passwd)
		{
			m_passwd = passwd;
		}
		else
		{
			m_passwd.clear();
		}
	}

	void UserImpl::SetEmail(const char* email)
	{
		if(email)
		{
			m_email = email;
		}
		else
		{
			m_email.clear();
		}
	}

	void UserImpl::SetRole(Role* pRole)
	{
		if(m_pRole!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRole);
		}
		m_pRole = pRole;
	}

}
