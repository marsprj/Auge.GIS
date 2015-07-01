#include "UnRegisterLayerRequest.h"

namespace auge
{
	UnRegisterLayerRequest::UnRegisterLayerRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}
	
	UnRegisterLayerRequest::~UnRegisterLayerRequest()
	{

	}

	bool UnRegisterLayerRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetMapName(cgi["mapName"]);
		SetLayerName(cgi["layerName"]);
		 return true;
	}

	const char*	UnRegisterLayerRequest::GetEngine()
	{
		return "ims";
	}

	const char*	UnRegisterLayerRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UnRegisterLayerRequest::GetRequest()
	{
		return "UnRegisterLayer";
	}

	const char*	UnRegisterLayerRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void UnRegisterLayerRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UnRegisterLayerRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}


	void UnRegisterLayerRequest::SetMapName(const char* mapName)
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

	void UnRegisterLayerRequest::SetLayerName(const char* layerName)
	{
		if(layerName==NULL)
		{
			m_layer_name.clear();
		}
		else
		{
			m_layer_name = layerName;
		}
	}

	const char*	UnRegisterLayerRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	const char*	UnRegisterLayerRequest::GetLayerName()
	{
		return m_layer_name.c_str();
	}
}