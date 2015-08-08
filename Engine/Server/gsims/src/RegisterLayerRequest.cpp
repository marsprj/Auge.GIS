#include "RegisterLayerRequest.h"

namespace auge
{
	RegisterLayerRequest::RegisterLayerRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_type = augeLayerNone;
		m_encoding = AUGE_DEFAULT_ENCODING;
	}
	
	RegisterLayerRequest::~RegisterLayerRequest()
	{

	}

	bool RegisterLayerRequest::Create(rude::CGI& cgi)
	{
		SetType(cgi["type"]);
		SetVersion(cgi["version"]);
		SetMapName(cgi["mapName"]);
		SetDataSource(cgi["datasource"]);
		SetType(cgi["layerType"]);
		SetLayerName(cgi["layerName"]);
		SetTableName(cgi["tableName"]);
		
		 return true;
	}

	const char*	RegisterLayerRequest::GetEngine()
	{
		return "ims";
	}

	const char*	RegisterLayerRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RegisterLayerRequest::GetRequest()
	{
		return "RegisterLayer";
	}

	const char* RegisterLayerRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	augeLayerType RegisterLayerRequest::GetType()
	{
		return m_type;
	}

	void RegisterLayerRequest::SetType(const char* type)
	{
		if(type==NULL)
		{
			m_type = augeLayerNone;
		}
		else if(g_stricmp(type,"Feature")==0)
		{
			m_type = augeLayerFeature;
		}
		else if(g_stricmp(type,"Raster")==0)
		{
			m_type = augeLayerRaster;
		}
		else if(g_stricmp(type,"QuadServer")==0)
		{
			m_type = augeLayerQuadServer;
		}
		else
		{
			m_type = augeLayerNone;
		}
	}

	void RegisterLayerRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RegisterLayerRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}


	void RegisterLayerRequest::SetMapName(const char* mapName)
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

	void RegisterLayerRequest::SetDataSource(const char* dataSource)
	{
		if(dataSource==NULL)
		{
			m_data_source.clear();
		}
		else
		{
			m_data_source = dataSource;
		}
	}

	void RegisterLayerRequest::SetLayerName(const char* layerName)
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

	void RegisterLayerRequest::SetTableName(const char* tableName)
	{
		if(tableName==NULL)
		{
			m_table_name.clear();
		}
		else
		{
			m_table_name = tableName;
		}
	}

	const char*	RegisterLayerRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	const char*	RegisterLayerRequest::GetDataSource()
	{
		return m_data_source.c_str();
	}

	const char*	RegisterLayerRequest::GetLayerName()
	{
		return m_layer_name.c_str();
	}

	const char*	RegisterLayerRequest::GetTableName()
	{
		return m_table_name.c_str();
	}

	const char* RegisterLayerRequest::GetWebURL()
	{
		return m_web_url.empty() ? NULL : m_web_url.c_str();
	}

	void RegisterLayerRequest::SetWebURL(const char* url)
	{
		if(url==NULL)
		{
			m_web_url.clear();
		}
		else
		{
			m_web_url = url;
		}
	}
}