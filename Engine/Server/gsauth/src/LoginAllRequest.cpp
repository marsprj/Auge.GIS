#include "LoginAllRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	LoginAllRequest::LoginAllRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	LoginAllRequest::~LoginAllRequest()
	{
		
	}

	bool LoginAllRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		SetPassword(cgi["password"]);
		SetRemoteAddress();
		return true;
	}

	const char* LoginAllRequest::GetRequest()
	{
		return "LoginAll";
	}
	 
	const char* LoginAllRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* LoginAllRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	LoginAllRequest::GetEngine()
	{
		return "was";
	}

	const char* LoginAllRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	LoginAllRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void LoginAllRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void LoginAllRequest::SetName(const char* name)
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

	void LoginAllRequest::SetPassword(const char* password)
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

	const char* LoginAllRequest::GetPassword()
	{
		return m_password.empty() ? NULL : m_password.c_str();
	}

	void LoginAllRequest::SetRemoteAddress()
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

	const char* LoginAllRequest::GetRemoteAddress()
	{
		return m_remote_address.c_str();
	}
}