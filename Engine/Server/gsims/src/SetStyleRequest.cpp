#include "SetStyleRequest.h"

namespace auge
{
	SetStyleRequest::SetStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}
	
	SetStyleRequest::~SetStyleRequest()
	{

	}

	bool SetStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetMapName(cgi["map"]);
		SetLayerName(cgi["layer"]);
		SetStyleName(cgi["style"]);
		 return true;
	}

	const char* SetStyleRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	SetStyleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	SetStyleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	SetStyleRequest::GetRequest()
	{
		return "SetStyle";
	}

	void SetStyleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	SetStyleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}


	void SetStyleRequest::SetMapName(const char* mapName)
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
	
	void SetStyleRequest::SetLayerName(const char* layerName)
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

	void SetStyleRequest::SetStyleName(const char* styleName)
	{
		if(styleName==NULL)
		{
			m_style_name.clear();
		}
		else
		{
			m_style_name = styleName;
		}
	}

	const char*	SetStyleRequest::GetMapName()
	{
		return m_map_name.c_str();
	}
	
	const char*	SetStyleRequest::GetLayerName()
	{
		return m_layer_name.c_str();
	}

	const char*	SetStyleRequest::GetStyleName()
	{
		return m_style_name.c_str();
	}
}