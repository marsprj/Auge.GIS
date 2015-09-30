#include "DescribeCsvRequest.h"

namespace auge
{
	DescribeCsvRequest::DescribeCsvRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	DescribeCsvRequest::~DescribeCsvRequest()
	{

	}

	bool DescribeCsvRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		return true;
	}

	const char*	DescribeCsvRequest::GetEngine()
	{
		return "ims";
	}

	const char*	DescribeCsvRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeCsvRequest::GetRequest()
	{
		return "DescribeCsv";
	}

	const char* DescribeCsvRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DescribeCsvRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeCsvRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool DescribeCsvRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void DescribeCsvRequest::SetPath(const char* path)
	{
		if(path==NULL)
		{
			m_path.clear();
		}
		else if(strlen(path)==0)
		{
			m_path.clear();
		}
		else
		{
			m_path = path;
		}
		
	}

	const char*	DescribeCsvRequest::GetPath()
	{
		if(m_path.empty())
		{
			return NULL;
		}
		return m_path.c_str();
	}

}