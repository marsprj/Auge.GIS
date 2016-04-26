#include "WorkspaceTFS.h"
#include "PGISTileStore.h"
#include "TileImpl.h"
#include <math.h>

namespace auge
{
	PGISTileStore::PGISTileStore()
	{
		m_tile_type = augeTileGoogleCRS84Quad;
		m_start_level = 1;
		m_end_level = 18;
		m_tile_format = "png";
		m_full_extent.Set(-256.0f,-256.0f,256.0f,256.0f);
		m_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		m_name = "default";

		m_pWorkspace = NULL;
	}

	PGISTileStore::~PGISTileStore()
	{

	}

	bool PGISTileStore::IsEmpty()
	{
		return true;
	}

	GEnvelope& PGISTileStore::GetExtent()
	{
		return m_extent;
	}

	GEnvelope& PGISTileStore::GetFullExtent()
	{
		return m_full_extent;
	}

	void PGISTileStore::GetTopLeftCorner(double &x, double &y)
	{
		x = -256;
		y = -256;
	}

	g_uint PGISTileStore::GetStartLevel()
	{
		return m_start_level;
	}

	g_uint PGISTileStore::GetEndLevel()
	{
		return m_end_level;
	}

	g_uint64 PGISTileStore::GetRows(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}
	
	g_uint64 PGISTileStore::GetCols(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}

	g_uint64 PGISTileStore::GetBlockRows(g_uint level)
	{
		return GetRows(level) / (AUGE_TILE_BLOCK_SIZE >> 8);
	}

	g_uint64 PGISTileStore::GetBlockCols(g_uint level)
	{
		return GetCols(level) / (AUGE_TILE_BLOCK_SIZE >> 8);
	}

	TileBlock* PGISTileStore::GetBlock(g_uint level, g_uint64 row, g_uint64 col)
	{
		return NULL;
	}

	const char*	PGISTileStore::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void PGISTileStore::SetName(const char* name)
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
	
	augeTileType PGISTileStore::GetTileType()
	{
		return m_tile_type;
	}

	const char* PGISTileStore::GetTileTypeAsString()
	{
		return AUGE_TILE_TYPE_PGIS;
	}

	Tile* PGISTileStore::GetTile(g_uint level, g_uint64 row, g_uint64 col)
	{
		char t_path[AUGE_PATH_MAX] = {0};
		RESULTCODE rc = GetTilePath(t_path, AUGE_PATH_MAX, level, row, col);
		if(rc!=AG_SUCCESS)
		{
			return NULL;
		}

		TileImpl* pTile = new TileImpl();
		pTile->Create(t_path);

		return pTile;
	}

	RESULTCODE PGISTileStore::PutTile(g_uint level, g_uint64 row, g_uint64 col, const char* path)
	{
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::PutTile(g_uint level, g_uint64 row, g_uint64 col, unsigned char* data, size_t size)
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
	RESULTCODE PGISTileStore::GetKey(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetTileExtent(GEnvelope& extent, g_uint level, g_uint64 row, g_uint64 col)
	{
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level));
		double xmin = m_full_extent.m_xmin + resolution * col;
		double ymax = m_full_extent.m_ymax - resolution * row;
		double xmax = xmin + resolution;
		double ymin = ymax - resolution;
		extent.Set(xmin,ymin,xmax,ymax);

		if(!m_extent.Intersects(extent))
		{
			return AG_FAILURE;
		}
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetTilePath(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{	
		char l_tile[AUGE_NAME_MAX] = {0};
		char r_tile[AUGE_NAME_MAX] = {0};
		char c_tile[AUGE_NAME_MAX] = {0};
		g_sprintf(l_tile,"L%02d", level);
		g_sprintf(r_tile,"R%08x", row);
		g_sprintf(c_tile,"C%08x", col);

		char l_path[AUGE_PATH_MAX] = {0};
		auge_make_path(l_path, NULL, m_layers_path.c_str(), l_tile, NULL);

		char r_path[AUGE_PATH_MAX] = {0};
		auge_make_path(r_path, NULL, l_path, r_tile, NULL);
		auge_mkdir(r_path);

		//char t_path[AUGE_PATH_MAX] = {0};
		auge_make_path(key, NULL, r_path, c_tile, m_tile_format.c_str());

		//g_snprintf(key, size, "%s/%d/%d_%d.%s",m_path.c_str(), level, row, col,m_tile_format.c_str());
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::RemoveTile(g_uint level, g_uint64 row, g_uint64 col)
	{
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::RemoveAll()
	{
		return AG_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE	PGISTileStore::CreateLevels(g_uint start, g_uint end)
	{
		char layers_path[AUGE_PATH_MAX] = {0};
		auge_make_path(layers_path, NULL, m_path.c_str(), "Layers", NULL);
		auge_mkdir(layers_path);

		char _alllayers_path[AUGE_PATH_MAX] = {0};
		auge_make_path(_alllayers_path, NULL, layers_path, "_alllayers", NULL);
		auge_mkdir(_alllayers_path);
		m_layers_path = _alllayers_path;

		char l_name[AUGE_NAME_MAX];
		char l_path[AUGE_PATH_MAX] = {0};
		for(g_uint i=start; i<=end; i++)
		{
			g_sprintf(l_name, "L%02d", i);
			auge_make_path(l_path,NULL, _alllayers_path, l_name, NULL);

			if(g_access(l_path, 4))
			{
				auge_mkdir(l_path);
			}
		}

		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::Create(TileWorkspaceFS* pWorkspace)
	{
		m_pWorkspace = pWorkspace;
		m_path = pWorkspace->GetPath();

		CreateLevels(m_start_level, m_end_level);

		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::Create(TileWorkspaceFS* pWorkspace, const char* name, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{
		m_pWorkspace = pWorkspace;
		m_path = pWorkspace->GetPath();
		m_start_level = start_level;
		m_end_level = end_level;
		m_extent = extent;

		SetName(name);

		CreateLevels(m_start_level, m_end_level);

		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetBoundingBox(GEnvelope& viewer, g_uint level, g_uint& r_min, g_uint& r_max, g_uint& c_min, g_uint& c_max)
	{
		return AG_SUCCESS;
	}

	g_uint PGISTileStore::GetOriginalLevel(GEnvelope& viewer, g_uint viewer_w, g_uint viewer_h)
	{
		return 0;
	}
}