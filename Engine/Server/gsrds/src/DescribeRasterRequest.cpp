#include "DescribeRasterRequest.h"

namespace auge
{
	DescribeRasterRequest::DescribeRasterRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	DescribeRasterRequest::~DescribeRasterRequest()
	{

	}

	bool DescribeRasterRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		SetRasterName(cgi["rasterName"]);
		SetSourceName(cgi["sourceName"]);
		return true;
	}

	const char*	DescribeRasterRequest::GetEngine()
	{
		return "rds";
	}

	const char*	DescribeRasterRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeRasterRequest::GetRequest()
	{
		return "DescribeRaster";
	}

	void DescribeRasterRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeRasterRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void DescribeRasterRequest::SetPath(const char* path)
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

	const char* DescribeRasterRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	DescribeRasterRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DescribeRasterRequest::SetRasterName(const char* name)
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

	const char* DescribeRasterRequest::GetRasterName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void DescribeRasterRequest::SetSourceName(const char* sourceName)
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

	const char* DescribeRasterRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}
}