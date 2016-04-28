#include "StopServiceRequest.h"

namespace auge
{
	StopServiceRequest::StopServiceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding =AUGE_DB_ENCODING;
	}

	StopServiceRequest::~StopServiceRequest()
	{

	}

	bool StopServiceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);		
		return true;
	}

	const char*	StopServiceRequest::GetEngine()
	{
		return "ims";
	}

	const char*	StopServiceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	StopServiceRequest::GetRequest()
	{
		return "StopService";
	}

	const char* StopServiceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void StopServiceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	StopServiceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool StopServiceRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void StopServiceRequest::SetName(const char* name)
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

	const char*	StopServiceRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}
}