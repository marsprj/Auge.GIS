#include "AddRasterRequest.h"

namespace auge
{
	AddRasterRequest::AddRasterRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	AddRasterRequest::~AddRasterRequest()
	{

	}

	bool AddRasterRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		SetName(cgi["name"]);
		SetSourceName(cgi["sourceName"]);
		return true;
	}

	const char*	AddRasterRequest::GetEngine()
	{
		return "rds";
	}

	const char*	AddRasterRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	AddRasterRequest::GetRequest()
	{
		return "AddRaster";
	}

	void AddRasterRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	AddRasterRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void AddRasterRequest::SetPath(const char* path)
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

	const char* AddRasterRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	AddRasterRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void AddRasterRequest::SetName(const char* name)
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

	const char* AddRasterRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void AddRasterRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	const char* AddRasterRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}
}