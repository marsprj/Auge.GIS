#include "GetServiceRequest.h"

namespace auge
{
	GetServiceRequest::GetServiceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	GetServiceRequest::~GetServiceRequest()
	{

	}

	bool GetServiceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	GetServiceRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetServiceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetServiceRequest::GetRequest()
	{
		return "GetService";
	}

	void GetServiceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetServiceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool GetServiceRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void GetServiceRequest::SetName(const char* name)
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

	const char*	GetServiceRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

}