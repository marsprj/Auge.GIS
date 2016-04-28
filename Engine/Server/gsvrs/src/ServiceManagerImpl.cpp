#include "ServiceImpl.h"
#include "ServiceManagerImpl.h"
#include "EnumServiceImpl.h"

#include "AugeData.h"

namespace auge
{
	ServiceManager*	augeGetServiceManagerInstance()
	{
		static ServiceManagerImpl g_serviceManager;
		return &g_serviceManager;
	}

	ServiceManagerImpl::ServiceManagerImpl()
	{
		m_pConnection = NULL;
	}

	ServiceManagerImpl::~ServiceManagerImpl()
	{
		Unload();
	}

	RESULTCODE ServiceManagerImpl::Initialize(GConnection* pConnection)
	{
		if(pConnection==NULL)
		{
			return AG_FAILURE;
		}

		m_pConnection = pConnection;
		if(!m_pConnection->HasTable("g_service"))
		{
			if(CreateServiceTable())
			{
				// error processing 
			}
		}
		//else
		//{
		//	const char* sql = "select gid,name,engine,uri,state from g_data_source";
		//	GResultSet* pResultSet = NULL;
		//	pResultSet = m_pConnection->ExecuteQuery(sql);
		//	if(pResultSet!=NULL)
		//	{
		//		Workspace* pWorkspace = NULL;
		//		int gid = NULL;
		//		const char* name= NULL;
		//		const char* engn=NULL;
		//		const char* uri = NULL;
		//		const char* stat= NULL;
		//		g_uint count = pResultSet->GetCount();
		//		for(g_uint i=0; i<count; i++)
		//		{
		//			gid = pResultSet->GetInt(i,0);
		//			name= pResultSet->GetString(i,1);
		//			engn= pResultSet->GetString(i,2);
		//			uri = pResultSet->GetString(i,3);
		//			stat= pResultSet->GetString(i,4);

		//			pWorkspace = CreateWorkspace(name, engn, uri);
		//			if(pWorkspace!=NULL)
		//			{
		//				pWorkspace->SetID(gid);
		//				m_connections.push_back(pWorkspace);
		//			}
		//		}
		//	}
		//}

		return AG_SUCCESS;
	}

	RESULTCODE ServiceManagerImpl::Unload()
	{
		Cleanup();
		return AG_SUCCESS;
	}

	g_uint ServiceManagerImpl::GetCount()
	{
		return m_services.size();
	}
	
	Service* ServiceManagerImpl::GetService(g_uint user_id, const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}

		if(!Has(user_id, szName))
		{
			return NULL;
		}

