#include "CreateUserRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	CreateUserRequest::CreateUserRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	CreateUserRequest::~CreateUserRequest()
	{
		
	}

	bool CreateUserRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		SetAlias(cgi["alias"]);
		SetPassword(cgi["password"]);
		SetEmail(cgi["email"]);
		SetRole(cgi["role"]);
		return true;
	}

	const char* CreateUserRequest::GetRequest()
	{
		return "CreateUser";
	}

	const char* CreateUserRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* CreateUserRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	CreateUserRequest::GetEngine()
	{
		return "was";
	}

	const char*	CreateUserRequest::GetName()
	{
		return m_name.c_str();
	}

	const char*	CreateUserRequest::GetAlias()
	{
		return m_alias.c_str();
	}

	const char*	CreateUserRequest::GetPassword()
	{
		return m_passwd.c_str();
	}

	const char*	CreateUserRequest::GetEmail()
	{
		return m_email.c_str();
	}

	const char* CreateUserRequest::GetRole()
	{
		return m_role.c_str();
	}

	const char* CreateUserRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void CreateUserRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void CreateUserRequest::SetName(const char* name)
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

	void CreateUserRequest::SetAlias(const char* alias)
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

	void CreateUserRequest::SetPassword(const char* passwd)
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

	void CreateUserRequest::SetEmail(const char* email)
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

	void CreateUserRequest::SetRole(const char* role)
	{
		if(role)
		{
			m_role = role;
		}
		else
		{
			m_role.clear();
		}
	}

}