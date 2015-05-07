#include "ConnectionManagerImpl.h"

namespace auge
{
	ConnectionManager* augeGetConnectionManagerInstance()
	{
		static ConnectionManagerImpl g_connMgr;
		return &g_connMgr;
	}

	const char*	augeGetDataSetType(augeDataSetType type)
	{
		switch(type)
		{
		case augeDataSetFeature:
			return "Feature";
		case augeDataSetRaster:
			return "Raster";
		}
	}

	ConnectionManagerImpl::ConnectionManagerImpl():
	m_pConnection(NULL)
	{

	}

	ConnectionManagerImpl::~ConnectionManagerImpl()
	{
		Unload();
	}

	g_int ConnectionManagerImpl::GetCount()
	{
		return m_connections.size();
	}

	Workspace* ConnectionManagerImpl::GetWorkspace(g_uint i)
	{
		if(i>=m_connections.size())
		{
			return NULL;
		}
		Workspace* pWorkspace = NULL;
		pWorkspace = m_connections[i];
		pWorkspace->AddRef();
		return pWorkspace;
	}

	Workspace* ConnectionManagerImpl::GetWorkspace(const char* name)
	{
		Workspace* pWorkspace = NULL;
		std::vector<Workspace*>::iterator iter;
		for(iter=m_connections.begin(); iter!=m_connections.end(); iter++)
		{
			pWorkspace = *iter;
			if(g_stricmp(pWorkspace->GetName(), name)==0)
			{
				if(!pWorkspace->IsOpen())
				{
					pWorkspace->Open();
					pWorkspace->AddRef();
				}
				return pWorkspace;
			}
		}
		return NULL;
	}

	Workspace* ConnectionManagerImpl::GetWorkspaceById(g_uint id)
	{
		Workspace* pWorkspace = NULL;
		std::vector<Workspace*>::iterator iter;
		for(iter=m_connections.begin(); iter!=m_connections.end(); iter++)
		{
			pWorkspace = *iter;
			if(pWorkspace->GetID()==id)
			{
				if(!pWorkspace->IsOpen())
				{
					pWorkspace->Open();
					pWorkspace->AddRef();
				}
				return pWorkspace;
			}
		}
		return NULL;
	}

	RESULTCODE ConnectionManagerImpl::Register(const char* name, const char* engine, const char* constr)
	{
		if(name==NULL || engine==NULL || constr==NULL)
		{
			return AG_FAILURE;
		}

		Workspace* pWorkspace = NULL;
		pWorkspace = CreateWorkspace(name, engine, constr);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = SaveWorkspace(name, engine, constr);
		if(rc!=AG_SUCCESS)
		{
			pWorkspace->Release();
			return AG_FAILURE;
		}
		m_connections.push_back(pWorkspace);
		return AG_SUCCESS;
	}

	RESULTCODE ConnectionManagerImpl::Unregister(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "delete from g_data_source where name='%s'", name);
		return m_pConnection->ExecuteSQL(sql);
	}

	bool ConnectionManagerImpl::Has(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "select count(*) from g_data_source where name='%s'", name);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if( !pResult )
		{
			return false;
		}
		int count = pResult->GetInt(0,0);
		pResult->Release();

		return count;
	}

	g_int ConnectionManagerImpl::GetID(const char* name)
	{
		if(name==NULL)
		{
			return -1;
		}

		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "select gid from g_data_source where name='%s'", name);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if( !pResult )
		{
			return -1;
		}
		int id = pResult->GetInt(0,0);
		pResult->Release();

		return id;
	}

	RESULTCODE ConnectionManagerImpl::Initialize(GConnection* pConnection)
	{
		if(pConnection==NULL)
		{
			return AG_FAILURE;
		}

		m_pConnection = pConnection;
		if(!m_pConnection->HasTable("g_data_source"))
		{
			if(CreateDataSourceTable())
			{
				// error processing 
			}
		}
		else
		{
			const char* sql = "select gid,name,engine,uri,state,d_uri from g_data_source";
			GResultSet* pResultSet = NULL;
			pResultSet = m_pConnection->ExecuteQuery(sql);
			if(pResultSet!=NULL)
			{
				Workspace* pWorkspace = NULL;
				int gid = -1;
				const char* name= NULL;
				const char* engn=NULL;
				const char* uri = NULL;
				const char* stat= NULL;
				const char* d_uri = NULL;
				g_uint count = pResultSet->GetCount();
				for(g_uint i=0; i<count; i++)
				{
					gid = pResultSet->GetInt(i,0);
					name= pResultSet->GetString(i,1);
					engn= pResultSet->GetString(i,2);
					uri = pResultSet->GetString(i,3);
					stat= pResultSet->GetString(i,4);
					d_uri = pResultSet->GetString(i,5);

					pWorkspace = CreateWorkspace(name, engn, uri);
					if(pWorkspace!=NULL)
					{
						pWorkspace->SetID(gid);
						m_connections.push_back(pWorkspace);
					}
				}
			}
		}	
		return AG_SUCCESS;
	}

	void ConnectionManagerImpl::Unload()
	{
		Workspace* pWorkspace = NULL;
		std::vector<Workspace*>::iterator iter;
		for(iter=m_connections.begin(); iter!=m_connections.end(); iter++)
		{
			pWorkspace = *iter;
			pWorkspace->Close();
			pWorkspace->Release();
		}
		m_connections.clear();
	}

	bool ConnectionManagerImpl::CreateDataSourceTable()
	{
		const char* sql = "CREATE TABLE g_data_source(gid serial NOT NULL,name character varying(32) NOT NULL, d_uri character varying(128) NOT NULL, engine character varying(16) NOT NULL, uri character varying(256) NOT NULL, state character varying(8),CONSTRAINT g_data_source_pkey PRIMARY KEY (gid),CONSTRAINT g_data_source_name_key UNIQUE (name))";
		if(m_pConnection->ExecuteSQL(sql)!=AG_SUCCESS)
		{
			return false;
		}
		return true;
	}

	Workspace* ConnectionManagerImpl::CreateWorkspace(const char* name, const char* engine, const char* uri)
	{
		Workspace* pWorkspace = NULL;

		DataEngine* pEngine = NULL;
		DataEngineManager* pEnginManager = NULL;
		pEnginManager = augeGetDataEngineManagerInstance();
		
		pEngine = pEnginManager->GetEngine(engine);
		if(pEngine==NULL)
		{
			return NULL;
		}

		pWorkspace = pEngine->CreateWorkspace();
		pWorkspace->SetConnectionString(uri);
		pWorkspace->SetName(name); 
		

		return pWorkspace;
	}

	RESULTCODE ConnectionManagerImpl::SaveWorkspace(const char* name, const char* engine, const char* uri)
	{
		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "insert into g_data_source (name, engine, uri) values('%s','%s','%s')", name, engine, uri);
		return m_pConnection->ExecuteSQL(sql);
	}
}
