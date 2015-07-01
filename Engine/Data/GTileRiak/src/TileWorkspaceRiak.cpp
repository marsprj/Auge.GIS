#include "TileWorkspaceRiak.h"
//#include "GoogleCRS84QuadTileStore.h"
//#include "PGISTileStore.h"

#include <math.h>

namespace auge
{
	void *radi_riack_alloc(void *optional_data, size_t size)
	{
		return malloc(size);
	}

	void radi_riack_free(void *optional_data, void *pointer)
	{
		if (pointer)
		{
			free(pointer);
		}
	}

	TileWorkspaceRiak::TileWorkspaceRiak()
	{
		m_riak = NULL;

		m_riak_allocator.alloc = radi_riack_alloc;
		m_riak_allocator.free = radi_riack_free;

		//m_tile_type = augeTileGoogleCRS84Quad;
		//m_start_level = 1;
		//m_end_level = 18;
		//m_tile_format = "png";
		//m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		//m_extent = m_full_extent;
	}

	TileWorkspaceRiak::~TileWorkspaceRiak()
	{
		if (m_riak != NULL)
		{
			riack_disconnect(m_riak);
			riack_free(m_riak);
			m_riak = NULL;
		}
	}

	const char*	TileWorkspaceRiak::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void TileWorkspaceRiak::SetName(const char* name)
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

	RESULTCODE TileWorkspaceRiak::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}
		m_connstring = conn_string;
		m_props.Parse(conn_string);		
		return AG_SUCCESS;
	}

	const char*	TileWorkspaceRiak::GetConnectionString()
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceRiak::Open()
	{
		const char* szServer = "192.168.111.172";
		const char* szInstance = "8087";

		//const char* szServer = m_props.GetValue(AUGE_DB_SERVER);
		//const char* szInstance = m_props.GetValue(AUGE_DB_INSTANCE);
		//const char* szDatabase = m_props.GetValue(AUGE_DB_DATABASE);
		//const char* szUser = m_props.GetValue(AUGE_DB_USER);
		//const char* szPassword = m_props.GetValue(AUGE_DB_PASSWORD);
		//const char* szEncoding = m_props.GetValue(AUGE_DB_ENCODING);

		if (szServer == NULL || szInstance == NULL /*|| szUser == NULL || szPassword == NULL*/)
		{
			return AG_FAILURE;
		}

		if (m_riak != NULL)
		{
			return AG_FAILURE;
		}
		m_riak = riack_new_client(&m_riak_allocator);
		int ret = riack_connect(m_riak, szServer, atoi(szInstance), NULL);
		if (ret != RIACK_SUCCESS)
		{
			riack_free(m_riak);
			m_riak = NULL;
			return AG_FAILURE;
		}
		return AG_SUCCESS;
	}

	void TileWorkspaceRiak::Close()
	{
	}

	bool TileWorkspaceRiak::IsOpen()
	{
		return true;
	}

	EnumDataSet* TileWorkspaceRiak::GetDataSets()
	{
		return NULL;
	}

	DataSet* TileWorkspaceRiak::OpenDataSet(const char* name)
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceRiak::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
	}

	DataEngine*	TileWorkspaceRiak::GetEngine()
	{
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE TileWorkspaceRiak::CreateTileStore(const char* name, augeTileType type)
	{

		return AG_SUCCESS;
	}

	RESULTCODE	TileWorkspaceRiak::CreateTileStore(const char* name, augeTileType type, g_uint start_level, g_uint end_level, GEnvelope& extent)
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
	
	TileStore* TileWorkspaceRiak::OpenTileStore(const char* name)
	{	
		if(name==NULL)
		{
			return NULL;
		}

		return NULL;
	}
	
	RESULTCODE TileWorkspaceRiak::RemoveTileStore()
	{
		return AG_SUCCESS;
	}

	EnumTileStore* TileWorkspaceRiak::GetTileStores()
	{
		return NULL;
	}
}