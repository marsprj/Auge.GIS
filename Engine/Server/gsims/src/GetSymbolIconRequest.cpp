#include "GetSymbolIconRequest.h"

namespace auge
{
	GetSymbolIconRequest::GetSymbolIconRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetSymbolIconRequest::~GetSymbolIconRequest()
	{

	}

	bool GetSymbolIconRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetType(cgi["type"]);
		return true;
	}

	const char*	GetSymbolIconRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetSymbolIconRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetSymbolIconRequest::GetRequest()
	{
		return "GetSymbolIcon";
	}

	const char* GetSymbolIconRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetSymbolIconRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetSymbolIconRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetSymbolIconRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	const char*	GetSymbolIconRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void GetSymbolIconRequest::SetType(const char* type)
	{
		if(type==NULL)
		{
			m_type.clear();
		}
		else
		{
			m_type = type;
		}
	}

	const char* GetSymbolIconRequest::GetType()
	{
		return m_type.empty() ? NULL : m_type.c_str();
	}
}