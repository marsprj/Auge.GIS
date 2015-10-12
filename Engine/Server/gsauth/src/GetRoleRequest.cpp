#include "GetRoleRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetRoleRequest::GetRoleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	GetRoleRequest::~GetRoleRequest()
	{
		
	}

	bool GetRoleRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		return true;
	}

	const char* GetRoleRequest::GetRequest()
	{
		return "GetRole";
	}
	 
	const char* GetRoleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetRoleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetRoleRequest::GetEngine()
	{
		return "was";
	}

	const char* GetRoleRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	GetRoleRequest::GetName()
	{
		return m_name.c_str();
	}

	void GetRoleRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void GetRoleRequest::SetName(const char* name)
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