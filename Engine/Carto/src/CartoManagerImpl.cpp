#include "CartoManagerImpl.h"
#include "MapImpl.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "StyleReaderImpl.h"
#include "StyleWriterImpl.h"

namespace auge
{
	CartoManager* augeGetCartoManagerInstance()
	{
		static CartoManagerImpl g_mapManager;
		return &g_mapManager;
	}

	CartoManagerImpl::CartoManagerImpl():
	m_pConnection(NULL)
	{

	}

	CartoManagerImpl::~CartoManagerImpl()
	{

	}

	RESULTCODE CartoManagerImpl::Initialize(GConnection* pConnection)
	{
		if(pConnection==NULL)
		{
			return AG_FAILURE;
		}
		
		if(!pConnection->IsOpen())
		{
			return AG_FAILURE;
		}

		m_pConnection = pConnection;
		if(!m_pConnection->HasTable("g_map"))
		{
			if(!CreateMapTable())
			{
				return AG_FAILURE;
			}
		}
		if(!m_pConnection->HasTable("g_style"))
		{
			if(!CreateStyleTable())
			{
				return AG_FAILURE;
			}
		}
		if(!m_pConnection->HasTable("g_layer"))
		{
			if(!CreateLayerTable())
			{
				return AG_FAILURE;
			}
		}
		return AG_SUCCESS;
	}

	void CartoManagerImpl::Cleanup()
	{

	}

	g_uint CartoManagerImpl::GetCount()
	{
		if(m_pConnection==NULL)
		{
			return 0;
		}
		const char* sql = "select count(*) from g_map";
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = atoi(pResult->GetString(0,0));
		pResult->Release();
		return count;
	}

