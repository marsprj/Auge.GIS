#include "GetRasterRequest.h"

namespace auge
{
	GetRasterRequest::GetRasterRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	GetRasterRequest::~GetRasterRequest()
	{

	}

	bool GetRasterRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	GetRasterRequest::GetEngine()
	{
		return "rds";
	}

	const char*	GetRasterRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetRasterRequest::GetRequest()
	{
		return "GetRaster";
	}

	void GetRasterRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetRasterRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetRasterRequest::SetPath(const char* path)
	{
		if(path==NULL)
		{
			m_path.clear();
		}
		else
		{
			m_path = path;
		}
	}

	const char* GetRasterRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	GetRasterRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetRasterRequest::SetName(const char* name)
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

	const char* GetRasterRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}
}