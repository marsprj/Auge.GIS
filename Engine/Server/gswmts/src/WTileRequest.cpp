#include "WTileRequest.h"

namespace auge
{
	std::string WTileRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string WTileRequest::DEFAULT_METHOD = "wms";

	WTileRequest::WTileRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	WTileRequest::~WTileRequest()
	{

	}

	const char*	WTileRequest::GetEngine()
	{
		return "wms";
	}

	const char*	WTileRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	WTileRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	const char* WTileRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	//const char*	WTileRequest::GetServiceName()
	//{
	//	return m_service_name.c_str();
	//}

	//void WTileRequest::SetServiceName(const char* serviceName)
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

	const char* WTileRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	void WTileRequest::SetMapName(const char* mapName)
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

	void WTileRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void WTileRequest::SetUser(const char* user)
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

	const char* WTileRequest::GetUser()
	{
		return m_user.c_str();
	}

	const char*	WTileRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}
	
	const char* WTileRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* WTileRequest::GetHost()
	{
		return m_host.c_str();
	}

	void WTileRequest::SetRequestMethod(const char* method)
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

	void WTileRequest::SetHost(const char* host)
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