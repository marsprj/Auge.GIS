#include "MapManager.h"
#include "Map.h"
#include "FeatureLayer.h"

#include "Workspace.h"

#include <libpq-fe.h>

namespace auge
{
	MapManager* MapManager::m_pManager = new MapManager();
	MapManager* MapManager::GetInstance()
	{		
		return m_pManager;
	}

	MapManager::MapManager():
	m_pConnection(NULL)
	{

	}

	MapManager::~MapManager()
	{
		Cleanup();
	}

	std::vector<Map*>& MapManager::GetMaps()
	{
		return m_maps;
	}

	Map* MapManager::GetMap(const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}
		return NULL;
	}

	uint MapManager::GetMapCount()
	{
		const char* sql_map = "select count(*) from ag_map";
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql_map);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return 0;
		}
		int n = PQntuples(pgResult);
		PQclear(pgResult);
		return n;
	}

	int	MapManager::AddMap(Map* pMap)
	{
		if(pMap==NULL)
		{
			return AG_FAILURE;
		}

		int rc = AG_SUCCESS;
		rc = CreateMap(pMap);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		Layer* pLayer = NULL;
		std::vector<Layer*>::iterator iter;
		std::vector<Layer*>& layers = pMap->GetLayers();
		for(iter=layers.begin(); iter!=layers.end(); iter++)
		{
			pLayer = *iter;
			AddLayer(pLayer);
		}
		
		return AG_SUCCESS;
	}

	int MapManager::CreateMap(Map* pMap)
	{
		if(pMap==NULL)
		{
			return AG_FAILURE;
		}

		char sql[PATH_MAX] = {0};
		sprintf(sql, "insert into ag_map (name) values('%s') return id", pMap->GetName());

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}

		uint id = (uint)(atoi(PQgetvalue(pgResult,0,0)));		
		pMap->SetID(id);
		PQclear(pgResult);

		return AG_SUCCESS;
	}

	bool MapManager::FindMap(const char* szName)
	{
		if(szName==NULL)
		{
			return false;
		}
		char sql[PATH_MAX] = {0};
		sprintf(sql, "select count(*) from ag_map where name='%s'", szName);

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return 0;
		}
		int n = PQntuples(pgResult);
		PQclear(pgResult);
		return (n>0);
	}

	int	MapManager::RemoveMap(const char* szName)
	{
		return AG_FAILURE;
	}

	int	MapManager::AddLayer(Layer* pLayer)
	{
		if(pLayer==NULL)
		{
			return AG_FAILURE;
		}

		Map* pMap = pLayer->GetMap();
		uint mid = pMap->GetID();

		return AG_SUCCESS;
	}

	int	MapManager::Initialize()
	{
		const char* sql_map = "create table ag_map (id serial, name character varying(32), owner character varying(32), constraint mid_uk unique(id))";
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql_map);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			//PQclear(pgResult);
			//return AG_FAILURE;
		}
		PQclear(pgResult);

		const char* sql_style = "create table ag_style (id serial, name character varying(32), style character varying(4096), constraint sid_uk unique(id))";
								
		pgResult = PQexec(m_pConnection->m_pgConnection, sql_style);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			//PQclear(pgResult);
			//return AG_FAILURE;
		}
		PQclear(pgResult);

		const char* sql_layer = "create table ag_layer (id serial, name character varying(32), table_name character varying(32), mid integer, sid integer," \
								"CONSTRAINT mid_fk FOREIGN KEY(mid) REFERENCES ag_map  (id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION," \
								"CONSTRAINT sid_fk FOREIGN KEY(sid) REFERENCES ag_style(id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION)";
//								"constraint mid_fk foreign key(mid) references ag_map  (id) match simple on update on action on delete on action," \
//								"constraint sid_fk foreign key(sid) references ag_style(id) match simple on update on action on delete on action)";
		pgResult = PQexec(m_pConnection->m_pgConnection, sql_layer);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			//PQclear(pgResult);
			//return AG_FAILURE;
		}
		PQclear(pgResult);

		return AG_SUCCESS;
	}

	bool MapManager::Initialized()
	{
		const char* sql = "select oid from pg_class where realname='ag_map'";
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return false;
		}
		int n = PQntuples(pgResult);
		PQclear(pgResult);
		return (n>0);
	}

	void MapManager::SetConnection(Workspace* pConnection)
	{
		m_pConnection = pConnection;
	}

	void MapManager::Cleanup()
	{
		Map* m = NULL;
		std::vector<Map*>::iterator iter;
		for(iter=m_maps.begin(); iter!=m_maps.end(); iter++)
		{
			m = *iter;
			m->Release();
		}
		m_maps.clear();
	}
}
