#include "LoginRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	LoginRequest::LoginRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	LoginRequest::~LoginRequest()
	{
		
	}

	bool LoginRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		SetPassword(cgi["password"]);
		SetRemoteAddress();
		return true;
	}

	const char* LoginRequest::GetRequest()
	{
		return "Login";
	}
	 
	const char* LoginRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* LoginRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	LoginRequest::GetEngine()
	{
		return "was";
	}

	const char* LoginRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	LoginRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void LoginRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void LoginRequest::SetName(const char* name)
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

	void LoginRequest::SetPassword(const char* password)
	{
		if(password)
		{
			m_password = password;
		}
		else
		{
			m_password.clear();
		}
	}

	const char* LoginRequest::GetPassword()
	{
		return m_password.empty() ? NULL : m_password.c_str();
	}

	void LoginRequest::SetRemoteAddress()
	{
		const char* remote_addr = getenv("HTTP_X_FORWARDED_FOR");//getenv("REMOTE_ADDR");
		if(remote_addr==NULL)
		{
			m_remote_address = "127.0.0.1" ;
		}
		else
		{
			if(strlen(remote_addr)==0)
			{
				m_remote_address = "127.0.0.1";
			}
			else
			{
				m_remote_address = remote_addr;
			}
		}
		
	}

	const char* LoginRequest::GetRemoteAddress()
	{
		return m_remote_address.c_str();
	}
}