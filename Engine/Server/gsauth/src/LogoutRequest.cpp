#include "LogoutRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	LogoutRequest::LogoutRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	LogoutRequest::~LogoutRequest()
	{
		
	}

	bool LogoutRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		return true;
	}

	const char* LogoutRequest::GetRequest()
	{
		return "Logout";
	}
	 
	const char* LogoutRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* LogoutRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	LogoutRequest::GetEngine()
	{
		return "wus";
	}

	const char* LogoutRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	LogoutRequest::GetName()
	{
		return m_name.c_str();
	}

	void LogoutRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void LogoutRequest::SetName(const char* name)
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
}