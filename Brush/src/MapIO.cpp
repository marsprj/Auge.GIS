#include "MapIO.h"
#include "Map.h"
#include "FeatureLayer.h"
#include "FeatureType.h"
#include "Workspace.h"
#include "Style.h"
#include "StyleIO.h"

#include <libpq-fe.h>

namespace auge
{
	MapIO* MapIO::m_pInstance = new MapIO();
	MapIO* MapIO::GetInstance()
	{		
		return m_pInstance;
	}

	MapIO::MapIO():
	m_pConnection(NULL)
	{

	}

	MapIO::~MapIO()
	{
		Cleanup();
		if(m_pInstance!=NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	std::vector<Map*>& MapIO::GetMaps()
	{
		return m_maps;
	}

	Map* MapIO::LoadMap(const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}
		Map* pMap = NULL;
		pMap = GetMap2(szName);
		if(pMap==NULL)
		{
			return NULL;
		}
		LoadLayers(pMap);

		return pMap;
	}

	uint MapIO::GetMapCount()
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

	int	MapIO::SaveMap(Map* pMap)
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
			SaveLayer(pLayer);
		}
		
		return AG_SUCCESS;
	}

	int MapIO::CreateMap(Map* pMap)
	{
		if(pMap==NULL)
		{
			return AG_FAILURE;
		}

		char sql[PATH_MAX] = {0};
		sprintf(sql, "insert into ag_map (name) values('%s') returning id", pMap->GetName());

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

	bool MapIO::HasMap(const char* szName)
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
		int n = atoi(PQgetvalue(pgResult, 0, 0));
		PQclear(pgResult);
		return (n>0);
	}

	int MapIO::GetMapID(const char* szName)
	{
		if(szName==NULL)
		{
			return -1;
		}
		char sql[PATH_MAX] = {0};
		sprintf(sql, "select id from ag_map where name='%s'", szName);

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return 0;
		}
		int n = PQntuples(pgResult);
		if(n==0)
		{
			PQclear(pgResult);
			return -1;
		}
		n = atoi(PQgetvalue(pgResult, 0, 0));
		PQclear(pgResult);
		return n;
	}

	int	MapIO::RemoveMap(const char* szName)
	{
		int mid = GetMapID(szName);
		if(mid<0)
		{
			return AG_FAILURE;
		}
		RemoveLayers(mid);

		char sql[PATH_MAX] = {0};
		sprintf(sql, "delete from ag_map where name='%s'", szName);
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}
		PQclear(pgResult);
		return AG_FAILURE;
	}

	Layer* MapIO::LoadLayer(int mapID, const char* szLayerName)
	{
		if((mapID<0) || (szLayerName==NULL))
		{
			return NULL;
		}

		char sql[PATH_MAX];
		sprintf(sql, "select id,name,type,table_name,sid from ag_layer where mid=%d and name='%s'", mapID, szLayerName);

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return NULL;
		}

		if(PQntuples(pgResult)==0)
		{
			PQclear(pgResult);
			return NULL;
		}
		
		const char* id = PQgetvalue(pgResult, 0, 0);
		const char* nm = PQgetvalue(pgResult, 0, 1);
		const char* tp = PQgetvalue(pgResult, 0, 2);
		const char* tn = PQgetvalue(pgResult, 0, 3);
		const char* sid= PQgetvalue(pgResult, 0, 4);

		Layer* l = NULL;
		if(strcmp(tp,"feature")==0)
		{
			l = LoadFeatureLayer(atoi(id),nm, tn, atoi(sid));
		}

		//if(l!=NULL)
		//{
		//	pMap->AddLayer(l);
		//}
		return l;
	}

	int	MapIO::SaveLayer(Layer* pLayer)
	{
		if(pLayer==NULL)
		{
			return AG_FAILURE;
		}
		Map* pMap = pLayer->GetMap();
		if(pMap==NULL)
		{
			return AG_FAILURE;
		}
		if(pMap->GetID()<0)
		{
			return AG_FAILURE;
		}

		int ret = AG_FAILURE;
		switch(pLayer->GetType())
		{
		case augeLayerFeature:
			{
				ret = SaveLayer(static_cast<FeatureLayer*>(pLayer));
			}
			break;
		case augeLayerRaster:
			break;
		}

		return ret;
	}

	int	MapIO::SaveLayer(FeatureLayer* pLayer)
	{
		Style* pStyle = NULL;
		pStyle = pLayer->GetStyle();
		if(pStyle!=NULL)
		{
			StyleIO* sio = StyleIO::GetInstance();
			sio->Save(pStyle);
		}

		return WriteLayer(pLayer);
	}

	int	MapIO::WriteLayer(FeatureLayer* pLayer)
	{
		int ret = AG_FAILURE;
		Map *pMap = pLayer->GetMap();
		Style *pStyle= pLayer->GetStyle();
		int mid=-1,sid=-1;
		mid = pMap->GetID();
		if(pStyle!=NULL)
		{
			sid = pStyle->GetID();
		}

		FeatureType* ft = pLayer->GetFeatureType();
		if(ft==NULL)
		{
			return AG_FAILURE;
		}

		const char* lname = pLayer->GetName();
		const char* fname = ft->GetName();

		char sql[PATH_MAX] = {0};
		sprintf(sql, "insert into ag_layer (name, type,table_name, mid, sid) values('%s','feature','%s',%d,%d) returning id", lname, fname,mid,sid);

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}

		int id = atoi(PQgetvalue(pgResult,0,0));		
		pLayer->SetID(id);
		PQclear(pgResult);

		return AG_SUCCESS;
	}

	int MapIO::RemoveLayer(Layer* pLayer)
	{
		if(pLayer==NULL)
		{
			return AG_FAILURE;
		}
		Map* pMap = pLayer->GetMap();
		if(pMap==NULL)
		{
			return AG_FAILURE;
		}
		return RemoveLayer(pMap->GetID(), pLayer->GetName());
	}

	int MapIO::RemoveLayer(int mapID, const char* szLayerName)
	{
		if((mapID<0) || (szLayerName==NULL))
		{
			return AG_FAILURE;
		}

		char sql[PATH_MAX] = {0};
		sprintf(sql, "delete from ag_layer where mid=%d and name='%s'", mapID, szLayerName);
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}
		PQclear(pgResult);

		return AG_SUCCESS;
	}

	int MapIO::RemoveLayer(const char* szMapName, const char* szLayerName)
	{
		if((szMapName==NULL) || (szLayerName==NULL))
		{
			return AG_FAILURE;
		}
		int mid = GetMapID(szMapName);
		return RemoveLayer(mid, szLayerName);
	}

	int MapIO::RemoveLayers(int mapID)
	{
		if(mapID<0)
		{
			return AG_FAILURE;
		}

		char sql[PATH_MAX] = {0};
		sprintf(sql, "delete from ag_layer where mid=%d", mapID);
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}
		PQclear(pgResult);

		return AG_SUCCESS;
	}

	int	MapIO::Initialize()
	{
		const char* sql_map = "create table ag_map (id serial, name character varying(32), owner character varying(32), constraint mid_uk unique(id), constraint mn_uk unique(name))";
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql_map);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			//PQclear(pgResult);
			//return AG_FAILURE;
		}
		PQclear(pgResult);

		const char* sql_layer = "create table ag_layer (id serial, name character varying(32), type character varying(16), table_name character varying(32), mid integer, sid integer," \
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

	bool MapIO::Initialized()
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

	void MapIO::SetConnection(Connection* pConnection)
	{
		m_pConnection = pConnection;
	}

	void MapIO::Cleanup()
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

	/************************************************************************/
	/* Map IO                                                               */
	/************************************************************************/
	Map* MapIO::GetMap2(const char* szName)
	{
		Map* pMap = NULL;
		char sql[PATH_MAX];
		sprintf(sql, "select id,name,owner from ag_map where name='%s'", szName);
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return NULL;
		}
		int n = PQntuples(pgResult);
		if(n==0)
		{
			PQclear(pgResult);
			return NULL;
		}
		
		const char* id = PQgetvalue(pgResult, 0, 0);
		const char* name = PQgetvalue(pgResult, 0, 1);
		const char* owner = PQgetvalue(pgResult, 0, 2);

		pMap = new Map();
		pMap->SetID(atoi(id));
		pMap->SetName(szName);

		PQclear(pgResult);

		return pMap;
	}

	int	MapIO::LoadLayers(Map* pMap)
	{
		char sql[PATH_MAX];
		sprintf(sql, "select id,name,type,table_name,sid from ag_layer where mid=%d", pMap->GetID());

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}

		//const char* id = NULL;
		//const char* nm = NULL;
		//const char* tp = NULL;
		//const char* tn = NULL;
		//const char* sid = NULL;
		Layer* l = NULL;
		int n = PQntuples(pgResult);
		for(int i=0; i<n; i++)
		{
			const char* id = PQgetvalue(pgResult, i, 0);
			const char* nm = PQgetvalue(pgResult, i, 1);
			const char* tp = PQgetvalue(pgResult, i, 2);
			const char* tn = PQgetvalue(pgResult, i, 3);
			const char* sid= PQgetvalue(pgResult, i, 4);

			if(strcmp(tp,"feature")==0)
			{
				l = LoadFeatureLayer(atoi(id),nm, tn, atoi(sid));
			}

			if(l!=NULL)
			{
				pMap->AddLayer(l);
			}
		}
		return AG_SUCCESS;
	}

	FeatureLayer* MapIO::LoadFeatureLayer(int id, const char* name, const char* fname, int sid)
	{
		FeatureLayer* l = NULL;
		FeatureType*  f = NULL;
		Connection* c = m_pConnection;
		Style* s = NULL;
		StyleIO* sio = StyleIO::GetInstance();

		s = sio->Read(sid);
		if(s==NULL)
		{
			return NULL;
		}

		f = c->OpenFeatureType(fname);
		if(f==NULL)
		{
			s->Release();
			return NULL;
		}

		l = new FeatureLayer();
		l->SetID(id);
		l->SetName(name);
		l->SetFeatureType(f);
		l->SetStyle(s);

		return l;
	}
}
