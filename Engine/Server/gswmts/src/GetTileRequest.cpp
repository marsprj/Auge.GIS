#include "GetTileRequest.h"

namespace auge
{	
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

		//SetServiceName(cgi["servicename"]);
		return true;
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

}