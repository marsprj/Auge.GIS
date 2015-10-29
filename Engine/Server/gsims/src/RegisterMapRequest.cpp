#include "RegisterMapRequest.h"

namespace auge
{
	RegisterMapRequest::RegisterMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}
	
	RegisterMapRequest::~RegisterMapRequest()
	{

	}

	bool RegisterMapRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);		
		SetMapName(cgi["mapName"]);
		SetServiceName(cgi["serviceName"]);
		 return true;
	}

	const char*	RegisterMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	RegisterMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RegisterMapRequest::GetRequest()
	{
		return "RegisterMap";
	}

	const char* RegisterMapRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RegisterMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RegisterMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}


	void RegisterMapRequest::SetMapName(const char* name)
	{
		if(name==NULL)
		{
			m_map_name.clear();
		}
		else
		{
			m_map_name = name;
		}
	}

	void RegisterMapRequest::SetServiceName(const char* name)
	{
		if(name==NULL)
		{
			m_servcie_name.clear();
		}
		else
		{
			m_servcie_name = name;
		}
	}

	const char*	RegisterMapRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	const char*	RegisterMapRequest::GetServiceName()
	{
		return m_servcie_name.c_str();
	}

}