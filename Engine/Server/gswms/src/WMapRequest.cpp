#include "WMapRequest.h"

namespace auge
{
	std::string WMapRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string WMapRequest::DEFAULT_METHOD = "wms";

	WMapRequest::WMapRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	WMapRequest::~WMapRequest()
	{

	}

	const char*	WMapRequest::GetEngine()
	{
		return "wms";
	}

	const char*	WMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	WMapRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	const char* WMapRequest::GetEncoding()
	{
		return  m_encoding.c_str();
	}

	//const char*	WMapRequest::GetServiceName()
	//{
	//	return m_service_name.c_str();
	//}

	//void WMapRequest::SetServiceName(const char* serviceName)
	//{
	//	if(serviceName==NULL)
	//	{
	//		m_service_name.clear();
	//	}
	//	else
	//	{
	//		m_service_name = serviceName;
	//	}
	//}

	const char* WMapRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	void WMapRequest::SetMapName(const char* mapName)
	{
		if(mapName==NULL)
		{
			m_map_name.clear();
		}
		else
		{
			m_map_name = mapName;
		}
	}

	void WMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void WMapRequest::SetUser(const char* user)
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

	const char* WMapRequest::GetUser()
	{
		return m_user.c_str();
	}

	const char*	WMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}
	
	const char* WMapRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* WMapRequest::GetHost()
	{
		return m_host.c_str();
	}

	void WMapRequest::SetRequestMethod(const char* method)
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

	void WMapRequest::SetHost(const char* host)
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