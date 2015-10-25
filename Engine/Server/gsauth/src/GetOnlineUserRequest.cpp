#include "GetOnlineUserRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetOnlineUserRequest::GetOnlineUserRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		m_offset = 0;
		m_count = 0;
	}

	GetOnlineUserRequest::~GetOnlineUserRequest()
	{
		
	}

	bool GetOnlineUserRequest::Create(rude::CGI& cgi)
	{
		SetCount(cgi["count"]);
		SetOffset(cgi["offset"]);
		return true;
	}

	const char* GetOnlineUserRequest::GetRequest()
	{
		return "GetOnlineUser";
	}
	 
	const char* GetOnlineUserRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetOnlineUserRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetOnlineUserRequest::GetEngine()
	{
		return "was";
	}

	const char* GetOnlineUserRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}
	
	void GetOnlineUserRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	void GetOnlineUserRequest::SetCount(const char* count)
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

	void GetOnlineUserRequest::SetOffset(const char* offset)
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

	g_uint GetOnlineUserRequest::GetCount()
	{
		return m_count;
	}

	g_uint GetOnlineUserRequest::GetOffset()
	{
		return m_offset;
	}
}