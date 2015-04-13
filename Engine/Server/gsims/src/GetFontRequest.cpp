#include "GetFontRequest.h"

namespace auge
{
	GetFontRequest::GetFontRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	GetFontRequest::~GetFontRequest()
	{

	}

	bool GetFontRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	GetFontRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetFontRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetFontRequest::GetRequest()
	{
		return "GetFont";
	}

	void GetFontRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetFontRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetFontRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}

	}

	const char*	GetFontRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}