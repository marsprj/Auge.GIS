#include "GetUserRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetUserRequest::GetUserRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = "GBK";
		
	}

	GetUserRequest::~GetUserRequest()
	{
		
	}

	bool GetUserRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		return true;
	}

	const char* GetUserRequest::GetRequest()
	{
		return "GetUser";
	}
	 
	const char* GetUserRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetUserRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetUserRequest::GetEngine()
	{
		return "wus";
	}

	const char* GetUserRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	GetUserRequest::GetName()
	{
		return m_name.c_str();
	}

	void GetUserRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void GetUserRequest::SetName(const char* name)
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