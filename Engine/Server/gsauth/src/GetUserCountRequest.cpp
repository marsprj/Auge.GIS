#include "GetUserCountRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetUserCountRequest::GetUserCountRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	GetUserCountRequest::~GetUserCountRequest()
	{
		
	}

	bool GetUserCountRequest::Create(rude::CGI& cgi)
	{
		return true;
	}

	const char* GetUserCountRequest::GetRequest()
	{
		return "GetUserCount";
	}
	 
	const char* GetUserCountRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetUserCountRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetUserCountRequest::GetEngine()
	{
		return "was";
	}

	const char* GetUserCountRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}
	
	void GetUserCountRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}