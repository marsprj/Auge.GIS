#include "TileUpdateRequest.h"

namespace auge
{
	std::string UpdateTileRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string UpdateTileRequest::DEFAULT_METHOD = "gps";

	UpdateTileRequest::UpdateTileRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_output_format("GML2"),
	m_encoding(AUGE_DEFAULT_ENCODING),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD)
	{
		
	}

	UpdateTileRequest::~UpdateTileRequest()
	{

	}

	const char*	UpdateTileRequest::GetEngine()
	{
		return "gps";
	}

	const char*	UpdateTileRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	UpdateTileRequest::GetRequest()
	{
		return "UpdateTile";
	}

	const char* UpdateTileRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void UpdateTileRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	UpdateTileRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool UpdateTileRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetTypeName(cgi["typeName"]);		
		SetMapName(cgi["mapName"]);

		SetSourceName(cgi["sourceName"]);
		SetTileStoreName(cgi["tileStore"]);

		SetLevel(cgi["level"]);
		SetRow(cgi["row"]);
		SetCol(cgi["col"]);

		return true;
	}

	void UpdateTileRequest::SetUser(const char* user)
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

	const char* UpdateTileRequest::GetUser()
	{
		return m_user.c_str();
	}

	void UpdateTileRequest::SetRequestMethod(const char* method)
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

	void UpdateTileRequest::SetHost(const char* host)
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

	const char* UpdateTileRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* UpdateTileRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* UpdateTileRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	void UpdateTileRequest::SetTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_type_name.clear();
		}
		else
		{
			m_type_name = typeName;
		}
	}

	const char* UpdateTileRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	void UpdateTileRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	const char* UpdateTileRequest::GetMapName()
	{
		return m_map_name.empty() ? NULL : m_map_name.c_str();
	}

	void UpdateTileRequest::SetMapName(const char* mapName)
	{
		if(mapName)
		{
			m_map_name = mapName;
		}
		else
		{
			m_map_name.clear();
		}
	}

	void UpdateTileRequest::SetTileStoreName(const char* tileStoreName)
	{
		if(tileStoreName==NULL)
		{
			m_tile_store_name.clear();
		}
		else
		{
			m_tile_store_name = tileStoreName;
		}
	}

	const char* UpdateTileRequest::GetTileStoreName()
	{
		return m_tile_store_name.empty() ? NULL : m_tile_store_name.c_str();
	}

	void UpdateTileRequest::SetOutputFormat(const char* format)
	{
		if(format==NULL)
		{
			m_output_format = "GML2";
		}
		else
		{
			m_output_format = format;
		}
	}

	const char* UpdateTileRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	void UpdateTileRequest::SetLevel(const char* level)
	{
		if(level)
		{
			m_level = atoi(level);
		}
		else
		{
			m_level = -1;
		}
	}

	void UpdateTileRequest::SetRow(const char* row)
	{
		if(row)
		{
			m_row = atoi(row);
		}
		else
		{
			m_row = -1;
		}
	}

	void UpdateTileRequest::SetCol(const char* col)
	{
		if(col)
		{
			m_col = atoi(col);
		}
		else
		{
			m_col = -1;
		}
	}

	g_uint UpdateTileRequest::GetLevel()
	{
		return m_level;
	}

	g_uint UpdateTileRequest::GetRow()
	{
		return m_row;
	}

	g_uint UpdateTileRequest::GetCol()
	{
		return m_col;
	}
}
