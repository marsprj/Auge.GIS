#include "WorkspaceTFS.h"
#include <math.h>

namespace auge
{
	TileWorkspaceFS::TileWorkspaceFS()
	{
		m_tile_type = augeTileGoogleCRS84Quad;
		m_start_level = 1;
		m_end_level = 18;
		m_tile_format = "png";
		m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		m_extent = m_full_extent;
	}

	TileWorkspaceFS::~TileWorkspaceFS()
	{

	}

	GEnvelope& TileWorkspaceFS::GetExtent()
	{
		return m_extent;
	}

	GEnvelope& TileWorkspaceFS::GetFullExtent()
	{
		return m_full_extent;
	}

	g_uint TileWorkspaceFS::GetRows(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2.0f, l-1);
	}
	
	g_uint TileWorkspaceFS::GetCols(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2.0f, l);
	}

	const char*	TileWorkspaceFS::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void TileWorkspaceFS::SetName(const char* name)
	{
		if(name)
		{
			m_name = name;
		}
		else
		{
			m_name.clear();
		}
	}

	RESULTCODE TileWorkspaceFS::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		m_connstring = conn_string;
		size_t epos = m_connstring.find_first_of("=");
		if(epos==std::string::npos)
		{
			m_connstring.clear();
			return AG_FAILURE;
		}

		m_path = m_connstring.substr(epos+1, m_connstring.length()-epos-1);
		return AG_SUCCESS;
	}

	const char*	TileWorkspaceFS::GetConnectionString()
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceFS::Open()
	{
		GError* pError = augeGetErrorInstance();

		if(m_path.empty())
		{
			const char* msg = "Path is NULL";
			pError->SetError(msg);
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_SUCCESS;
		if(g_access(m_path.c_str(), 4))
		{
			rc = auge_mkdir(m_path.c_str());
			if(rc!=AG_SUCCESS)
			{
				char msg[AUGE_MSG_MAX] = {0};
				g_sprintf(msg, "Cannot Create Folder [%s]", m_path.c_str());
				pError->SetError(msg);
			}
		}

		CreateLevels(m_start_level,m_end_level);
		
		return AG_SUCCESS;
	}

	void TileWorkspaceFS::Close()
	{

	}

	bool TileWorkspaceFS::IsOpen()
	{
		return true;
	}

	DataSet* TileWorkspaceFS::OpenDataSet(const char* name)
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceFS::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
	}

	DataEngine*	TileWorkspaceFS::GetEngine()
	{
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	const char* TileWorkspaceFS::CreateTileStore(const char* name, augeTileType type)
	{
		return NULL;
	}
	
	TileStore* TileWorkspaceFS::GetTileStore(const char* name)
	{
		return NULL;
	}
	
	RESULTCODE TileWorkspaceFS::RemoveTileStore()
	{
		return AG_SUCCESS;
	}

	augeTileType TileWorkspaceFS::GetTileType()
	{
		return m_tile_type;
	}

	Tile* TileWorkspaceFS::GetTile(g_uint level, g_uint row, g_uint col)
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceFS::PutTile(g_uint level, g_uint row, g_uint col, const char* path)
	{
		return AG_SUCCESS;
	}

	RESULTCODE TileWorkspaceFS::PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size)
	{
		return AG_SUCCESS;
	}
	/*
	 * ------------------------------------------------------------
	 * augeTileGoogleCrs84Quad
	 *	[Row]: 0 ~ 2^(l-1)
	 *	[Col]: 0 ~ 2^l
	 * ------------------------------------------------------------
	 */
	RESULTCODE TileWorkspaceFS::GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{
		return AG_SUCCESS;
	}

	RESULTCODE TileWorkspaceFS::GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col)
	{
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level-1));
		double xmin = m_full_extent.m_xmin + resolution * col;
		double ymax = m_full_extent.m_ymax - resolution * row;
		double xmax = xmin + resolution;
		double ymin = ymax - resolution;
		extent.Set(xmin,ymin,xmax,ymax);
		return AG_SUCCESS;
	}

	RESULTCODE TileWorkspaceFS::GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{	
		g_snprintf(key, size, "%s/%d/%d_%d.%s",m_path.c_str(), level, row, col,m_tile_format.c_str());
		return AG_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE	TileWorkspaceFS::CreateLevels(g_uint start, g_uint end)
	{
		char level[AUGE_NAME_MAX];
		char l_path[AUGE_PATH_MAX] = {0};
		for(g_uint i=start; i<=end; i++)
		{
			g_sprintf(level, "%d", i);
			auge_make_path(l_path,NULL, m_path.c_str(), level, NULL);

			if(g_access(l_path, 4))
			{
				auge_mkdir(l_path);
			}
		}

		return AG_SUCCESS;
	}
}