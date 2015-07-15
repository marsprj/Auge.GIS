#include "GetSymbolRequest.h"

namespace auge
{
	GetSymbolRequest::GetSymbolRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetSymbolRequest::~GetSymbolRequest()
	{

	}

	bool GetSymbolRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetType(cgi["type"]);
		return true;
	}

	const char*	GetSymbolRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetSymbolRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetSymbolRequest::GetRequest()
	{
		return "GetSymbol";
	}

	const char* GetSymbolRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetSymbolRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetSymbolRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetSymbolRequest::SetName(const char* name)
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

	const char*	GetSymbolRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void GetSymbolRequest::SetType(const char* type)
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

	const char* GetSymbolRequest::GetType()
	{
		return m_type.empty() ? NULL : m_type.c_str();
	}
}