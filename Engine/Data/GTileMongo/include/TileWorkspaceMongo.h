#ifndef __AUGE_TILE_STORE_FILE_SYSTEM_H__
#define __AUGE_TILE_STORE_FILE_SYSTEM_H__

#include <bson.h>
#include <mongoc.h>

#include "AugeTile.h"
#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class TileWorkspaceMongo : virtual public TileWorkspace
	{
		friend class GoogleCRS84QuadTileStore;

	public:
		TileWorkspaceMongo();
		virtual ~TileWorkspaceMongo();
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
		virtual GConnection*	GetConnection();

		virtual EnumDataSet*	GetDataSets();
		virtual DataSet*		OpenDataSet(const char* name);
		virtual RESULTCODE		RemoveDataSet(const char* name);

		virtual	DataEngine*		GetEngine();

		//========================================================================
		// TileWorkspaceFS Class
		//========================================================================
		virtual RESULTCODE		CreateTileStore(const char* name, augeTileType type);
		virtual RESULTCODE		CreateTileStore(const char* name, augeTileType type, g_uint start_level, g_uint end_level, GEnvelope& extent);
		virtual TileStore*		OpenTileStore(const char* name);
		virtual RESULTCODE		RemoveTileStore();

		virtual EnumTileStore*	GetTileStores();

	private:
		bool					FindTileStore(const char* name);
		RESULTCODE				InsertTileStoreMeta(const char* name, const char* type, g_uint start_level, g_uint end_level, GEnvelope& extent);
		
	private:

		mongoc_client_t*	 m_mongo;
		mongoc_database_t*	 m_mongo_db;
		mongoc_collection_t* m_mongo_meta;

		std::string			m_name;
		std::string			m_database;
		std::string			m_connstring;
		GPropertySet		m_props;
	};

}

#endif //__AUGE_TILE_STORE_FILE_SYSTEM_H__

//////////////////////////////////////////////////////////////////////////
// SERVER=127.0.0.1;INSTANCE=27017;DATABASE=tfs;USER=user;PASSWORD=qwer1234
//////////////////////////////////////////////////////////////////////////