		Service* pService = NULL;
		std::vector<Service*>::iterator iter;
		for(iter=m_services.begin(); iter!=m_services.end(); iter++)
		{
			pService = *iter;
			if(strcmp(pService->GetName(), szName)==0)
			{
				return pService;
			}
		}
		pService = LoadService(user_id,szName);
		if(pService!=NULL)
		{
			m_services.push_back(pService);
		}
		return pService;
	}

	g_int ServiceManagerImpl::GetServiceID(g_uint user_id, const char* szName)
	{
		if(szName==NULL||m_pConnection==NULL)
		{
			GLogger	*pLogger = augeGetLoggerInstance();
			pLogger->Error("Service Name is NULL", __FILE__, __LINE__);

			return -1;
		}

		char sql[AUGE_SQL_MAX] = {0};
		//g_sprintf(sql, "select s.gid, m.m_name, s.version, s.state from g_service s, g_map m where s.name='%s' and s.m_id=m.gid", szName);
		g_sprintf(sql, "select gid from g_service where s_name='%s' and user_id=%d", szName, user_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}

		if(!pResult->GetCount())
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Service [%s] not registered.", szName);
			GError	*pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger	*pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);

			pResult->Release();
			return -1; 
		}

		g_uint s_id = pResult->GetInt(0,0);
		pResult->Release();
		return s_id;
	}

	EnumService* ServiceManagerImpl::GetServices(g_uint user_id)
	{
		EnumServiceImpl* pServices = new EnumServiceImpl();

		if(m_pConnection!=NULL)
		{
			g_uint count = 0;
			const char* name = NULL;
			GResultSet* pResult = NULL;
			char sql[AUGE_SQL_MAX];
			memset(sql, 0, AUGE_SQL_MAX);
			g_snprintf(sql, AUGE_SQL_MAX, "select s_name from g_service where user_id=%d order by gid", user_id);

			pResult = m_pConnection->ExecuteQuery(sql);
			
			Service* pService = NULL;
			count = pResult->GetCount();
			for(g_uint i=0; i<count; i++)
			{
				name = pResult->GetString(i,0);
				if(name!=NULL)
				{
					pService = GetService(user_id, name);
					if(pService!=NULL)
					{
						pServices->Add(pService);
					}
				}
			}

			AUGE_SAFE_RELEASE(pResult);
		}
		return pServices;
	}

	Service* ServiceManagerImpl::LoadService(g_uint user_id, const char* szName)
	{
		if(szName==NULL||m_pConnection==NULL)
		{
			GLogger	*pLogger = augeGetLoggerInstance();
			pLogger->Error("Service Name is NULL", __FILE__, __LINE__);

			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		//g_sprintf(sql, "select s.gid, m.m_name, s.version, s.state from g_service s, g_map m where s.name='%s' and s.m_id=m.gid", szName);
		g_sprintf(sql, "select gid, m_id, s_uri, version, state from g_service where s_name='%s' and user_id=%d", szName, user_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		if(!pResult->GetCount())
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Service [%s] not registered.", szName);
			GError	*pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger	*pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);

			pResult->Release();
			return NULL; 
		}

		ServiceImpl* pService = NULL;
		g_uint s_id = pResult->GetInt(0,0);
		g_int m_id = pResult->GetInt(0,1);
		const char* uri = pResult->GetString(0,2);

		pService = new ServiceImpl();
		pService->SetID(s_id);
		pService->SetName(szName);
		pService->SetURI(uri);
		if(m_id>0)
		{
			Map* pMap = NULL;
			CartoManager* pCartoManager = augeGetCartoManagerInstance();
			pMap = pCartoManager->LoadMap(m_id);
			if(pMap!=NULL)
			{
				pService->SetMapName(szName);
				pService->SetMap(pMap);
			}
		}
		else
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Service [%s] does not attach Map.", szName);
			GLogger	*pLogger = augeGetLoggerInstance();
			pLogger->Info(msg, __FILE__, __LINE__);
		}

		pResult->Release();
		return pService;
	}

	bool ServiceManagerImpl::Has(g_uint user_id, const char* szName)
	{
		if(szName==NULL)
		{
			return false;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select count(*) from g_service where s_name='%s' and user_id=%d", szName, user_id);
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}
		
		int count = pResult->GetInt(0,0);
		pResult->Release();
		
		return count;
	}

	RESULTCODE ServiceManagerImpl::Register(g_uint user_id, const char* szName, const char* szURI)
	{
		if(m_pConnection==NULL||szName==NULL||szURI==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "insert into g_service (user_id, s_name, s_uri) values(%d,'%s','%s')", user_id, szName, szURI);
		
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE ServiceManagerImpl::Register(g_uint user_id, const char* szName, const char* mapName, const char* szURI)
	{
		if(m_pConnection==NULL||szName==NULL||mapName==NULL||szURI==NULL)
		{
			return AG_FAILURE;
		}

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		g_int map_id = pCartoManager->GetMapID(user_id, mapName);
		if(map_id<0)
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "Fail to get map [%s]", mapName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "insert into g_service (user_id, s_name, s_uri) values(%d,'%s','%s') returning gid", user_id, szName, szURI);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}

		g_int service_id = pResult->GetInt(0,0);
		pResult->Release();

		return RegisterMap(service_id, map_id);
	}

	RESULTCODE ServiceManagerImpl::Unregister(g_uint user_id, const char* szName)
	{
		if(m_pConnection==NULL||szName==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "delete from g_service where s_name='%s'", szName); 
		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);

		Remove(user_id, szName);

		return rc;
	}

	RESULTCODE ServiceManagerImpl::RegisterMap(g_uint s_id, g_uint m_id)
	{
		if(m_pConnection==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "update g_service set m_id=%d where gid=%d", m_id,s_id);
		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);
		
		return rc;
	}

	RESULTCODE ServiceManagerImpl::Start(g_uint user_id, const char* szName)
	{	
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		if(szName==NULL)
		{
			const char* msg = "Service Name is NULL";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}
		if(!Has(user_id, szName))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_snprintf(msg, AUGE_MSG_MAX, "Cannot find Service [%s]");
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "update g_service set state=0 where user_id=%d and s_name='%s'", user_id, szName);

		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE ServiceManagerImpl::Stop(g_uint user_id, const char* szName)
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		if(szName==NULL)
		{
			const char* msg = "Service Name is NULL";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}
		if(!Has(user_id, szName))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_snprintf(msg, AUGE_MSG_MAX, "Cannot find Service [%s]");
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "update g_service set state=1 where user_id=%d and s_name='%s'", user_id, szName);

		return m_pConnection->ExecuteSQL(sql);
	}

	void ServiceManagerImpl::Cleanup()
	{
		Service* pService = NULL;
		std::vector<Service*>::iterator iter;
		for(iter=m_services.begin(); iter!=m_services.end(); iter++)
		{
			pService = *iter;
			pService->Release();
		}
		m_services.clear();
	}

	bool ServiceManagerImpl::CreateServiceTable()
	{
		const char* sql = "CREATE TABLE g_service(gid serial NOT NULL,user_id integer NOT NULL DEFAULT 1,s_name character varying(32) NOT NULL,s_uri character varying(128) NOT NULL,m_id integer DEFAULT -1,version integer DEFAULT 1,state integer NOT NULL DEFAULT 0,CONSTRAINT g_service_pkey PRIMARY KEY (gid ),CONSTRAINT g_service_s_name_key UNIQUE (s_name),CONSTRAINT g_service_user_fkey FOREIGN KEY (user_id) REFERENCES g_user (gid) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION)";
		return (m_pConnection->ExecuteSQL(sql)==AG_SUCCESS);
	}

	RESULTCODE ServiceManagerImpl::Remove(g_uint user_id, const char* szName)
	{
		if(szName==NULL)
		{
			return AG_FAILURE;
		}

		const char* name = NULL;
		Service* pService = NULL;
		std::vector<Service*>::iterator iter;
		for(iter=m_services.begin(); iter!=m_services.end(); iter++)
		{
			pService = *iter;
			if(pService!=NULL)
			{
				name = pService->GetName();
				if(!strcmp(name, szName))
				{
					m_services.erase(iter);
					break;
				}
			}
			
		}

		return AG_SUCCESS;
	}
}