#include "GetUserRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetUserRequest::GetUserRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		m_offset = 0;
		m_count = 0;
	}

	GetUserRequest::~GetUserRequest()
	{
		
	}

	bool GetUserRequest::Create(rude::CGI& cgi)
	{
		SetName(cgi["name"]);
		SetCount(cgi["count"]);
		SetOffset(cgi["offset"]);
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
		return "was";
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

	void GetUserRequest::SetCount(const char* count)
	{
		if(count==NULL)
		{
			m_count = 0;
		}
		if(strlen(count)==0)
		{
			m_count = 0;
		}
		m_count = atoi(count);
	}

	void GetUserRequest::SetOffset(const char* offset)
	{
		if(offset==NULL)
		{
			m_offset = 0;
		}
		if(strlen(offset)==0)
		{
			m_offset = 0;
		}
		m_offset = atoi(offset);
	}

	g_uint GetUserRequest::GetCount()
	{
		return m_count;
	}

	g_uint GetUserRequest::GetOffset()
	{
		return m_offset;
	}
}