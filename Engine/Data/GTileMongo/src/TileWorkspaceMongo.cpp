#include "TileWorkspaceMongo.h"
#include "EnumTileStoreImpl.h"
#include "GoogleCRS84QuadTileStore.h"
#include "PGISTileStore.h"

#include <math.h>

namespace auge
{
	#define AUGE_TILE_STORE_META_COLLECTION	"tile_meta"
	//--------------------------------------------------------
	//{"name":"store1","type":"GoogleCRS84Quad"}
	//{"name":"pgis","type":"PGIS"}
	//--------------------------------------------------------

	TileWorkspaceMongo::TileWorkspaceMongo()
	{
		m_mongo = NULL;
		m_mongo_db = NULL;
		m_mongo_meta = NULL;

		//m_tile_type = augeTileGoogleCRS84Quad;
		//m_start_level = 1;
		//m_end_level = 18;
		//m_tile_format = "png";
		//m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		//m_extent = m_full_extent;
	}

	TileWorkspaceMongo::~TileWorkspaceMongo()
	{
		Close();
	}

	const char*	TileWorkspaceMongo::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void TileWorkspaceMongo::SetName(const char* name)
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

	RESULTCODE TileWorkspaceMongo::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}
		m_connstring = conn_string;
		m_props.Parse(conn_string);		
		return AG_SUCCESS;
	}

	const char*	TileWorkspaceMongo::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE TileWorkspaceMongo::Open()
	{
		if(m_mongo!=NULL)
		{
			return AG_FAILURE;
		}

		const char* szServer   = m_props.GetValue(AUGE_DB_SERVER);
		const char* szInstance = m_props.GetValue(AUGE_DB_INSTANCE);
		const char* szDatabase = m_props.GetValue(AUGE_DB_DATABASE);
		const char* szUser     = m_props.GetValue(AUGE_DB_USER);
		const char* szPassword = m_props.GetValue(AUGE_DB_PASSWORD);
		const char* szEncoding = m_props.GetValue(AUGE_DB_ENCODING);

		if(szServer==NULL||szDatabase==NULL||szUser==NULL||szPassword==NULL)
		{
			return AG_FAILURE;
		}

		char constr[AUGE_NAME_MAX] = {0};
		g_sprintf(constr, "mongodb://%s:%s/", szServer, szInstance);
		m_mongo = mongoc_client_new(constr);
		if(m_mongo == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "Cannot to connect to mongdo %s:%s", szServer, szInstance);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);

			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return AG_FAILURE;
		}

		m_mongo_db = mongoc_client_get_database(m_mongo, szDatabase);
		m_mongo_meta = mongoc_client_get_collection(m_mongo, szDatabase, AUGE_TILE_STORE_META_COLLECTION);

		m_database = szDatabase;

		return AG_SUCCESS;
	}

	void TileWorkspaceMongo::Close()
	{
		if(m_mongo!=NULL)
		{
			mongoc_client_destroy(m_mongo);
			m_mongo = NULL;
		}
	}

	bool TileWorkspaceMongo::IsOpen()
	{
		return m_mongo;
	}

	GConnection* TileWorkspaceMongo::GetConnection()
	{
		return NULL;
	}

	EnumDataSet* TileWorkspaceMongo::GetDataSets()
	{
		return NULL;
	}

	DataSet* TileWorkspaceMongo::OpenDataSet(const char* name)
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceMongo::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
	}

	DataEngine*	TileWorkspaceMongo::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE TileWorkspaceMongo::CreateTileStore(const char* name, augeTileType type)
	{
		GEnvelope extent(-180,-90,180,90);
		return CreateTileStore(name, type, 1, 18, extent);
	}

	RESULTCODE	TileWorkspaceMongo::CreateTileStore(const char* name, augeTileType type, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{
		const char* store_type = NULL;
		switch(type)
		{
		case augeTileGoogleCRS84Quad:
			store_type = "GoogleCRS84Quad";
			break;
		case augeTilePGIS:
			store_type = "PGIS";
			break;
		}
		if(store_type==NULL)
		{
			return AG_FAILURE;
		}

		if(FindTileStore(name))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "TileStore [%s] existed.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return AG_FAILURE;
		}

		return InsertTileStoreMeta(name, store_type, start_level, end_level, extent);
	}
	
	TileStore* TileWorkspaceMongo::OpenTileStore(const char* name)
	{	
		if(name==NULL)
		{
			return NULL;
		}

		char type[AUGE_NAME_MAX];
		memset(type, 0, AUGE_NAME_MAX);

		bson_t m_query;
		bson_init (&m_query);
		bson_append_utf8 (&m_query, "name", -1, name, -1);

		mongoc_cursor_t *m_cursor = NULL;
		m_cursor = mongoc_collection_find ( m_mongo_meta,
											MONGOC_QUERY_NONE,
											0,
											0,
											0,
											&m_query,
											NULL,  /* Fields, NULL for all. */
											NULL); /* Read Prefs, NULL for default */

		const bson_t *bson = NULL;
		if(!mongoc_cursor_next(m_cursor, &bson))
		{
			mongoc_cursor_destroy (m_cursor);
			bson_destroy (&m_query);
			return NULL;
		}
			
		uint32_t length = 0;
		const char* value = NULL;
		bson_iter_t bson_iter;
		bson_iter_init(&bson_iter, bson);

		bson_iter_find(&bson_iter, "type");
		value = bson_iter_utf8(&bson_iter, &length);
		memcpy(type, value, length);
		
		mongoc_cursor_destroy (m_cursor);
		bson_destroy (&m_query);

		//Create TileStore Object
		bson_error_t error;
		mongoc_gridfs_t* mgo_gridfs = mongoc_client_get_gridfs(m_mongo, m_database.c_str(), name,&error);
		if(mgo_gridfs==NULL)
		{
			return NULL;
		}
		TileStore* pTileStore = NULL;
		if(g_stricmp(type, "PGIS")==0)
		{
			PGISTileStore* pTileStorePGIS = new PGISTileStore();
			pTileStorePGIS->Create(this, mgo_gridfs, name);
			pTileStore = pTileStorePGIS;
		}
		else if(g_stricmp(type, "GoogleCRS84Quad")==0)
		{
			GoogleCRS84QuadTileStore *pTileStoreGoogle = new GoogleCRS84QuadTileStore();
			pTileStoreGoogle->Create(this, mgo_gridfs, name);
			pTileStore = pTileStoreGoogle;
		}		
		
		return pTileStore;
	}
	
	RESULTCODE TileWorkspaceMongo::RemoveTileStore(const char* name)
	{
		return AG_SUCCESS;
	}

	EnumTileStore* TileWorkspaceMongo::GetTileStores()
	{
		char name[AUGE_NAME_MAX];		
		char type[AUGE_NAME_MAX];
		
		bson_t query;
		bson_init (&query);

		mongoc_cursor_t *m_cursor = NULL;
		m_cursor = mongoc_collection_find ( m_mongo_meta,
											MONGOC_QUERY_NONE,
											0,
											0,
											0,
											&query,
											NULL,  /* Fields, NULL for all. */
											NULL); /* Read Prefs, NULL for default */

		uint32_t length = 0;
		const char* value = NULL;

		bson_iter_t bson_iter;
		const bson_t *bson = NULL;

		bson_error_t error;
		mongoc_gridfs_t* mgo_gridfs = NULL;

		TileStore* pTileStore = NULL;
		EnumTileStoreImpl* pEnumTileStore = new EnumTileStoreImpl();
		
		while(mongoc_cursor_next(m_cursor, &bson))
		{	
			bson_iter_init(&bson_iter, bson);

			bson_iter_find(&bson_iter, "name");
			value = bson_iter_utf8(&bson_iter, &length);
			memset(name, 0, AUGE_NAME_MAX);
			memcpy(name, value, length);

			bson_iter_find(&bson_iter, "type");
			value = bson_iter_utf8(&bson_iter, &length);
			memset(type, 0, AUGE_NAME_MAX);
			memcpy(type, value, length);

			//memset(name, 0, AUGE_NAME_MAX);
			mgo_gridfs = mongoc_client_get_gridfs(m_mongo, m_database.c_str(), name,&error);
			if(mgo_gridfs!=NULL) 
			{
				if(g_stricmp(type, "GoogleCRS84Quad")==0)
				{
					GoogleCRS84QuadTileStore *pTileStoreGoogle = new GoogleCRS84QuadTileStore();
					pTileStoreGoogle->Create(this, mgo_gridfs, name);
					pTileStore = pTileStoreGoogle;
				}
				else if(g_stricmp(type, "PGIS")==0)
				{
					PGISTileStore* pTileStorePGIS = new PGISTileStore();
					pTileStorePGIS->Create(this, mgo_gridfs, name);
					pTileStore = pTileStorePGIS;
				}

				pEnumTileStore->Add(pTileStore);
			}
		}

		mongoc_cursor_destroy (m_cursor);
		bson_destroy (&query);

		return pEnumTileStore;
	}

	bool TileWorkspaceMongo::FindTileStore(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		bson_error_t error;
		bson_t query;
		bson_init (&query);
		bson_append_utf8 (&query, "name", -1, name, -1);
		/*
		int64_t                       mongoc_collection_count                (
		mongoc_collection_t           *collection,
		mongoc_query_flags_t           flags,
		const bson_t                  *query,
		int64_t                        skip,
		int64_t                        limit,
		const mongoc_read_prefs_t     *read_prefs,
		bson_error_t                  *error);
		*/
		int64_t count = mongoc_collection_count (m_mongo_meta, MONGOC_QUERY_NONE, &query, 0, 0, NULL, &error);
		//int64_t count = mongoc_collection_count (m_mongo_meta, MONGOC_QUERY_NONE, query, 0, 0, NULL, NULL, &error);

		//mongoc_cursor_t *m_cursor = NULL;
		//m_cursor = mongoc_collection_find (m_mongo_meta,
		//									MONGOC_QUERY_NONE,
		//									0,
		//									0,
		//									0,
		//									&m_query,
		//									NULL,  /* Fields, NULL for all. */
		//									NULL); /* Read Prefs, NULL for default */

		//const bson_t *m_doc = NULL;
		//if(!mongoc_cursor_next(m_cursor, &m_doc))
		//{
		//	mongoc_cursor_destroy (m_cursor);
		//	bson_destroy (&m_query);
		//	return false;
		//}

		//size_t length = 0;
		//const char* str = bson_as_json(m_doc,&length);

		//mongoc_cursor_destroy (m_cursor);
		bson_destroy (&query);

		return (count>0);
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE TileWorkspaceMongo::InsertTileStoreMeta(const char* name, const char* type, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{

		bson_t *b_doc = NULL;
		bson_oid_t b_oid;
		bson_error_t b_error;
		RESULTCODE rc = AG_SUCCESS;

		b_doc = bson_new();
		bson_oid_init(&b_oid, NULL);
		BSON_APPEND_OID(b_doc, "_oid", &b_oid);
		BSON_APPEND_UTF8(b_doc, "name", name);
		BSON_APPEND_UTF8(b_doc, "type", type);
		BSON_APPEND_INT32(b_doc, "level_min", start_level);
		BSON_APPEND_INT32(b_doc, "level_max", end_level);

		if(!mongoc_collection_insert(m_mongo_meta, MONGOC_INSERT_NONE, b_doc, NULL, &b_error))
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError(b_error.message);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(b_error.message);

			rc = AG_FAILURE;
		}

		bson_destroy(b_doc);

		return AG_SUCCESS;
	}
}
