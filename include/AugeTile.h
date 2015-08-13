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

	#define AUGE_TILE_TYPE_GOOGLE_CRS84QUAD		"GoogleCRS84Quad"
	#define AUGE_TILE_TYPE_GOOGLE_CRS84SCALE	"GlobalCRS84Scale"
	#define AUGE_TILE_TYPE_EPSG4326				"EPSG4326"
	#define AUGE_TILE_TYPE_EPSG900913			"EPSG900913"
	#define AUGE_TILE_TYPE_GLOBAL_CRS84PIXEL	"GlobalCRS84Pixel"
	#define AUGE_TILE_TYPE_PGIS					"PGIS"

	#define	AUGE_TILE_SIZE						256
	#define AUGE_TILE_BLOCK_SIZE				4096

	class Tile : public GObject
	{
	protected:
		Tile(){}
		virtual ~Tile(){}
	public:
		virtual const char*		GetKey()  = 0;
		virtual	g_uchar*		GetData() = 0;
		virtual g_uint			GetSize() = 0;
	};

	class TileBlock : public GObject
	{
	protected:
		TileBlock(){}
		virtual ~TileBlock(){}
	public:
		virtual g_uint			GetLevel() = 0;
		virtual	g_uint64		GetBlockRow() = 0;
		virtual	g_uint64		GetBlockCol() = 0;

		virtual g_uint64		GetRow() = 0;
		virtual g_uint64		GetCol() = 0;

		virtual g_uint64		GetRows() = 0;
		virtual g_uint64		GetCols() = 0;

		virtual GEnvelope&		GetExtent() = 0;
		virtual RESULTCODE		GetTileExtent(GEnvelope& extent, g_uint level, g_uint64 row, g_uint64 col) = 0;

		virtual void			Release() = 0;
	};

	class TileStore : public GObject
	{
	protected:
		TileStore(){}
		virtual ~TileStore(){}
	public:
		virtual const char*		GetName() = 0;

		virtual GEnvelope&		GetExtent() = 0;
		virtual GEnvelope&		GetFullExtent() = 0;
		virtual void			GetTopLeftCorner(double &x, double &y) = 0;

		virtual g_uint64		GetRows(g_uint level) = 0;
		virtual g_uint64		GetCols(g_uint level) = 0;

		virtual g_uint64		GetBlockRows(g_uint level) = 0;
		virtual g_uint64		GetBlockCols(g_uint level) = 0;
		virtual TileBlock*		GetBlock(g_uint level, g_uint64 row, g_uint64 col) = 0;

		virtual augeTileType	GetTileType() = 0;
		virtual const char*		GetTileTypeAsString() = 0;
		virtual	Tile*			GetTile(g_uint level, g_uint64 row, g_uint64 col) = 0;
		virtual RESULTCODE		PutTile(g_uint level, g_uint64 row, g_uint64 col, const char* path) = 0;
		virtual RESULTCODE		PutTile(g_uint level, g_uint64 row, g_uint64 col, g_uchar* data, size_t size) = 0;

		

		virtual RESULTCODE		GetKey(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col) = 0;
		virtual RESULTCODE		GetTileExtent(GEnvelope& extent, g_uint level, g_uint64 row, g_uint64 col) = 0;
		virtual RESULTCODE		GetTilePath(char* key, size_t size, g_uint level, g_uint64 row, g_uint64 col) = 0;

		virtual RESULTCODE		GetBoundingBox(GEnvelope& viewer, g_uint level, g_uint& r_min, g_uint& r_max, g_uint& c_min, g_uint& c_max) = 0;
		virtual g_uint			GetOriginalLevel(GEnvelope& viewer, g_uint viewer_w, g_uint viewer_h) = 0;

	};

	class EnumTileStore
	{
	protected:
		EnumTileStore(){}
		virtual ~EnumTileStore(){}
	public:
		virtual	void			Reset() = 0;
		virtual	TileStore*		Next() = 0;
		virtual void			Release() = 0;
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
		virtual RESULTCODE		RemoveTileStore(const char* name) = 0;

		virtual EnumTileStore*	GetTileStores() = 0;

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