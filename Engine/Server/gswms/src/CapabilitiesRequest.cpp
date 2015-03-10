#include "CapabilitiesRequest.h"

namespace auge
{
	std::string CapabilitiesRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string CapabilitiesRequest::DEFAULT_METHOD = "wms";
	
	CapabilitiesRequest::CapabilitiesRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{
		
	}

	CapabilitiesRequest::~CapabilitiesRequest()
	{

	}

	const char*	CapabilitiesRequest::GetEngine()
	{
		return "wms";
	}

	const char*	CapabilitiesRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CapabilitiesRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	const char*	CapabilitiesRequest::GetServiceName()
	{
		return m_service_name.c_str();
	}

	void CapabilitiesRequest::SetServiceName(const char* serviceName)
	{
		if(serviceName==NULL)
		{
			m_service_name.clear();
		}
		else
		{
			m_service_name = serviceName;
		}
	}

	void CapabilitiesRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void CapabilitiesRequest::SetUser(const char* user)
	{
		if(user==NULL)
		{
			m_user.clear();
		}
		else
		{
			m_user = user;
		}
	}

	const char* CapabilitiesRequest::GetUser()
	{
		return m_user.c_str();
	}

	const char*	CapabilitiesRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool CapabilitiesRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetServiceName(cgi["servicename"]);
		return true;
	}

	const char* CapabilitiesRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* CapabilitiesRequest::GetHost()
	{
		return m_host.c_str();
	}

	void CapabilitiesRequest::SetRequestMethod(const char* method)
	{
		if(method==NULL)
		{
			m_request_method = DEFAULT_METHOD;
		}
		else if(!strlen(method))
		{
			m_request_method = DEFAULT_METHOD;
		}
		else
		{
			m_request_method = method;
		}
	}

	void CapabilitiesRequest::SetHost(const char* host)
	{
		if(host==NULL)
		{
			m_host = DEFAULT_HOST;
		}
		else if(!strlen(host))
		{
			m_host = DEFAULT_HOST;
		}
		else
		{
			m_host = host;
		}
	}

}