#include "IsLoginedRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	IsLoginRequest::IsLoginRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	IsLoginRequest::~IsLoginRequest()
	{
		
	}

	bool IsLoginRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		return true;
	}

	const char* IsLoginRequest::GetRequest()
	{
		return "IsLogin";
	}
	 
	const char* IsLoginRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* IsLoginRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	IsLoginRequest::GetEngine()
	{
		return "wus";
	}

	const char* IsLoginRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	IsLoginRequest::GetName()
	{
		return m_name.c_str();
	}

	void IsLoginRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void IsLoginRequest::SetName(const char* name)
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