#ifndef __AUGE_GOOGLE_CRS84_QUAD_TILE_STORE_H__
#define __AUGE_GOOGLE_CRS84_QUAD_TILE_STORE_H__

#include "AugeTile.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class TileWorkspaceFS;

	class GoogleCRS84QuadTileStore : public TileStore
	{
	public:
		GoogleCRS84QuadTileStore();
		virtual ~GoogleCRS84QuadTileStore();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);

		virtual GEnvelope&		GetExtent();
		virtual GEnvelope&		GetFullExtent();
		virtual void			GetTopLeftCorner(double &x, double &y);

		virtual g_uint			GetRows(g_uint level);
		virtual g_uint			GetCols(g_uint level);

		virtual augeTileType	GetTileType();
		virtual const char*		GetTileTypeAsString();

		virtual	Tile*			GetTile(g_uint level, g_uint64 row, g_uint64 col);
		virtual RESULTCODE		PutTile(g_uint level, g_uint64 row, g_uint64 col, const char* path);
		virtual RESULTCODE		PutTile(g_uint level, g_uint64 row, g_uint64 col, g_uchar* data, size_t size);

		virtual RESULTCODE		GetKey(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col);
		virtual RESULTCODE		GetTileExtent(GEnvelope& extent, g_uint level, g_uint64 row, g_uint64 col);
		virtual RESULTCODE		GetTilePath(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col);

		virtual RESULTCODE		GetBoundingBox(GEnvelope& viewer, g_uint level, g_uint& r_min, g_uint& r_max, g_uint& c_min, g_uint& c_max);
		virtual g_uint			GetOriginalLevel(GEnvelope& viewer, g_uint viewer_w, g_uint viewer_h);

	public:
		RESULTCODE		Create(TileWorkspaceFS* pWorkspace);
		RESULTCODE		Create(TileWorkspaceFS* pWorkspace, const char* name, g_uint start_level, g_uint end_level, GEnvelope& extent);

	private:
		RESULTCODE		CreateLevels(g_uint start, g_uint end);
		
	private:
		std::string		m_name;
		std::string		m_path;
		std::string		m_connstring;
		std::string		m_tile_format;
		augeTileType	m_tile_type;

		GEnvelope		m_extent;
		GEnvelope		m_full_extent;

		g_uint			m_start_level;
		g_uint			m_end_level;

		TileWorkspaceFS* m_pWorkspace;
	};
}

#endif //__AUGE_GOOGLE_CRS84_QUAD_TILE_STORE_H__
