#include "WFeatureRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	std::string WFeatureRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string WFeatureRequest::DEFAULT_METHOD = "wfs";

	WFeatureRequest::WFeatureRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding("GBK"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	WFeatureRequest::~WFeatureRequest()
	{

	}

	const char*	WFeatureRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	WFeatureRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	WFeatureRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	const char*	WFeatureRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	//const char*	WFeatureRequest::GetServiceName()
	//{
	//	return m_service_name.c_str();
	//}

	//void WFeatureRequest::SetServiceName(const char* serviceName)
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

	const char* WFeatureRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	void WFeatureRequest::SetMapName(const char* mapName)
	{
		if((mapName==NULL)||(strlen(mapName)==0))
		{
			m_map_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			if(pWebContext->IsIE())
			{
				m_map_name = mapName;
			}
			else
			{
				m_map_name = auge_encoding_convert(AUGE_ENCODING_UTF8, AUGE_ENCODING_GBK, mapName, strlen(mapName));
			}
		}
	}

	const char* WFeatureRequest::GetSourceName()
	{
		return m_source_name.c_str();
	}

	void WFeatureRequest::SetSourceName(const char* sourceName)
	{
		if((sourceName==NULL)||(strlen(sourceName)==0))
		{
			m_source_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			if(pWebContext->IsIE())
			{
				m_source_name = sourceName;
			}
			else
			{
				m_source_name = auge_encoding_convert(AUGE_ENCODING_UTF8, AUGE_ENCODING_GBK, sourceName, strlen(sourceName));
			}
		}
	}

	bool WFeatureRequest::IsValidSource()
	{
		return !m_source_name.empty();
	}

	void WFeatureRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void WFeatureRequest::SetUser(const char* user)
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

	const char* WFeatureRequest::GetUser()
	{
		return m_user.c_str();
	}

	const char*	WFeatureRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char* WFeatureRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* WFeatureRequest::GetHost()
	{
		return m_host.c_str();
	}

	void WFeatureRequest::SetRequestMethod(const char* method)
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

	void WFeatureRequest::SetHost(const char* host)
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
