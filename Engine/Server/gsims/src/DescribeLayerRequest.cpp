#include "DescribeLayerRequest.h"

namespace auge
{
	DescribeLayerRequest::DescribeLayerRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	DescribeLayerRequest::~DescribeLayerRequest()
	{

	}

	bool DescribeLayerRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetMapName(cgi["mapName"]);
		SetLayerName(cgi["layerName"]);
		return true;
	}

	const char*	DescribeLayerRequest::GetEngine()
	{
		return "ims";
	}

	const char*	DescribeLayerRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeLayerRequest::GetRequest()
	{
		return "DescribeLayer";
	}

	const char* DescribeLayerRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DescribeLayerRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeLayerRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool DescribeLayerRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void DescribeLayerRequest::SetMapName(const char* name)
	{
		if(name==NULL)
		{
			m_map_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_map_name.clear();
		}
		else
		{
			m_map_name = name;
		}
		
	}

	const char*	DescribeLayerRequest::GetMapName()
	{
		if(m_map_name.empty())
		{
			return NULL;
		}
		return m_map_name.c_str();
	}

	void DescribeLayerRequest::SetLayerName(const char* name)
	{
		if(name==NULL)
		{
			m_layer_name.clear();
		}
		else
		{
			m_layer_name = name;
		}

	}

	const char*	DescribeLayerRequest::GetLayerName()
	{
		return m_layer_name.empty() ? NULL : m_layer_name.c_str();
	}

}