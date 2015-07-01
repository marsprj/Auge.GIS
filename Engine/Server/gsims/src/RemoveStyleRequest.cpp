#include "RemoveStyleRequest.h"

namespace auge
{
	RemoveStyleRequest::RemoveStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	RemoveStyleRequest::~RemoveStyleRequest()
	{

	}

	bool RemoveStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	RemoveStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	RemoveStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveStyleRequest::GetRequest()
	{
		return "RemoveStyle";
	}

	const char* RemoveStyleRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RemoveStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool RemoveStyleRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void RemoveStyleRequest::SetName(const char* name)
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

	const char*	RemoveStyleRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}