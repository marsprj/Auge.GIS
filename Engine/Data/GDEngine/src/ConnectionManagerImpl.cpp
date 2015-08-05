#include "ConnectionManagerImpl.h"
#include "EnumWorkspaceImpl.h"

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
		case augeDataSetAttribute:
			return "Attribute";
		}
		return "";
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
			if(pWorkspace==NULL)
			{
				continue;
			}

			if(g_stricmp(pWorkspace->GetName(), name)==0)
			{
				if(IsUpdated(pWorkspace))
				{
					Workspace* pnewWorkspace = LoadWorkspace(name);
					pWorkspace->Close();
					pWorkspace->Release();
					pWorkspace = pnewWorkspace;
					*iter = pnewWorkspace;
				}
				if(pWorkspace!=NULL)
				{
					if(!pWorkspace->IsOpen())
					{
						pWorkspace->Open();
						pWorkspace->AddRef();
					}
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
			if(pWorkspace!=NULL)
			{
				if(pWorkspace->GetID()==id)
				{
					if(IsUpdated(pWorkspace))
					{
						Workspace* pnewWorkspace = LoadWorkspace(pWorkspace->GetName());
						pWorkspace->Close();
						pWorkspace->Release();
						pWorkspace = pnewWorkspace;
						*iter = pnewWorkspace;
					}
					if(pWorkspace!=NULL)
					{
						if(!pWorkspace->IsOpen())
						{
							pWorkspace->Open();
							pWorkspace->AddRef();
						}
					}
					return pWorkspace;
				}
			}
		}
		return NULL;
	}

	EnumWorkspace* ConnectionManagerImpl::GetWorkspaces()
	{
		EnumWorkspaceImpl* pWorkspaces = new EnumWorkspaceImpl();
		if(m_pConnection==NULL)
		{
			return pWorkspaces;
		}

		const char* sql = "select gid,name,engine,uri,version,state,d_uri from g_data_source";
		GResultSet* pResultSet = NULL;
		pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet!=NULL)
		{
			Workspace* pWorkspace = NULL;
			int gid = -1;
			int version = 0;
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
				version=pResultSet->GetInt(i,4);
				stat= pResultSet->GetString(i,5);
				d_uri = pResultSet->GetString(i,6);

				pWorkspace = NewWorkspace(gid,name, engn, uri,version);
				if(pWorkspace!=NULL)
				{
					pWorkspaces->Add(pWorkspace);
				}
			}
		}
		return pWorkspaces;
	}

	RESULTCODE ConnectionManagerImpl::Register(const char* name, const char* engine, const char* constr)
	{
		if(name==NULL || engine==NULL || constr==NULL)
		{
			return AG_FAILURE;
		}

		Workspace* pWorkspace = NULL;
		pWorkspace = NewWorkspace(-1, name, engine, constr,1);
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

	RESULTCODE ConnectionManagerImpl::Update(const char* name, const char* engine, const char* constr)
	{
		if(name==NULL || engine==NULL || constr==NULL)
		{
			return AG_FAILURE;
		}

		if(!Has(name))
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select gf_datasource_update('%s','%s','%s')", name, engine, constr);
		return m_pConnection->ExecuteSQL(sql);

		return AG_SUCCESS;
	}

	RESULTCODE ConnectionManagerImpl::Update(g_uint gid, const char* name, const char* engine, const char* constr)
	{
		if(name==NULL || engine==NULL || constr==NULL)
		{
			return AG_FAILURE;
		}

		if(!Has(name))
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select gf_datasource_update(%d,'%s','%s','%s')", gid, name, engine, constr);
		return m_pConnection->ExecuteSQL(sql);

		return AG_SUCCESS;
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
			const char* sql = "select gid,name,engine,uri,version,state,d_uri from g_data_source";
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
				int version = -1;
				const char* d_uri = NULL;
				g_uint count = pResultSet->GetCount();
				for(g_uint i=0; i<count; i++)
				{
					gid = pResultSet->GetInt(i,0);
					name= pResultSet->GetString(i,1);
					engn= pResultSet->GetString(i,2);
					uri = pResultSet->GetString(i,3);
					version = pResultSet->GetInt(i,4);
					stat= pResultSet->GetString(i,5);
					d_uri = pResultSet->GetString(i,6);

					pWorkspace = NewWorkspace(gid, name, engn, uri,version);
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

	Workspace* ConnectionManagerImpl::NewWorkspace(g_uint gid, const char* name, const char* engine, const char* uri, g_uint version)
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
		pWorkspace->SetID(gid);
		pWorkspace->SetConnectionString(uri);
		pWorkspace->SetName(name); 
		pWorkspace->SetVersion(version);

		return pWorkspace;
	}

	RESULTCODE ConnectionManagerImpl::SaveWorkspace(const char* name, const char* engine, const char* uri)
	{
		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "insert into g_data_source (name, engine, uri) values('%s','%s','%s')", name, engine, uri);
		return m_pConnection->ExecuteSQL(sql);
	}

	Workspace* ConnectionManagerImpl::LoadWorkspace(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,name,engine,uri,version,state,d_uri from g_data_source where name='%s'", name);

		Workspace* pWorkspace = NULL;
		GResultSet* pResultSet = NULL;
		pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet==NULL)
		{
			return NULL;
		}

		g_uint count = pResultSet->GetCount();
		if(count==0)
		{
			pResultSet->Release();
			return NULL;
		}
		int	gid = pResultSet->GetInt(0,0);
		//const char* name= pResultSet->GetString(0,1);
		const char* engn= pResultSet->GetString(0,2);
		const char* uri = pResultSet->GetString(0,3);
		int version = pResultSet->GetInt(0,4);
		const char* stat= pResultSet->GetString(0,5);
		const char* d_uri = pResultSet->GetString(0,6);

		pWorkspace = NewWorkspace(gid, name, engn, uri,version);
		pResultSet->Release();

		return pWorkspace;
	}

	Workspace* ConnectionManagerImpl::NewWorkspace(const char* name)
	{
		return NULL;
	}

	//------------------------------------------------------------------------
	// DataSource User Methods Begin
	//------------------------------------------------------------------------
	g_int ConnectionManagerImpl::GetCount(g_uint user_id)
	{
		return m_connections.size();
	}

	Workspace* ConnectionManagerImpl::GetWorkspace(g_uint user_id, const char* name)
	{
		Workspace* pWorkspace = NULL;
		std::vector<Workspace*>::iterator iter;
		for(iter=m_connections.begin(); iter!=m_connections.end(); iter++)
		{
			pWorkspace = *iter;
			if(pWorkspace==NULL)
			{
				continue;
			}

			if(g_stricmp(pWorkspace->GetName(), name)==0)
			{
				if(IsUpdated(pWorkspace))
				{
					Workspace* pnewWorkspace = LoadWorkspace(user_id, name);
					pWorkspace->Close();
					pWorkspace->Release();
					pWorkspace = pnewWorkspace;
					*iter = pnewWorkspace;
				}
				if(pWorkspace!=NULL)
				{
					if(!pWorkspace->IsOpen())
					{
						pWorkspace->Open();
						pWorkspace->AddRef();
					}
				}
				return pWorkspace;
			}
		}
		return NULL;
	}


	EnumWorkspace* ConnectionManagerImpl::GetWorkspaces(g_uint user_id)
	{
		EnumWorkspaceImpl* pWorkspaces = new EnumWorkspaceImpl();
		if(m_pConnection==NULL)
		{
			return pWorkspaces;
		}

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		sprintf(sql, "select gid,name,engine,uri,version,state,d_uri from g_data_source where user_id=%d", user_id);		
		GResultSet* pResultSet = NULL;
		pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet!=NULL)
		{
			Workspace* pWorkspace = NULL;
			int gid = -1;
			int version = 0;
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
				version=pResultSet->GetInt(i,4);
				stat= pResultSet->GetString(i,5);
				d_uri = pResultSet->GetString(i,6);

				pWorkspace = NewWorkspace(gid,name, engn, uri,version);
				if(pWorkspace!=NULL)
				{
					pWorkspaces->Add(pWorkspace);
				}
			}
		}
		return pWorkspaces;
	}

	RESULTCODE ConnectionManagerImpl::Register(g_uint user_id, const char* name, const char* engine, const char* constr)
	{
		if(name==NULL || engine==NULL || constr==NULL)
		{
			return AG_FAILURE;
		}

		Workspace* pWorkspace = NULL;
		pWorkspace = NewWorkspace(-1, name, engine, constr,1);
		if(pWorkspace==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = SaveWorkspace(user_id, name, engine, constr);
		if(rc!=AG_SUCCESS)
		{
			pWorkspace->Release();
			return AG_FAILURE;
		}
		m_connections.push_back(pWorkspace);
		return AG_SUCCESS;
	}

	RESULTCODE ConnectionManagerImpl::Unregister(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "delete from g_data_source where name='%s' and user_id=%d", name, user_id);
		return m_pConnection->ExecuteSQL(sql);
	}

	//RESULTCODE ConnectionManagerImpl::Update(g_uint user_id, const char* name, const char* engine, const char* constr)
	//{
	//	if(name==NULL || engine==NULL || constr==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	if(!Has(name))
	//	{
	//		return AG_FAILURE;
	//	}

	//	char sql[AUGE_SQL_MAX];
	//	g_snprintf(sql, AUGE_SQL_MAX, "select gf_datasource_update('%s','%s','%s')", name, engine, constr);
	//	return m_pConnection->ExecuteSQL(sql);

	//	return AG_SUCCESS;
	//}

	//RESULTCODE ConnectionManagerImpl::Update(g_uint user_id, g_uint gid, const char* name, const char* engine, const char* constr)
	//{
	//	if(name==NULL || engine==NULL || constr==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	if(!Has(name))
	//	{
	//		return AG_FAILURE;
	//	}

	//	char sql[AUGE_SQL_MAX];
	//	g_snprintf(sql, AUGE_SQL_MAX, "select gf_datasource_update(%d,'%s','%s','%s')", gid, name, engine, constr);
	//	return m_pConnection->ExecuteSQL(sql);

	//	return AG_SUCCESS;
	//}

	bool ConnectionManagerImpl::Has(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "select count(*) from g_data_source where name='%s' and use_id=%d", name, user_id);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if( !pResult )
		{
			return false;
		}
		int count = pResult->GetInt(0,0);
		pResult->Release();

		return count;
	}

	g_int ConnectionManagerImpl::GetID(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return -1;
		}

		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "select gid from g_data_source where name='%s' and user_id=%d", name, user_id);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if( !pResult )
		{
			return -1;
		}
		int id = pResult->GetInt(0,0);
		pResult->Release();

		return id;
	}

	Workspace* ConnectionManagerImpl::NewWorkspace(g_uint user_id, g_uint gid, const char* name, const char* engine, const char* uri, g_uint version)
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
		pWorkspace->SetID(gid);
		pWorkspace->SetConnectionString(uri);
		pWorkspace->SetName(name); 
		pWorkspace->SetVersion(version);

		return pWorkspace;
	}

	Workspace* ConnectionManagerImpl::NewWorkspace(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,name,engine,uri,version,state,d_uri from g_data_source where name='%s' and user_id=%d", name, user_id);

		Workspace* pWorkspace = NULL;
		GResultSet* pResultSet = NULL;
		pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet==NULL)
		{
			return NULL;
		}

		g_uint count = pResultSet->GetCount();
		if(count==0)
		{
			pResultSet->Release();
			return NULL;
		}
		int	gid = pResultSet->GetInt(0,0);
		//const char* name= pResultSet->GetString(0,1);
		const char* engn= pResultSet->GetString(0,2);
		const char* uri = pResultSet->GetString(0,3);
		int version = pResultSet->GetInt(0,4);
		const char* stat= pResultSet->GetString(0,5);
		const char* d_uri = pResultSet->GetString(0,6);

		pWorkspace = NewWorkspace(gid, name, engn, uri,version);
		pResultSet->Release();

		RESULTCODE rc = pWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pWorkspace->Release();
			return  NULL;
		}

		return pWorkspace;

	}

	RESULTCODE ConnectionManagerImpl::SaveWorkspace(g_uint user_id, const char* name, const char* engine, const char* uri)
	{
		char sql[AUGE_NAME_MAX];
		g_snprintf(sql, AUGE_NAME_MAX, "insert into g_data_source (name, engine, uri, user_id) values('%s','%s','%s', %d)", name, engine, uri, user_id);
		return m_pConnection->ExecuteSQL(sql);
	}

	Workspace* ConnectionManagerImpl::LoadWorkspace(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,name,engine,uri,version,state,d_uri from g_data_source where name='%s' and user_id=%d", name, user_id);

		Workspace* pWorkspace = NULL;
		GResultSet* pResultSet = NULL;
		pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet==NULL)
		{
			return NULL;
		}

		g_uint count = pResultSet->GetCount();
		if(count==0)
		{
			pResultSet->Release();
			return NULL;
		}
		int	gid = pResultSet->GetInt(0,0);
		//const char* name= pResultSet->GetString(0,1);
		const char* engn= pResultSet->GetString(0,2);
		const char* uri = pResultSet->GetString(0,3);
		int version = pResultSet->GetInt(0,4);
		const char* stat= pResultSet->GetString(0,5);
		const char* d_uri = pResultSet->GetString(0,6);

		pWorkspace = NewWorkspace(gid, name, engn, uri,version);
		pResultSet->Release();

		return pWorkspace;
	}

	//------------------------------------------------------------------------
	// DataSource User Methods End
	//------------------------------------------------------------------------

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
		const char* sql = "CREATE TABLE g_data_source(gid serial NOT NULL,name character varying(32) NOT NULL, d_uri character varying(128) NOT NULL, engine character varying(16) NOT NULL, uri character varying(256) NOT NULL, user_id integer DEFAULT 1, state character varying(8),CONSTRAINT g_data_source_pkey PRIMARY KEY (gid),CONSTRAINT g_data_source_name_key UNIQUE (name))";
		if(m_pConnection->ExecuteSQL(sql)!=AG_SUCCESS)
		{
			return false;
		}
		return true;
	}

	

	bool ConnectionManagerImpl::IsUpdated(Workspace* pWorkspace)
	{
		if(!pWorkspace)
		{
			return true;
		}
		g_uint gid = pWorkspace->GetID();
		g_uint n_version = GetWorkspaceVersion(gid);
		g_uint c_version = pWorkspace->GetVersion();
		bool res = (n_version > c_version);
		return res;
	}

	g_int ConnectionManagerImpl::GetWorkspaceVersion(g_uint gid)
	{
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select version from g_data_source where gid=%d",gid);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}
		g_int version = pResult->GetInt(0,0);
		pResult->Release();
		return version;
	}

	
}
