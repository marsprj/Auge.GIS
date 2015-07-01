#include "CapabilitiesRequest.h"

namespace auge
{
	std::string GeoProcessingCapabilitiesRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GeoProcessingCapabilitiesRequest::DEFAULT_METHOD = "gps";

	GeoProcessingCapabilitiesRequest::GeoProcessingCapabilitiesRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{
		
	}

	GeoProcessingCapabilitiesRequest::~GeoProcessingCapabilitiesRequest()
	{

	}

	const char*	GeoProcessingCapabilitiesRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GeoProcessingCapabilitiesRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GeoProcessingCapabilitiesRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	void GeoProcessingCapabilitiesRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GeoProcessingCapabilitiesRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char* GeoProcessingCapabilitiesRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	bool GeoProcessingCapabilitiesRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		return true;
	}

	void GeoProcessingCapabilitiesRequest::SetUser(const char* user)
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

	const char* GeoProcessingCapabilitiesRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GeoProcessingCapabilitiesRequest::SetRequestMethod(const char* method)
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

	void GeoProcessingCapabilitiesRequest::SetHost(const char* host)
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

	const char* GeoProcessingCapabilitiesRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GeoProcessingCapabilitiesRequest::GetHost()
	{
		return m_host.c_str();
	}

	void GeoProcessingCapabilitiesRequest::Info()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetRequest();
		pLogger->Debug("[Requet Parameters]");
		g_sprintf(str,"\t%s:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);
		value = GetUser();
		g_sprintf(str,"\t%s:%s", "User", (value==NULL)?"":value);
		pLogger->Debug(str);
		g_sprintf(str,"\t%s:%s", "Service", "ims");
		pLogger->Debug(str);
		value = GetVersion();
		g_sprintf(str,"\t%s:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);
		
	}

}
