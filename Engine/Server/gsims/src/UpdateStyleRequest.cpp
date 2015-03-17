#include "UpdateStyleRequest.h"

namespace auge
{
	UpdateStyleRequest::UpdateStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	UpdateStyleRequest::~UpdateStyleRequest()
	{

	}

	bool UpdateStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetStyle(cgi["type"]);
		SetStyle(cgi["style"]);
		return true;
	}

	const char*	UpdateStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	UpdateStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UpdateStyleRequest::GetRequest()
	{
		return "UpdateStyle";
	}

	void UpdateStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UpdateStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool UpdateStyleRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void UpdateStyleRequest::SetName(const char* name)
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

	const char*	UpdateStyleRequest::GetName()
	{
		return m_name.c_str();
	}

	void UpdateStyleRequest::SetStyle(const char* style)
	{
		if(style==NULL)
		{
			m_style.c_str();
		}
		else
		{
			m_style = style;
		}
	}
	
	const char*	UpdateStyleRequest::GetStyle()
	{
		return m_style.c_str();
	}

	void UpdateStyleRequest::SetType(const char* type)
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

	const char*	UpdateStyleRequest::GetType()
	{
		return m_type.c_str();
	}
}