	Map* CartoManagerImpl::GetMap(g_uint mid)
	{
		if(m_pConnection==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select m_name,version from g_map where gid=%d", mid);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		if(!pResult->GetCount())
		{
			pResult->Release();
			return NULL;
		}

		const char* name = pResult->GetString(0,0);
		g_int version = pResult->GetInt(0,1);

		Map* pMap = new MapImpl();
		pMap->SetID(mid);
		pMap->SetName(name);
		pMap->SetVersion(version);

		pResult->Release();

		return pMap;
	}

	Map* CartoManagerImpl::GetMap(const char* name)
	{
		if(name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,version from g_map where m_name='%s'", name);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		if(!pResult->GetCount())
		{
			pResult->Release();
			return NULL;
		}
	
		g_int id = pResult->GetInt(0,0);
		g_int version = pResult->GetInt(0,1);
		pResult->Release();

		Map* pMap = new MapImpl();
		pMap->SetID(id);
		pMap->SetName(name);
		pMap->SetVersion(version);

		return pMap;
	}

	Map* CartoManagerImpl::LoadMap(g_uint mid)
	{
		if(m_pConnection==NULL)
		{
			return NULL;
		}
		
		Map* pMap = GetMap(mid);
		if(pMap==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_ame,l_type,f_name,d_id,s_id,visible,r_b,w_b,q_b,version from g_layer where m_id=%d order by gid", mid);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		Layer* pLayer = NULL;
		int gid, d_id,s_id,l_type, version, visible;
		const char* l_name, *f_name;
		g_int count = pResult->GetCount();
		for(g_int i=0; i<count; i++)
		{
			gid = pResult->GetInt(i, 0);
			l_name = pResult->GetString(i, 1);
			l_type = pResult->GetInt(i, 2);
			f_name = pResult->GetString(i, 3);
			d_id   = pResult->GetInt(i, 4);
			s_id   = pResult->GetInt(i, 5);
			version= pResult->GetInt(i, 6);
			visible= pResult->GetInt(i, 7);

			pLayer = CreateLayer(gid, l_name, (augeLayerType)l_type, f_name, d_id, version,visible);
			if(pLayer!=NULL)
			{
				Style* pStyle = NULL;
				pStyle = GetStyle(s_id);
				pLayer->SetStyle(pStyle);
				pMap->AddLayer(pLayer);
			}
		}
		pResult->Release();
		return pMap;
	}

	Map* CartoManagerImpl::LoadMap(const char* name)
	{
		if(name==NULL || m_pConnection==NULL)
		{
			return NULL;
		}

		Map* pMap = GetMap(name);
		if(pMap==NULL)
		{
			return NULL;
		}
		g_int mid = pMap->GetID();
		if(mid<0)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_ame,l_type,f_name,d_id,s_id,version,visible,r_b,w_b,q_b from g_layer where m_id=%d order by gid", mid);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		Layer* pLayer = NULL;
		int gid, d_id,s_id,l_type,version,visible;
		const char* l_name, *f_name;
		g_int count = pResult->GetCount();
		for(g_int i=0; i<count; i++)
		{
			gid = pResult->GetInt(i, 0);
			l_name = pResult->GetString(i, 1);
			l_type = pResult->GetInt(i, 2);
			f_name = pResult->GetString(i, 3);
			d_id   = pResult->GetInt(i, 4);
			s_id   = pResult->GetInt(i, 5);
			version= pResult->GetInt(i, 6);
			visible= pResult->GetInt(i, 7);

			pLayer = CreateLayer(gid, l_name, (augeLayerType)l_type, f_name, d_id, version, visible);
			if(pLayer!=NULL)
			{
				Style* pStyle = NULL;
				pStyle = GetStyle(s_id);
				pLayer->SetStyle(pStyle);
				pMap->AddLayer(pLayer);
			}
		}
		pResult->Release();
		return pMap;

		//return LoadMap(mid);
	}

	Map* CartoManagerImpl::CreateMap(const char* name)
	{
		if(name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_map (m_name) values('%s') returning gid", name);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		g_int id = pResult->GetInt(0,0);
		pResult->Release();

		MapImpl* pMap = new MapImpl();
		pMap->SetID(id);
		pMap->SetName(name);

		return pMap;
	}

	Map* CartoManagerImpl::RemoveMap(const char* name)
	{
		return NULL;
	}

	Layer* CartoManagerImpl::CreateLayer(const char* name, augeLayerType type, const char* f_name, g_uint map_id, g_uint source_id, g_uint style_id)
	{
		if(name==NULL||f_name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_layer (l_ame,l_type,f_name,m_id,d_id,s_id) values('%s',%d,'%s',%d,%d,%d) returning gid", name, type, f_name, map_id, source_id,style_id);
		
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		int gid = pResult->GetInt(0,0);
		pResult->Release();

		Layer* pLayer = NULL;
		
		switch(type)
		{
		case augeLayerFeature:
			{
				pLayer = CreateFeatureLayer(name, f_name, source_id);
			}
			break;
		case augeLayerRaster:
			break;
		}
		if(pLayer!=NULL)
		{
			pLayer->SetID(gid);
		}

		return pLayer;
	}

	FeatureLayer* CartoManagerImpl::CreateFeatureLayer(const char* name, const char* f_name, g_uint source_id)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		FeatureLayer* pFLayer = pCartoFactory->CreateFeatureLayer();
		pFLayer->SetName(f_name);
		
		FeatureWorksapce* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = static_cast<FeatureWorksapce*>(pConnManager->GetWorkspaceById(source_id));
		if(pWorkspace==NULL)
		{
			return NULL;
		}

		FeatureClass* pFeatureClass = NULL;
		pFeatureClass = pWorkspace->OpenFeatureClass(f_name);
		if(pFeatureClass==NULL)
		{
			return NULL;
		}
		
		pFLayer->SetFeatureClass(pFeatureClass);
		return pFLayer;
	}

	Layer* CartoManagerImpl::CreateLayer(int id, const char* name, augeLayerType type, const char* f_name, g_int source_id, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		FeatureLayer* pFLayer = pCartoFactory->CreateFeatureLayer();
		pFLayer->SetName(f_name);
		pFLayer->SetID(id);
		pFLayer->SetVersion(version);
		pFLayer->SetVisiable(visible);

		FeatureWorksapce* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = static_cast<FeatureWorksapce*>(pConnManager->GetWorkspaceById(source_id));
		if(pWorkspace!=NULL)
		{
			FeatureClass* pFeatureClass = NULL;
			pFeatureClass = pWorkspace->OpenFeatureClass(f_name);
			if(pFeatureClass!=NULL)
			{
				pFLayer->SetFeatureClass(pFeatureClass);
			}
		}

		return pFLayer;
	}

	bool CartoManagerImpl::HasStyle(g_uint id)
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select count(*) from g_style where gid=%d", id);
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = pResult->GetInt(0,0);
		pResult->Release();
		return (count>0);
	}


	bool CartoManagerImpl::HasStyle(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select count(*) from g_style where s_name='%s'", name);
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = pResult->GetInt(0,0);
		pResult->Release();
		return (count>0);
	}

	g_uint CartoManagerImpl::GetStyleCount()
	{
		const char* sql = "select count(*) g_style g_map";
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = pResult->GetInt(0,0);
		pResult->Release();
		return count; 
	}

	Style* CartoManagerImpl::GetStyle(g_uint id)
	{	
		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select s_name,s_text from g_style where gid=%d", id);
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		if(!pResult->GetCount())
		{
			return NULL;
		}
		const char* name = pResult->GetString(0,0);
		const char* text = pResult->GetString(0,1);

		Style* pStyle = NULL;
		StyleReaderImpl reader;
		pStyle = reader.Read(text, strlen(text));		

		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
			pStyle->SetName(name);
		}
		pResult->Release();
		
		return pStyle;
	}

	Style* CartoManagerImpl::GetStyle(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select gid,s_text from g_style where s_name='%s'", name);
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		if(!pResult->GetCount())
		{
			return NULL;
		}
		g_uint id = pResult->GetInt(0,0);
		const char* text = pResult->GetString(0,1);

		Style* pStyle = NULL;
		StyleReaderImpl reader;
		pStyle = reader.Read(text, strlen(text));
		pResult->Release();

		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
			pStyle->SetName(name);
		}

		return pStyle;
	}

	g_int CartoManagerImpl::CreateStyle(const char* name, Style* pStyle)
	{
		if(name==NULL||pStyle==NULL)
		{
			return AG_FAILURE;
		}

		if(HasStyle(name))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "[%s] already existed.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return AG_FAILURE;
		}

		StyleWriterImpl writer;
		XDocument* pxDoc = NULL;		
		pxDoc = writer.Write(pStyle);
		if(pxDoc==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Serialize [Style] error.");
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
		}

		RESULTCODE rc = AG_FAILURE;
		int nSize = 0;
		g_uchar* buffer = NULL;
		rc = pxDoc->WriteToString(&buffer, nSize, "GBK",1);
		if(rc!=AG_SUCCESS)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Serialize XML Document Error.");
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			pxDoc->Release();
			return NULL;
		}

