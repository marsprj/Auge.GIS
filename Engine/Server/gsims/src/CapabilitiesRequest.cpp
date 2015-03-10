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
		return "ims";
	}

	const char*	CapabilitiesRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CapabilitiesRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	void CapabilitiesRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
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
		return true;
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

	const char* CapabilitiesRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* CapabilitiesRequest::GetHost()
	{
		return m_host.c_str();
	}

	void CapabilitiesRequest::Debug()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetUser();
		g_sprintf(str,"\t[%s]:%s", "User", (value==NULL)?"":value);
		pLogger->Debug(str);
		g_sprintf(str,"\t[%s]:%s", "Service", "ims");
		pLogger->Debug(str);
		value = GetVersion();
		g_sprintf(str,"\t[%s]:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);
		value = GetRequest();
		g_sprintf(str,"\t[%s]:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);
	}

}
