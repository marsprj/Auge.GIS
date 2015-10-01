#include "RemoveServiceRequest.h"

namespace auge
{
	RemoveServiceRequest::RemoveServiceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding =AUGE_DB_ENCODING;
	}

	RemoveServiceRequest::~RemoveServiceRequest()
	{

	}

	bool RemoveServiceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);		
		return true;
	}

	const char*	RemoveServiceRequest::GetEngine()
	{
		return "ims";
	}

	const char*	RemoveServiceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveServiceRequest::GetRequest()
	{
		return "RemoveService";
	}

	const char* RemoveServiceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RemoveServiceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveServiceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool RemoveServiceRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void RemoveServiceRequest::SetName(const char* name)
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

	const char*	RemoveServiceRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}
}