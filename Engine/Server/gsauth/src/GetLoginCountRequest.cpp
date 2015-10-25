#include "GetLoginCountRequest.h"
#include "AugeXML.h"
#include "AugeField.h"

namespace auge
{
	GetLoginCountRequest::GetLoginCountRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		
	}

	GetLoginCountRequest::~GetLoginCountRequest()
	{
		
	}

	bool GetLoginCountRequest::Create(rude::CGI& cgi)
	{
		return true;
	}

	const char* GetLoginCountRequest::GetRequest()
	{
		return "GetLoginCount";
	}
	 
	const char* GetLoginCountRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetLoginCountRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	GetLoginCountRequest::GetEngine()
	{
		return "was";
	}

	const char* GetLoginCountRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}
	
	void GetLoginCountRequest::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}