		std::string sql;
		sql.append("insert into g_style (s_name, s_text) values('");
		sql.append(name);
		sql.append("','");
		sql.append((char*)buffer);
		sql.append("') returning gid");
		
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			pxDoc->Release();
			return NULL;
		}
		
		g_uint gid = pResult->GetInt(0,0);

		pResult->Release();
		pxDoc->Release();

		return gid;
	}

	RESULTCODE CartoManagerImpl::UpdateStyle(const char* name, Style* pStyle)
	{
		if(name==NULL||pStyle==NULL)
		{
			return AG_FAILURE;
		}

		StyleWriterImpl writer;
		XDocument* pxDoc = NULL;		
		pxDoc = writer.Write(pStyle);
		if(pxDoc==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Serialize [Style] error.");
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
		}

		RESULTCODE rc = AG_FAILURE;
		int nSize = 0;
		g_uchar* buffer = NULL;
		rc = pxDoc->WriteToString(&buffer, nSize, "GBK");
		if(rc!=AG_SUCCESS)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Serialize XML Document Error.");
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			pxDoc->Release();
			return NULL;
		}

		char sql[AUGE_SQL_LONG_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_LONG_MAX, "update g_style set s_text='%s' where s_name='%s'", "sss",name);

		rc = m_pConnection->ExecuteSQL(sql);
		pxDoc->Release();

		return (rc==AG_SUCCESS);
	}

	RESULTCODE CartoManagerImpl::RemoveStyle(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "delete from g_style where s_name='%s'", name);

		return m_pConnection->ExecuteSQL(sql);
	}



	//////////////////////////////////////////////////////////////////////////
	RESULTCODE CartoManagerImpl::CreateMapTable()
	{
		const char* sql =   "CREATE TABLE g_map(" \
							"  gid serial NOT NULL," \
							"  m_name character varying(32) NOT NULL," \
							"  version integer DEFAULT 1," \
							"  minx double precision DEFAULT -180," \
							"  maxx double precision DEFAULT -90," \
							"  miny double precision DEFAULT  180," \
							"  maxy double precision DEFAULT  90," \
							"  CONSTRAINT g_map_pkey PRIMARY KEY (gid )," \
							"  CONSTRAINT g_map_name_key UNIQUE (m_name )" \
							")";

		RESULTCODE rc = AG_SUCCESS;
		rc = m_pConnection->ExecuteSQL(sql);
		return (rc == AG_SUCCESS);
	}

	RESULTCODE CartoManagerImpl::CreateLayerTable()
	{
		const char* sql =   "CREATE TABLE g_layer (" \
							"	gid serial NOT NULL," \
							"	l_ame character varying(32) NOT NULL," \
							"	l_type integer NOT NULL DEFAULT 0," \
							"	f_name character varying(32) NOT NULL," \
							"	m_id integer NOT NULL," \
							"	s_id integer DEFAULT -1," \
							"	d_id integer NOT NULL," \
							"	r_b integer NOT NULL DEFAULT 1," \
							"	w_b integer NOT NULL DEFAULT 1," \
							"	q_b integer NOT NULL DEFAULT 1," \
							"	visible integer NOT NULL DEFAULT 1," \
							"	version integer DEFAULT 1," \
							"	CONSTRAINT g_layer_pkey PRIMARY KEY (gid )," \
							"	CONSTRAINT g_layer_m_id_fkey FOREIGN KEY (m_id) " \
							"	REFERENCES g_map (gid) MATCH SIMPLE " \
							"	ON UPDATE NO ACTION ON DELETE NO ACTION," \
							"	CONSTRAINT g_layer_d_id_fkey FOREIGN KEY (d_id) " \
							"	REFERENCES g_data_source (gid) MATCH SIMPLE " \
							"	ON UPDATE NO ACTION ON DELETE NO ACTION" \
							/*"	CONSTRAINT g_layer_s_id_fkey FOREIGN KEY (s_id) " \
							/"	REFERENCES g_style (gid) MATCH SIMPLE " \
							/"	ON UPDATE NO ACTION ON DELETE NO ACTION," \
							"	CONSTRAINT g_layer_l_ame_key UNIQUE (l_ame ) " \*/
							")";

		RESULTCODE rc = AG_SUCCESS;
		rc = m_pConnection->ExecuteSQL(sql);
		return (rc == AG_SUCCESS);

		return AG_SUCCESS;
	}

	RESULTCODE CartoManagerImpl::CreateStyleTable()
	{
		const char* sql =   "CREATE TABLE g_style (" \
			"	gid serial NOT NULL," \
			"	s_name character varying(32) NOT NULL," \
			"	s_text text," \
			"	version integer DEFAULT 1," \
			"   CONSTRAINT g_style_pkey PRIMARY KEY (gid)" \
			")";

		RESULTCODE rc = AG_SUCCESS;
		rc = m_pConnection->ExecuteSQL(sql);
		return (rc == AG_SUCCESS);
	}

	//CREATE TABLE g_layer(gid serial NOT NULL,type integer NOT NULL,version integer,l_ame character varying(32) NOT NULL,f_name character varying(32) NOT NULL,s_id integer NOT NULL,m_id integer,CONSTRAINT g_layer_m_id_fkey FOREIGN KEY (m_id)REFERENCES g_map (gid) MATCH SIMPLEON UPDATE NO ACTION ON DELETE NO ACTION,CONSTRAINT g_layer_s_id_fkey FOREIGN KEY (s_id)REFERENCES g_data_source (gid) MATCH SIMPLEON UPDATE NO ACTION ON DELETE NO ACTION,CONSTRAINT g_layer_l_ame_key UNIQUE (l_ame))
}