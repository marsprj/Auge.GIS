#include "UpdateMapLayerOrderRequest.h"

namespace auge
{
	UpdateMapLayerOrderRequest::UpdateMapLayerOrderRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	UpdateMapLayerOrderRequest::~UpdateMapLayerOrderRequest()
	{

	}

	bool UpdateMapLayerOrderRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetLayers(cgi["layers"]);
		return true;
	}

	const char*	UpdateMapLayerOrderRequest::GetEngine()
	{
		return "ims";
	}

	const char*	UpdateMapLayerOrderRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UpdateMapLayerOrderRequest::GetRequest()
	{
		return "UpdateMapLayerOrder";
	}

	const char* UpdateMapLayerOrderRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void UpdateMapLayerOrderRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UpdateMapLayerOrderRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void UpdateMapLayerOrderRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}
	
	const char*	UpdateMapLayerOrderRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

	const char*	UpdateMapLayerOrderRequest::GetLayers()
	{
		return m_layers.c_str();
	}


	void UpdateMapLayerOrderRequest::SetLayers(const char* layers)
	{
		if(layers==NULL)
		{
			m_layers.clear();
		}
		else
		{
			m_layers = layers;
		}
	}
}