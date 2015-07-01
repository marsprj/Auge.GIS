#include "BuildPyramidRequest.h"

namespace auge
{
	std::string BuildPyramidRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string BuildPyramidRequest::DEFAULT_METHOD = "gps";

	BuildPyramidRequest::BuildPyramidRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_output_format("GML2"),
		m_start_level(-1),
		m_end_level(-1),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD),
		m_encoding(AUGE_DEFAULT_ENCODING)
	{

	}

	BuildPyramidRequest::~BuildPyramidRequest()
	{

	}

	const char*	BuildPyramidRequest::GetEngine()
	{
		return "gps";
	}

	const char*	BuildPyramidRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	BuildPyramidRequest::GetRequest()
	{
		return "BuildPyramid";
	}

	const char* BuildPyramidRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void BuildPyramidRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	BuildPyramidRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool BuildPyramidRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetTypeName(cgi["typeName"]);		
		SetMapName(cgi["mapName"]);

		SetSourceName(cgi["sourceName"]);
		SetTileStoreName(cgi["tileStore"]);

		SetStartLevel(cgi["start"]);
		SetEndLevel(cgi["end"]);

		return true;
	}

	void BuildPyramidRequest::SetUser(const char* user)
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

	const char* BuildPyramidRequest::GetUser()
	{
		return m_user.c_str();
	}

	void BuildPyramidRequest::SetRequestMethod(const char* method)
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

	void BuildPyramidRequest::SetHost(const char* host)
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

	const char* BuildPyramidRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* BuildPyramidRequest::GetHost()
	{
		return m_host.c_str();
	}

	//const char* BuildPyramidRequest::GetTypeName()
	//{
	//	return m_type_name.c_str();
	//}

	void BuildPyramidRequest::SetTypeName(const char* typeName)
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

	const char* BuildPyramidRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	void BuildPyramidRequest::SetSourceName(const char* sourceName)
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

	const char* BuildPyramidRequest::GetMapName()
	{
		return m_map_name.empty() ? NULL : m_map_name.c_str();
	}

	void BuildPyramidRequest::SetMapName(const char* mapName)
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

	void BuildPyramidRequest::SetTileStoreName(const char* tileStoreName)
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

	const char* BuildPyramidRequest::GetTileStoreName()
	{
		return m_tile_store_name.empty() ? NULL : m_tile_store_name.c_str();
	}

	void BuildPyramidRequest::SetOutputFormat(const char* format)
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

	const char* BuildPyramidRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	void BuildPyramidRequest::SetStartLevel(const char* level)
	{
		if(level)
		{
			if(isdigit(level[0]))
			{
				m_start_level = atoi(level);
			}
		}
	}
	
	void BuildPyramidRequest::SetEndLevel(const char* level)
	{
		if(level)
		{
			if(isdigit(level[0]))
			{
				m_end_level = atoi(level);
			}
		}
	}

	g_uint BuildPyramidRequest::GetStartLevel()
	{
		return m_start_level;
	}

	g_uint BuildPyramidRequest::GetEndLevel()
	{
		return m_end_level;
	}
}
