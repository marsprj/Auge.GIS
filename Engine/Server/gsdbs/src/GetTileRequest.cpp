#include "GetTileRequest.h"

namespace auge
{	
	std::string GetTileRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GetTileRequest::DEFAULT_METHOD = "wms";

	GetTileRequest::GetTileRequest()
	{
		m_mime_type = "image/png";
		m_level = 1;
		m_row = -1;
		m_col = -1;
	}

	GetTileRequest::~GetTileRequest()
	{

	}


	const char*	GetTileRequest::GetRequest()
	{
		return "GetTile"; 
	}

	//service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/jpeg&tilematrixset=pgis_tile_store&tilematrix=1&tilerow=0&tilecol=1
	bool GetTileRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetMapName(cgi["mapName"]);
		SetStoreName(cgi["layer"]);
		//SetMapName(cgi["layer"]);
		
		SetStyle(cgi["style"]);
		SetFormat(cgi["format"]);
		SetMatrixSet(cgi["tilematrixset"]);
		SetLevel(cgi["tilematrix"]);
		SetRow(cgi["tilerow"]);
		SetCol(cgi["tilecol"]);

		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Debug(m_map_name.c_str(),__FILE__,__LINE__);

		//SetServiceName(cgi["servicename"]);
		return true;
	}

	const char*	GetTileRequest::GetEngine()
	{
		return "wms";
	}

	const char*	GetTileRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetTileRequest::GetEncoding()
	{
		return "GBK";
	}

	const char* GetTileRequest::GetStoreName()
	{
		return m_store_name.empty() ? NULL : m_store_name.c_str();
	}

	void GetTileRequest::SetStoreName(const char* name)
	{
		if(name)
		{
			m_store_name = name;
		}
		else
		{
			m_store_name.clear();
		}
	}

	void GetTileRequest::SetStyle(const char* style)
	{
		if(style)
		{
			m_style = style;
		}
		else
		{
			m_style.clear();
		}
	}

	const char* GetTileRequest::GetStyle()
	{
		return m_style.empty() ? NULL : m_style.c_str();
	}

	void GetTileRequest::SetFormat(const char* format)
	{
		if(format)
		{
			m_mime_type = format;
		}
		else
		{
			m_mime_type.clear();
		}
	}

	const char* GetTileRequest::GetFormat()
	{
		return m_mime_type.empty() ? NULL : m_mime_type.c_str();
	}

	void GetTileRequest::SetMatrixSet(const char* matrixset)
	{
		if(matrixset)
		{
			m_matrix_set = matrixset;
		}
		else
		{
			m_matrix_set.clear();
		}
	}

	const char* GetTileRequest::GetMatrixSet()
	{
		return m_matrix_set.empty() ? NULL : m_matrix_set.c_str();
	}

	void GetTileRequest::SetLevel(const char* level)
	{
		if(level)
		{
			const char* ptr = strstr(level,":");
			const char* plevel = ptr ? ptr+1 : level;
			if(isdigit(plevel[0]))
			{
				m_level = atoi(plevel);
			}
			else
			{
				m_level = -1;
			}
		}
		else
		{
			m_level = -1;
		}
	}

	g_int GetTileRequest::GetLevel()
	{
		return m_level;
	}

	void GetTileRequest::SetRow(const char* row)
	{
		if(row)
		{
			if(isdigit(row[0]))
			{
				m_row = atoi(row);
			}
			else
			{
				m_row = -1;
			}
		}
		else
		{
			m_row = -1;
		}
	}

	g_int GetTileRequest::GetRow()
	{
		return m_row;
	}

	void GetTileRequest::SetCol(const char* col)
	{
		if(col)
		{
			if(isdigit(col[0]))
			{
				m_col = atoi(col);
			}
			else
			{
				m_col = -1;
			}
		}
		else
		{
			m_col = -1;
		}
	}

	g_int GetTileRequest::GetCol()
	{
		return m_col;
	}

	const char* GetTileRequest::GetMapName()
	{
		return m_map_name.c_str();
	}

	void GetTileRequest::SetMapName(const char* mapName)
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

	void GetTileRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void GetTileRequest::SetUser(const char* user)
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

	const char* GetTileRequest::GetUser()
	{
		return m_user.c_str();
	}

	const char*	GetTileRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char* GetTileRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetTileRequest::GetHost()
	{
		return m_host.c_str();
	}

	void GetTileRequest::SetRequestMethod(const char* method)
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

	void GetTileRequest::SetHost(const char* host)
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