#include "GetMapRequest.h"

namespace auge
{
	GetMapRequest::GetMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	GetMapRequest::~GetMapRequest()
	{

	}

	bool GetMapRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	GetMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetMapRequest::GetRequest()
	{
		return "GetMap";
	}

	void GetMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool GetMapRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void GetMapRequest::SetName(const char* name)
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

	const char*	GetMapRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}