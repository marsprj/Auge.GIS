#ifndef __AUGE_TILE_H__
#define __AUGE_TILE_H__

#include "AugeData.h"

namespace auge
{
	typedef enum 
	{
		augeTileGoogleCRS84Quad	= 0,
		augeTileGlobalCRS84Scale,
		augeTileEPSG4326,
		augeTileEPSG900913,
		augeTileGlobalCRS84Pixel,
		augeTilePGIS
	}augeTileType;

	class Tile : public GObject
	{
	protected:
		Tile(){}
		virtual ~Tile(){}
	};

	class TileStore : public GObject
	{
	protected:
		TileStore(){}
		virtual ~TileStore(){}
	public:
		virtual GEnvelope&		GetExtent() = 0;
		virtual GEnvelope&		GetFullExtent() = 0;
		virtual void			GetTopLeftCorner(double &x, double &y) = 0;

		virtual g_uint			GetRows(g_uint level) = 0;
		virtual g_uint			GetCols(g_uint level) = 0;

		virtual augeTileType	GetTileType() = 0;
		virtual	Tile*			GetTile(g_uint level, g_uint row, g_uint col) = 0;
		virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, const char* path) = 0;
		virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size) = 0;

		virtual RESULTCODE		GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col) = 0;
		virtual RESULTCODE		GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col) = 0;
		virtual RESULTCODE		GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col) = 0;
	};

	class TileWorkspace : virtual public Workspace
	{
	protected:
		TileWorkspace(){}
		virtual ~TileWorkspace(){}
	public:
		virtual TileStore*		OpenTileStore(const char* name) = 0;
		virtual RESULTCODE		CreateTileStore(const char* name, augeTileType type) = 0;
		virtual RESULTCODE		CreateTileStore(const char* name, augeTileType type, g_uint start_level, g_uint end_level, GEnvelope& extent) = 0;
		virtual RESULTCODE		RemoveTileStore() = 0;

		//virtual GEnvelope&		GetExtent() = 0;
		//virtual GEnvelope&		GetFullExtent() = 0;
		//virtual g_uint			GetRows(g_uint level) = 0;
		//virtual g_uint			GetCols(g_uint level) = 0;

		//virtual augeTileType	GetTileType() = 0;
		//virtual	Tile*			GetTile(g_uint level, g_uint row, g_uint col) = 0;
		//virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, const char* path) = 0;
		//virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size) = 0;

		//virtual RESULTCODE		GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col) = 0;
		//virtual RESULTCODE		GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col) = 0;
		//virtual RESULTCODE		GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col) = 0;
	};
}

#endif //__AUGE_TILE_H__