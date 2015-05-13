#ifndef __AUGE_TILE_STORE_FILE_SYSTEM_H__
#define __AUGE_TILE_STORE_FILE_SYSTEM_H__

#include "AugeTile.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class TileWorkspaceFS : virtual public TileWorkspace
	{
	public:
		TileWorkspaceFS();
		virtual ~TileWorkspaceFS();
	public:
		//========================================================================
		// Base Class
		//========================================================================
		virtual const char*		GetName();
		virtual void			SetName(const char* name);

		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();
		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		//virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);
		virtual RESULTCODE		RemoveDataSet(const char* name);

		virtual	DataEngine*		GetEngine();

		//========================================================================
		// TileWorkspaceFS Class
		//========================================================================
		virtual const char*		CreateTileStore(const char* name, augeTileType type);
		virtual TileStore*		GetTileStore(const char* name);
		virtual RESULTCODE		RemoveTileStore();
		//------------------------------------------------------------------------
		virtual GEnvelope&		GetExtent();
		virtual GEnvelope&		GetFullExtent();
		virtual g_uint			GetRows(g_uint level);
		virtual g_uint			GetCols(g_uint level);

		virtual augeTileType	GetTileType();
		virtual	Tile*			GetTile(g_uint level, g_uint row, g_uint col);
		virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, const char* path);
		virtual RESULTCODE		PutTile(g_uint level, g_uint row, g_uint col, unsigned char* data, size_t size);

		virtual RESULTCODE		GetKey(char* key, size_t size, g_uint level, g_uint row, g_uint col);
		virtual RESULTCODE		GetExtent(GEnvelope& extent, g_uint level, g_uint row, g_uint col);
		virtual RESULTCODE		GetTilePath(char* key, size_t size, g_uint level, g_uint row, g_uint col);

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
	};

}


#endif //__AUGE_TILE_STORE_FILE_SYSTEM_H__
