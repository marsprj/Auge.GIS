#include "PGISTileStore.h"
#include <math.h>

namespace auge
{
	PGISTileStore::PGISTileStore()
	{
		m_tile_type = augeTileGoogleCRS84Quad;
		m_start_level = 1;
		m_end_level = 18;
		m_tile_format = "png";
		m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		m_extent = m_full_extent;
	}

	PGISTileStore::~PGISTileStore()
	{

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

	}

	g_uint PGISTileStore::GetRows(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
	}
	
	g_uint PGISTileStore::GetCols(g_uint level)
	{
		float l = level > 20 ? 20 : level;
		return pow(2, l);
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

	Tile* PGISTileStore::GetTile(g_uint level, g_uint row, g_uint col)
	{
		return NULL;
	}

	RESULTCODE PGISTileStore::PutTile(g_uint level, g_uint row, g_uint col, const char* path)
	{
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size)
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
	RESULTCODE PGISTileStore::GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col)
	{
		double resolution = m_full_extent.GetHeight() / pow(2.0f,(float)(level-1));
		double xmin = m_full_extent.m_xmin + resolution * col;
		double ymax = m_full_extent.m_ymax - resolution * row;
		double xmax = xmin + resolution;
		double ymin = ymax - resolution;
		extent.Set(xmin,ymin,xmax,ymax);
		return AG_SUCCESS;
	}

	RESULTCODE PGISTileStore::GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col)
	{	
		g_snprintf(key, size, "%s/%d/%d_%d.%s",m_path.c_str(), level, row, col,m_tile_format.c_str());
		return AG_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE	PGISTileStore::CreateLevels(g_uint start, g_uint end)
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