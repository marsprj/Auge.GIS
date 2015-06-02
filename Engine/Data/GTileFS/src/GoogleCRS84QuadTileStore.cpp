#include "WorkspaceTFS.h"
#include "GoogleCRS84QuadTileStore.h"
#include "TileImpl.h"
#include <math.h>

namespace auge
{
	double scale_denomiators[]={
				5.590822640287178E8,
				2.795411320143589E8,
				1.397705660071794E8,
				6.988528300358972E7,
				3.494264150179486E7,
				1.747132075089743E7,
				8735660.375448715,
				4367830.187724357,
				2183915.093862179,
				1091957.546931089,
				545978.7734655447,
				272989.3867327723,
				136494.6933663862,
				68247.34668319309,
				34123.67334159654,
				17061.83667079827,
				8530.918335399136,
				4265.459167699568, 
				2132.729583849784};

	GoogleCRS84QuadTileStore::GoogleCRS84QuadTileStore()
	{
		m_tile_type = augeTileGoogleCRS84Quad;
		m_start_level = 1;
		m_end_level = 18;
		m_tile_format = "png";
		m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		m_extent = m_full_extent;
		m_pWorkspace = NULL;
	}

	GoogleCRS84QuadTileStore::~GoogleCRS84QuadTileStore()
	{

	}

	GEnvelope& GoogleCRS84QuadTileStore::GetExtent()
	{
		return m_extent;
	}

	GEnvelope& GoogleCRS84QuadTileStore::GetFullExtent()
	{
		return m_full_extent;
	}

	void GoogleCRS84QuadTileStore::GetTopLeftCorner(double &x, double &y)
	{
		x = -180;
		y = -90;
	}

	g_uint64 GoogleCRS84QuadTileStore::GetRows(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l-1);
	}
	
	g_uint64 GoogleCRS84QuadTileStore::GetCols(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}

	g_uint64 GoogleCRS84QuadTileStore::GetBlockRows(g_uint level)
	{
		return GetRows(level) >> (AUGE_TILE_BLOCK_SIZE >> 8);
	}

	g_uint64 GoogleCRS84QuadTileStore::GetBlockCols(g_uint level)
	{
		return 0;
	}

	TileBlock* GoogleCRS84QuadTileStore::GetBlock(g_uint level, g_uint64 row, g_uint64 col)
	{
		return NULL;
	}

	const char*	GoogleCRS84QuadTileStore::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void GoogleCRS84QuadTileStore::SetName(const char* name)
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
	
	augeTileType GoogleCRS84QuadTileStore::GetTileType()
	{
		return m_tile_type;
	}

	const char* GoogleCRS84QuadTileStore::GetTileTypeAsString()
	{
		return AUGE_TILE_TYPE_GOOGLE_CRS84QUAD;
	}

	Tile* GoogleCRS84QuadTileStore::GetTile(g_uint level, g_uint64 row, g_uint64 col)
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

	RESULTCODE GoogleCRS84QuadTileStore::PutTile(g_uint level, g_uint64 row, g_uint64 col, const char* path)
	{
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::PutTile(g_uint level, g_uint64 row, g_uint64 col, unsigned char* data, size_t size)
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
	RESULTCODE GoogleCRS84QuadTileStore::GetKey(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::GetTileExtent(GEnvelope& extent, g_uint level, g_uint64 row, g_uint64 col)
	{
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level-1));
		double xmin = m_full_extent.m_xmin + resolution * col;
		double ymax = m_full_extent.m_ymax - resolution * row;
		double xmax = xmin + resolution;
		double ymin = ymax - resolution;
		extent.Set(xmin,ymin,xmax,ymax);
		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::GetTilePath(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col)
	{	
		g_snprintf(key, size, "%s/%d/%d_%d.%s",m_path.c_str(), level, row, col,m_tile_format.c_str());
		return AG_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE	GoogleCRS84QuadTileStore::CreateLevels(g_uint start, g_uint end)
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

	RESULTCODE GoogleCRS84QuadTileStore::Create(TileWorkspaceFS* pWorkspace)
	{
		m_pWorkspace = pWorkspace;
		m_path = pWorkspace->GetPath();

		CreateLevels(m_start_level, m_end_level);

		return AG_SUCCESS;
	}

	RESULTCODE GoogleCRS84QuadTileStore::Create(TileWorkspaceFS* pWorkspace, const char* name, g_uint start_level, g_uint end_level, GEnvelope& extent)
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

	RESULTCODE GoogleCRS84QuadTileStore::GetBoundingBox(GEnvelope& viewer, g_uint level, g_uint& r_min, g_uint& r_max, g_uint& c_min, g_uint& c_max)
	{
		if(!m_full_extent.Intersects(viewer))
		{
			return AG_FAILURE;
		}

		GEnvelope common = m_full_extent.Intersect(viewer);
	
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level-1));

		c_min = floor((common.m_xmin - m_full_extent.m_xmin) / resolution);
		c_max = ceil ((common.m_xmax - m_full_extent.m_xmin) / resolution);

		r_min = floor((m_full_extent.m_ymax - common.m_ymax) / resolution);
		r_max = ceil ((m_full_extent.m_ymax - common.m_ymin) / resolution);
		
		return AG_SUCCESS;
	}

	g_uint GoogleCRS84QuadTileStore::GetOriginalLevel(GEnvelope& viewer, g_uint viewer_w, g_uint viewer_h)
	{
		g_uint64 p_w = viewer_h * viewer.GetHeight() / 180.f;
		for(g_uint i=0; i<20; i++)
		{
			g_uint v = 1 << i;
			if( v > p_w )
			{
				return i+1;
			}
		}
		return 0;
	}
}