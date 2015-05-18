#include "WorkspaceTileMongo.h"
//#include "GoogleCRS84QuadTileStore.h"
//#include "PGISTileStore.h"

#include <math.h>

namespace auge
{
	TileWorkspaceMongo::TileWorkspaceMongo()
	{
		m_mongo = NULL;
		m_mongo_db = NULL;

		//m_tile_type = augeTileGoogleCRS84Quad;
		//m_start_level = 1;
		//m_end_level = 18;
		//m_tile_format = "png";
		//m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		//m_extent = m_full_extent;
	}

	TileWorkspaceMongo::~TileWorkspaceMongo()
	{
		
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
		return NULL;
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
		return true;
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
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE TileWorkspaceMongo::CreateTileStore(const char* name, augeTileType type)
	{

		return AG_SUCCESS;
	}

	RESULTCODE	TileWorkspaceMongo::CreateTileStore(const char* name, augeTileType type, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{
		/*switch(type)
		{
		case augeTileGoogleCRS84Quad:
			{
				GoogleCRS84QuadTileStore *pTileStore = new GoogleCRS84QuadTileStore();
				pTileStore->Create(this,name, start_level, end_level, extent);
				m_pTileStore = pTileStore;
			}
			break;
		case augeTilePGIS:
			{
				PGISTileStore* pTileStore = new PGISTileStore();
				pTileStore->Create(this,name, start_level, end_level, extent);
				m_pTileStore = pTileStore;
			}
			break;
		}*/
		return AG_SUCCESS;
	}
	
	TileStore* TileWorkspaceMongo::OpenTileStore(const char* name)
	{	
		if(name==NULL)
		{
			return NULL;
		}

		mongoc_collection_t *mgo_collection = mongoc_client_get_collection(m_mongo, m_database.c_str(), name);
		if(mgo_collection==NULL)
		{
			return NULL;
		}

		return NULL;
	}
	
	RESULTCODE TileWorkspaceMongo::RemoveTileStore()
	{
		return AG_SUCCESS;
	}

}