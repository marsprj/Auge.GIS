#include "DescribeServiceRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	DescribeServiceRequest::DescribeServiceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	DescribeServiceRequest::~DescribeServiceRequest()
	{

	}

	bool DescribeServiceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetEncoding(cgi["encoding"]);
		return true;
	}

	const char*	DescribeServiceRequest::GetEngine()
	{
		return "ims";
	}

	const char*	DescribeServiceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeServiceRequest::GetRequest()
	{
		return "DescribeService";
	}

	const char* DescribeServiceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DescribeServiceRequest::SetEncoding(const char encoding)
	{
		if(encoding==NULL)
		{
			m_encoding = AUGE_ENCODING_UTF8;
		}
		else
		{
			if(strlen(encoding)==0)
			{
				m_encoding = AUGE_ENCODING_UTF8;
			}
			else
			{
				m_encoding = AUGE_ENCODING_GBK;
			}
		}
	}

	void DescribeServiceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeServiceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool DescribeServiceRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void DescribeServiceRequest::SetName(const char* name)
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
			WebContext* pWebContext = augeGetWebContextInstance();
			m_name = pWebContext->ParameterEncoding(name);
		}
		
	}

	const char*	DescribeServiceRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}