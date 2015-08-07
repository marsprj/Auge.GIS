#include "WorkspaceTFS.h"
#include "GoogleCRS84QuadTileStore.h"
#include "PGISTileStore.h"
#include "EnumTileStoreImpl.h"

#include <math.h>

namespace auge
{
	TileWorkspaceFS::TileWorkspaceFS()
	{
		m_pTileStore = NULL;

		//m_tile_type = augeTileGoogleCRS84Quad;
		//m_start_level = 1;
		//m_end_level = 18;
		//m_tile_format = "png";
		//m_full_extent.Set(-180.0f,-90.0f,180.0f,90.0f);
		//m_extent = m_full_extent;
	}

	TileWorkspaceFS::~TileWorkspaceFS()
	{

	}

	const char*	TileWorkspaceFS::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void TileWorkspaceFS::SetName(const char* name)
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

	RESULTCODE TileWorkspaceFS::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		m_connstring = conn_string;
		size_t epos = m_connstring.find_first_of("=");
		if(epos==std::string::npos)
		{
			m_connstring.clear();
			return AG_FAILURE;
		}

		m_path = m_connstring.substr(epos+1, m_connstring.length()-epos-1);
		return AG_SUCCESS;
	}

	const char*	TileWorkspaceFS::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE TileWorkspaceFS::Open()
	{
		GError* pError = augeGetErrorInstance();

		if(m_path.empty())
		{
			const char* msg = "Path is NULL";
			pError->SetError(msg);
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_SUCCESS;
		if(g_access(m_path.c_str(), 4))
		{
			rc = auge_mkdir(m_path.c_str());
			if(rc!=AG_SUCCESS)
			{
				char msg[AUGE_MSG_MAX] = {0};
				g_sprintf(msg, "Cannot Create Folder [%s]", m_path.c_str());
				pError->SetError(msg);
			}
		}
		
		return AG_SUCCESS;
	}

	void TileWorkspaceFS::Close()
	{

	}

	bool TileWorkspaceFS::IsOpen()
	{
		return true;
	}

	GConnection* TileWorkspaceFS::GetConnection()
	{
		return NULL;
	}

	EnumDataSet* TileWorkspaceFS::GetDataSets()
	{
		return NULL;
	}

	DataSet* TileWorkspaceFS::OpenDataSet(const char* name)
	{
		return NULL;
	}

	RESULTCODE TileWorkspaceFS::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
	}

	DataEngine*	TileWorkspaceFS::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	//////////////////////////////////////////////////////////////////////////
	RESULTCODE TileWorkspaceFS::CreateTileStore(const char* name, augeTileType type)
	{
		if(m_pTileStore!=NULL)
		{
			return AG_FAILURE;
		}

		switch(type)
		{
		case augeTileGoogleCRS84Quad:
			{
				GoogleCRS84QuadTileStore *pTileStore = new GoogleCRS84QuadTileStore();
				pTileStore->Create(this);
				m_pTileStore = pTileStore;
			}
			break;
		case augeTilePGIS:
			{
				PGISTileStore* pTileStore = new PGISTileStore();
				pTileStore->Create(this);
				m_pTileStore = pTileStore;
			}
			break;
		}
		return AG_SUCCESS;
	}

	RESULTCODE	TileWorkspaceFS::CreateTileStore(const char* name, augeTileType type, g_uint start_level, g_uint end_level, GEnvelope& extent)
	{
		if(m_pTileStore!=NULL)
		{
			return AG_FAILURE;
		}

		switch(type)
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
		}
		return AG_SUCCESS;
	}
	
	TileStore* TileWorkspaceFS::OpenTileStore(const char* name)
	{
		if(m_pTileStore==NULL)
		{
			//GoogleCRS84QuadTileStore *pTileStore = new GoogleCRS84QuadTileStore();
			//pTileStore->Create(this);

			PGISTileStore* pTileStore = new PGISTileStore();
			pTileStore->Create(this);

			m_pTileStore = pTileStore;
		}
		if(m_pTileStore!=NULL)
		{
			m_pTileStore->AddRef();
		}
		return m_pTileStore;
	}
	
	RESULTCODE TileWorkspaceFS::RemoveTileStore()
	{
		return AG_SUCCESS;
	}

	EnumTileStore* TileWorkspaceFS::GetTileStores()
	{
		EnumTileStoreImpl* pEnumStore = new EnumTileStoreImpl();
		TileStore* pTileStore = OpenTileStore(NULL);
		if(pTileStore!=NULL)
		{
			pEnumStore->Add(pTileStore);
		}
		return pEnumStore;
	}

	const char*	TileWorkspaceFS::GetPath()
	{
		return m_path.c_str();
	}
}