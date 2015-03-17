#include "AddStyleRequest.h"

namespace auge
{
	AddStyleRequest::AddStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	AddStyleRequest::~AddStyleRequest()
	{

	}

	bool AddStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetStyle(cgi["type"]);
		SetStyle(cgi["style"]);
		return true;
	}

	const char*	AddStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	AddStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	AddStyleRequest::GetRequest()
	{
		return "AddStyle";
	}

	void AddStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	AddStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool AddStyleRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void AddStyleRequest::SetName(const char* name)
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

	const char*	AddStyleRequest::GetName()
	{
		return m_name.c_str();
	}

	void AddStyleRequest::SetStyle(const char* style)
	{
		if(style==NULL)
		{
			m_style.clear();
		}
		else
		{
			m_style = style;
		}
	}

	const char*	AddStyleRequest::GetStyle()
	{
		return m_style.c_str();
	}

	void AddStyleRequest::SetType(const char* type)
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

	const char*	AddStyleRequest::GetType()
	{
		return m_type.c_str();
	}
}