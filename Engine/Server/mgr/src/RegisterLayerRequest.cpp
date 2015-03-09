#include "RegisterLayerRequest.h"

namespace auge
{
	RegisterLayerRequest::RegisterLayerRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}
	
	RegisterLayerRequest::~RegisterLayerRequest()
	{

	}

	bool RegisterLayerRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetMapName(cgi["mapName"]);
		SetDataSource(cgi["datasource"]);
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
}