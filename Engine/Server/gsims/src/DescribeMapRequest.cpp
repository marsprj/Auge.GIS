#include "DescribeMapRequest.h"

namespace auge
{
	DescribeMapRequest::DescribeMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	DescribeMapRequest::~DescribeMapRequest()
	{

	}

	bool DescribeMapRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	DescribeMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	DescribeMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeMapRequest::GetRequest()
	{
		return "DescribeMap";
	}

	const char* DescribeMapRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DescribeMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool DescribeMapRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void DescribeMapRequest::SetName(const char* name)
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

	const char*	DescribeMapRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}