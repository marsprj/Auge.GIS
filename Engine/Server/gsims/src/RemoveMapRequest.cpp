#include "RemoveMapRequest.h"

namespace auge
{
	RemoveMapRequest::RemoveMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	RemoveMapRequest::~RemoveMapRequest()
	{

	}

	bool RemoveMapRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		return true;
	}

	const char*	RemoveMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	RemoveMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveMapRequest::GetRequest()
	{
		return "RemoveMap";
	}

	const char* RemoveMapRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RemoveMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool RemoveMapRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void RemoveMapRequest::SetName(const char* name)
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
			//m_name = name;
		}
	}

	const char*	RemoveMapRequest::GetName()
	{
		return m_name.c_str();
	}

}