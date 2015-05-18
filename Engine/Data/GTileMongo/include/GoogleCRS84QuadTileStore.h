#ifndef __AUGE_GOOGLE_CRS84_QUAD_TILE_STORE_H__
#define __AUGE_GOOGLE_CRS84_QUAD_TILE_STORE_H__

#include <bson.h>
#include <mongoc.h>

#include "AugeTile.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class TileWorkspaceMongo;

	class GoogleCRS84QuadTileStore : public TileStore
	{
	public:
		GoogleCRS84QuadTileStore();
		virtual ~GoogleCRS84QuadTileStore();
	public:
		virtual const char*		GetName();
		virtual void			SetName(const char* name);

		virtual GEnvelope&		GetExtent() ;
		virtual GEnvelope&		GetFullExtent() ;
		virtual void			GetTopLeftCorner(double &x, double &y);

		virtual g_uint			GetRows(g_uint level) ;
		virtual g_uint			GetCols(g_uint level) ;

		virtual augeTileType	GetTileType() ;
		virtual	Tile*			GetTile(g_uint level, g_uint row, g_uint col) ;
		virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, const char* path) ;
		virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size) ;

		virtual RESULTCODE		GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col) ;
		virtual RESULTCODE		GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col) ;
		virtual RESULTCODE		GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col) ;

	public:
		RESULTCODE		Create(TileWorkspaceMongo* pWorkspace, mongoc_collection_t *mgo_collection);
		RESULTCODE		Create(TileWorkspaceMongo* pWorkspace, const char* name, g_uint start_level, g_uint end_level, GEnvelope& extent);

	private:
		RESULTCODE		CreateLevels(g_uint start, g_uint end);

	private:
		std::string		m_name;
		std::string		m_connstring;
		std::string		m_tile_format;
		augeTileType	m_tile_type;

		GEnvelope		m_extent;
		GEnvelope		m_full_extent;

		g_uint			m_start_level;
		g_uint			m_end_level;

		TileWorkspaceMongo*  m_pWorkspace;
		mongoc_collection_t* m_collection;
	};
}

#endif //__AUGE_GOOGLE_CRS84_QUAD_TILE_STORE_H__
