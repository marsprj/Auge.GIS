#include "SetStyleRequest.h"

namespace auge
{
	SetStyleRequest::SetStyleRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}
	
	SetStyleRequest::~SetStyleRequest()
	{

	}

	bool SetStyleRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetLayerName(cgi["layer"]);
		SetStyleName(cgi["style"]);
		SetMapName(cgi["mapName"]);
		 return true;
	}

	//const char*	SetStyleRequest::GetEngine()
	//{
	//	return "wms";
	//}

	//const char*	SetStyleRequest::GetVersion()
	//{
	//	return m_version.c_str();
	//}

	const char*	SetStyleRequest::GetRequest()
	{
		return "SetStyle";
	}

	//void SetStyleRequest::SetVersion(const char* value)
	//{
	//	if(value==NULL)
	//	{
	//		return;
	//	}
	//	m_version = value;
	//}

	//const char*	SetStyleRequest::GetMimeType()
	//{
	//	return m_mime_type.c_str();
	//}

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

	const char*	SetStyleRequest::GetLayerName()
	{
		return m_layer_name.c_str();
	}

	const char*	SetStyleRequest::GetStyleName()
	{
		return m_style_name.c_str();
	}
}