#include "GetStyleRequest.h"

namespace auge
{
	GetStyleRequest::GetStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetStyleRequest::~GetStyleRequest()
	{

	}

	bool GetStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	GetStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetStyleRequest::GetRequest()
	{
		return "GetStyle";
	}

	const char* GetStyleRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool GetStyleRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void GetStyleRequest::SetName(const char* name)
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

	const char*	GetStyleRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}