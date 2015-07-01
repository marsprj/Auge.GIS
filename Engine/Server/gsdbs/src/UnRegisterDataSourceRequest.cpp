#include "UnRegisterDataSourceRequest.h"

namespace auge
{
	UnRegisterDataSourceRequest::UnRegisterDataSourceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	UnRegisterDataSourceRequest::~UnRegisterDataSourceRequest()
	{

	}

	bool UnRegisterDataSourceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	UnRegisterDataSourceRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	UnRegisterDataSourceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UnRegisterDataSourceRequest::GetRequest()
	{
		return "UnRegisterDataSource";
	}

	const char* UnRegisterDataSourceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void UnRegisterDataSourceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UnRegisterDataSourceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void UnRegisterDataSourceRequest::SetName(const char* name)
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

	const char*	UnRegisterDataSourceRequest::GetName()
	{
		return m_name.c_str();
	}

}