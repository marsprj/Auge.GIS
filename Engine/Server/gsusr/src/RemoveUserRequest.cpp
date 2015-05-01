#include "RemoveUserRequest.h"f
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	RemoveUserRequest::RemoveUserRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = "GBK";
		
	}

	RemoveUserRequest::~RemoveUserRequest()
	{
		
	}

	bool RemoveUserRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		return true;
	}

	const char* RemoveUserRequest::GetRequest()
	{
		return "RemoveUser";
	}

	const char* RemoveUserRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* RemoveUserRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	RemoveUserRequest::GetEngine()
	{
		return "wus";
	}

	const char*	RemoveUserRequest::GetName()
	{
		return m_name.c_str();
	}

	void RemoveUserRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void RemoveUserRequest::SetName(const char* name)
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