#include "CartoManagerImpl.h"
#include "MapImpl.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeFeature.h"
#include "StyleReaderImpl.h"
#include "StyleWriterImpl.h"
#include "EnumStyleImpl.h"
#include "EnumMapImpl.h"
#include "EnumColorMapImpl.h"
#include "ColorMapImpl.h"
#include "FeatureStyleImpl.h"

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
			else
			{
				//InitStyleTable();
			}
		}
		if(!m_pConnection->HasTable("g_layer"))
		{
			if(!CreateLayerTable())
			{
				return AG_FAILURE;
			}
		}

		if(!m_pConnection->HasTable("g_color_map"))
		{
			if(!CreateColorMapTable())
			{
				return AG_FAILURE;
			}
			else
			{
				InitColorMap();
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

	Map* CartoManagerImpl::CreateMap(GResultSet* pResult, int row)
	{
		int mid = pResult->GetInt(row,0);
		const char* name = pResult->GetString(row,1);
		const char* uri = pResult->GetString(row,2);
		g_int version = pResult->GetInt(row,3);
		double xmin = pResult->GetDouble(row,4);
		double ymin = pResult->GetDouble(row,5);
		double xmax = pResult->GetDouble(row,6);
		double ymax = pResult->GetDouble(row,7);
		g_int  srid = pResult->GetInt(row,8);
		const char* thumbnail = pResult->GetString(row,9);
		double v_xmin = pResult->GetDouble(row,10);
		double v_ymin = pResult->GetDouble(row,11);
		double v_xmax = pResult->GetDouble(row,12);
		double v_ymax = pResult->GetDouble(row,13);

		MapImpl* pMap = new MapImpl();
		pMap->SetID(mid);
		pMap->SetURI(uri);
		pMap->SetName(name);
		pMap->SetVersion(version);
		pMap->SetExtent(xmin, ymin,xmax,ymax);
		pMap->SetViewer(v_xmin, v_ymin,v_xmax,v_ymax);
		pMap->SetSRID(srid);
		pMap->SetThumbnail(thumbnail);
		return pMap;
	}

	Map* CartoManagerImpl::GetMap(g_uint mid)
	{
		if(m_pConnection==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select gid, m_name,m_uri,version,minx,miny,maxx,maxy,srid,thumbnail,v_minx,v_miny,v_maxx,v_maxy from g_map where gid=%d", mid);

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

		Map* pMap = CreateMap(pResult, 0);

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
		g_snprintf(sql, AUGE_SQL_MAX, "select gid, m_name,m_uri,version,minx,miny,maxx,maxy,srid,thumbnail,v_minx,v_miny,v_maxx,v_maxy from g_map where m_name='%s'", name);

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

		Map* pMap = CreateMap(pResult, 0);
		pResult->Release();

		return pMap;
	}

	EnumMap* CartoManagerImpl::GetMaps()
	{
		if(m_pConnection==NULL)
		{
			return NULL;
		}

		const char* sql = "select gid,m_name,m_uri,version,minx,miny,maxx,maxy,srid,thumbnail,v_minx,v_miny,v_maxx,v_maxy from g_map";

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumMapImpl* pEnumMap = new EnumMapImpl();
		int count = pResult->GetCount();
		for(int i=0; i<count; i++) 
		{
			//int mid = pResult->GetInt(i,0);
			//const char* name = pResult->GetString(i,1);
			//const char* uri = pResult->GetString(i,2);
			//g_int version = pResult->GetInt(i,3);
			//double xmin = pResult->GetDouble(i,4);
			//double ymin = pResult->GetDouble(i,5);
			//double xmax = pResult->GetDouble(i,6);
			//double ymax = pResult->GetDouble(i,7);
			//g_int  srid = pResult->GetInt(i,8);

			//Map* pMap = new MapImpl();
			//pMap->SetID(mid);
			//pMap->SetURI(uri);
			//pMap->SetName(name);
			//pMap->SetVersion(version);
			//pMap->SetExtent(xmin, ymin,xmax,ymax);
			//pMap->SetSRID(srid);
			Map* pMap = CreateMap(pResult, i);
			if(pMap!=NULL)
			{
				pEnumMap->Add(pMap);
			}
		}
		
		pResult->Release();
		return pEnumMap;
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
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_name,l_type,f_name,d_id,s_id,version,visible,r_b,w_b,q_b,f_path,web_url from g_layer where m_id=%d order by gid", mid);
		
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		Layer* pLayer = NULL;
		int gid, d_id,s_id,l_type, version, visible;
		const char* l_name, *f_name, *f_path, *web_url;
		g_int count = pResult->GetCount();
		for(g_int i=0; i<count; i++)
		{
			gid = pResult->GetInt(i, 0);
			l_name = pResult->GetString(i, 1);
			l_type = pResult->GetInt(i, 2);
			f_name = pResult->GetString(i, 3);
			f_path = pResult->GetString(i, 11);
			d_id   = pResult->GetInt(i, 4);
			s_id   = pResult->GetInt(i, 5);
			version= pResult->GetInt(i, 6);
			visible= pResult->GetInt(i, 7);
			web_url= pResult->GetString(i,12);

			pLayer = CreateLayer(gid, l_name, (augeLayerType)l_type, f_name, f_path, d_id, s_id, version,visible, web_url);
			if(pLayer!=NULL)
			{
				pMap->AddLayer(pLayer);
				//switch(pLayer->GetType())
				//{
				//case augeLayerFeature:
				//	{
				//		FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
				//		FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
				//		Style* pStyle = GetStyle(s_id, pFeatureClass);
				//		pFeatureLayer->SetStyle(pStyle);
				//	}
				//	break;
				//}
			}
		}
		pResult->Release();
		return pMap;
	}

	//Map* CartoManagerImpl::LoadMap(const char* name)
	//{
	//	if(name==NULL || m_pConnection==NULL)
	//	{
	//		return NULL;
	//	}

	//	Map* pMap = GetMap(name);
	//	if(pMap==NULL)
	//	{
	//		return NULL;
	//	}
	//	g_int mid = pMap->GetID();
	//	if(mid<0)
	//	{
	//		return NULL;
	//	}

	//	char sql[AUGE_SQL_MAX] = {0};
	//	//g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_name,l_type,f_name,d_id,s_id,version,visible,r_b,w_b,q_b,web_url from g_layer where m_id=%d order by gid", mid);
	//	g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_name,l_type,f_name,d_id,s_id,version,visible,r_b,w_b,q_b,web_url from g_layer where m_id=%d order by gid", mid);

	//	GResultSet* pResult = NULL;
	//	pResult = m_pConnection->ExecuteQuery(sql);
	//	if(pResult==NULL)
	//	{
	//		return NULL;
	//	}

	//	Layer* pLayer = NULL;
	//	int gid, d_id,s_id,l_type,version,visible;
	//	const char* l_name, *f_name, *web_url;
	//	g_int count = pResult->GetCount();
	//	for(g_int i=0; i<count; i++)
	//	{
	//		gid = pResult->GetInt(i, 0);
	//		l_name = pResult->GetString(i, 1);
	//		l_type = pResult->GetInt(i, 2);
	//		f_name = pResult->GetString(i, 3);
	//		d_id   = pResult->GetInt(i, 4);
	//		s_id   = pResult->GetInt(i, 5);
	//		version= pResult->GetInt(i, 6);
	//		visible= pResult->GetInt(i, 7);
	//		web_url= pResult->GetString(i,11);


	//		pLayer = CreateLayer(gid, l_name, (augeLayerType)l_type, f_name, d_id, s_id, version, visible, web_url);
	//		if(pLayer!=NULL)
	//		{
	//			pMap->AddLayer(pLayer);
	//			//switch(pLayer->GetType())
	//			//{
	//			//case augeLayerFeature:
	//			//	{
	//			//		FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
	//			//		FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
	//			//		Style* pStyle = GetStyle(s_id, pFeatureClass);
	//			//		pFeatureLayer->SetStyle(pStyle);
	//			//	}
	//			//	break;
	//			//case augeLayerRaster:
	//			//	break;
	//			//}
	//		}
	//	}
	//	pResult->Release();
	//	return pMap;

	//	//return LoadMap(mid);
	//}

	//Map* CartoManagerImpl::LoadMap(const char* name)
	//{
	//	if(name==NULL || m_pConnection==NULL)
	//	{
	//		return NULL;
	//	}

	//	Map* pMap = GetMap(name);
	//	if(pMap==NULL)
	//	{
	//		return NULL;
	//	}
	//	g_int mid = pMap->GetID();
	//	if(mid<0)
	//	{
	//		return NULL;
	//	}

	//	LoadLayers(pMap);

	//	return pMap;

	//	//return LoadMap(mid);
	//}

	RESULTCODE CartoManagerImpl::LoadLayers(Map* pMap)
	{
		RESULTCODE rc = AG_FAILURE;
		std::string map_layers;
		rc = GetMapLayers(map_layers, pMap->GetID());
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		char layer_ids[AUGE_PATH_MAX];
		memset(layer_ids, 0, AUGE_PATH_MAX);
		PraseMapLayerIds(layer_ids, map_layers.c_str());

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_name,l_type,f_name,d_id,s_id,version,visible,r_b,w_b,q_b,f_path,web_url from g_layer where gid in (%s)", layer_ids);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}

		Layer* pLayer = NULL;
		int gid, d_id,s_id,l_type, version, visible;
		const char* l_name, *f_name, *f_path, *web_url;
		
		int layer_id = 0;
		int layer_visiblity = 1;
		int count = pResult->GetCount();
		char* str = strdup(map_layers.c_str());
		const char* ptr = strtok(str,",");
		while(ptr!=NULL)
		{
			for(int i=0; i<count; i++)
			{
				//layer_id = atoi(ptr);
				sscanf(ptr, "%d:%d", &layer_id, &layer_visiblity);
				if(pResult->GetInt(i,0)==layer_id)
				{
					gid = pResult->GetInt(i, 0);
					l_name = pResult->GetString(i, 1);
					l_type = pResult->GetInt(i, 2);
					f_name = pResult->GetString(i, 3);
					f_path = pResult->GetString(i, 11);
					d_id   = pResult->GetInt(i, 4);
					s_id   = pResult->GetInt(i, 5);
					version= pResult->GetInt(i, 6);
					visible= pResult->GetInt(i, 7);
					web_url= pResult->GetString(i,12);

					pLayer = CreateLayer(gid, l_name, (augeLayerType)l_type, f_name, f_path, d_id, s_id, version,visible, web_url);
					if(pLayer!=NULL)
					{
						pLayer->SetVisiable(layer_visiblity);
						pMap->AddLayer(pLayer);
					} 
				}
			}

			ptr = strtok(NULL,",");
		}
		free(str);

		return AG_SUCCESS;
	}

	RESULTCODE CartoManagerImpl::PraseMapLayerIds(char* layer_ids, const char* map_layers)
	{
		layer_ids[0] = '\0';
		int id,v;
		char szid[AUGE_NAME_MAX];
		char* str = strdup(map_layers);
		char* ptr = strtok(str,",");
		while(ptr!=NULL)
		{
			sscanf(ptr, "%d:%d", &id, &v);
			g_sprintf(szid, "%d,", id);
			strcat(layer_ids, szid);

			ptr = strtok(NULL,",");
		}
		free(str);
		size_t len = strlen(layer_ids);
		if(len>0)
		{ 
			layer_ids[len-1] = '\0';
		}
		return AG_SUCCESS;
	}

	RESULTCODE CartoManagerImpl::GetMapLayers(std::string& map_layers, g_uint mapID)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_sprintf(sql, "select m_layers from g_map where gid=%d", mapID);
		
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}
		if(!pResult->GetCount())
		{
			pResult->Release();
			return AG_FAILURE;
		}
		map_layers = pResult->GetString(0,0);
		pResult->Release();
		return AG_SUCCESS;
	}

	Map* CartoManagerImpl::CreateMap(const char* name)
	{
		if(name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		char uuid[AUGE_PATH_MAX];
		memset(uuid, 0, AUGE_PATH_MAX);
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_map (m_name,m_uuid) values('%s','%s') returning gid", name, uuid);

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

	Map* CartoManagerImpl::CreateMap(const char* name, g_uint srid, double xmin, double ymin, double xmax, double ymax)
	{
		if(name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}
		char uuid[AUGE_PATH_MAX];
		memset(uuid, 0, AUGE_PATH_MAX);
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_map (m_name, minx, miny, maxx, maxy,v_minx,v_miny,v_maxx,v_maxy,srid,m_uuid) values('%s',%f,%f,%f,%f,%f,%f,%f,%f,%d,'%s') returning gid", name, xmin, ymin, xmax, ymax,xmin, ymin, xmax, ymax,srid,uuid);

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

	RESULTCODE CartoManagerImpl::RemoveMap(const char* name)
	{
		if( name == NULL )
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "delete from g_map where m_name='%s' ", name);
		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);
		if(rc==AG_SUCCESS)
		{

		}
		return rc;
	}

	g_int CartoManagerImpl::GetMapID(const char* name)
	{
		if(name==NULL)
		{
			return -1;
		}
		g_int mapID = -1;
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select gid from g_map where m_name='%s'", name);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}
		if(!pResult->GetCount())
		{
			return -1;
		}

		mapID = pResult->GetInt(0,0);
		pResult->Release();

		return mapID;
	}

	RESULTCODE CartoManagerImpl::SetMapThumbnail(g_uint user_id, g_uint map_id, const char* thumbnail)
	{
		if(thumbnail==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "update g_map set thumbnail='%s' where gid=%d", thumbnail, map_id);
		return m_pConnection->ExecuteSQL(sql);
	}

	Layer* CartoManagerImpl::CreateLayer(const char* name, augeLayerType type, const char* f_name, g_uint map_id, g_uint source_id, g_uint style_id)
	{
		if(name==NULL||f_name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		if(HasLayer(name, map_id))
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Layer [%s] already exists.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			return NULL;
		}

		Layer* pLayer = NULL;
		switch(type)
		{
		case augeLayerFeature:
			pLayer = CreateFeatureLayer(name, f_name, source_id);
			break;
		case augeLayerGraphic:
			pLayer = CreateGraphicLayer(name, f_name, source_id);
			break;
		case augeLayerRaster:
			break;
		}

		if(pLayer==NULL)
		{
			return NULL;
		}


		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_layer (l_name,l_type,f_name,m_id,d_id,s_id) values('%s',%d,'%s',%d,%d,%d) returning gid", name, type, f_name, map_id, source_id,style_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		int gid = pResult->GetInt(0,0);
		pResult->Release();
		pLayer->SetID(gid);

		return pLayer;
	}

	Layer* CartoManagerImpl::CreateRasterLayer(const char* name, const char* r_name, const char* r_path, g_uint map_id, g_uint source_id)
	{
		if(r_name==NULL||r_path==NULL)
		{
			return NULL;
		}

		if(HasLayer(name, map_id))
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Layer [%s] already exists.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			return NULL;
		}
		
		Layer* pLayer = CreateRasterLayer(name, r_name, r_path, source_id);
		if(pLayer==NULL)
		{
			return NULL;
		}


		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_layer (l_name,l_type,f_name,f_path,m_id,d_id) values('%s',%d,'%s','%s',%d,%d) returning gid", name, augeLayerRaster, r_name, r_path, map_id, source_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		int gid = pResult->GetInt(0,0);
		pResult->Release();
		pLayer->SetID(gid);

		return pLayer;
	}

	Layer* CartoManagerImpl::CreateWebLayer(const char* name, augeLayerType type, const char* url, g_uint map_id)
	{
		if(name==NULL||url==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		if(HasLayer(name, map_id))
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Layer [%s] already exists.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			return NULL;
		}

		WebLayer* pWebLayer = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		switch(type)
		{
		case augeLayerWMS:
			break;
		case augeLayerWMTS:
			break;
		case augeLayerWFS:
			break;
		case augeLayerQuadServer:
			pWebLayer = pCartoFactory->CreateQuadServerLayer();
			break;
		}
		if(pWebLayer==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_layer (l_name,l_type,f_name,m_id,d_id,s_id, web_url) values('%s',%d,'%s',%d,%d,%d,'%s') returning gid", name, type, "", map_id, "", "", url);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		int gid = pResult->GetInt(0,0);
		pResult->Release();

		pWebLayer->SetID(gid);
		return pWebLayer;
	}

	g_int CartoManagerImpl::GetLayerID(const char* layerName, const char* mapName)
	{
		if(layerName==NULL||mapName==NULL)
		{
			return -1;
		}

		g_int map_id = GetMapID(mapName);
		if(map_id<0)
		{
			return -1;
		}

		g_int layer_id = -1;
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select gid from g_layer where m_id=%d and l_name='%s'", map_id, layerName);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}
		if(pResult->GetCount())
		{
			layer_id = pResult->GetInt(0,0);
		}
		pResult->Release();

		return layer_id;
	}

	bool CartoManagerImpl::HasLayer(const char* layerName, int mapID)
	{
		if(layerName==NULL)
		{
			return false;
		}

		g_int layer_id = -1;
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select gid from g_layer where m_id=%d and l_name='%s'", mapID, layerName);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}
		bool has = pResult->GetCount();
		pResult->Release();
		return has;
	}

	FeatureLayer* CartoManagerImpl::CreateFeatureLayer(const char* name, const char* f_name, g_uint source_id)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		FeatureLayer* pFLayer = pCartoFactory->CreateFeatureLayer();
		pFLayer->SetName(f_name);

		FeatureWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(source_id));
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

	GraphicLayer* CartoManagerImpl::CreateGraphicLayer(const char* name, const char* f_name, g_uint source_id)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		GraphicLayer* pGLayer = pCartoFactory->CreateGraphicLayer();
		pGLayer->SetName(f_name);

		FeatureWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(source_id));
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

		pGLayer->SetFeatureClass(pFeatureClass);
		return pGLayer;
	}

	RasterLayer* CartoManagerImpl::CreateRasterLayer(const char* name, const char* r_name, const char* r_path, g_uint source_id)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		RasterLayer* pRLayer = pCartoFactory->CreateRasterLayer();
		pRLayer->SetName(name);

		RasterWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<RasterWorkspace*>(pConnManager->GetWorkspaceById(source_id));
		if(pWorkspace==NULL)
		{
			return NULL;
		}

		RasterFolder* pRasterFolder = NULL;
		pRasterFolder = pWorkspace->GetFolder(r_path);
		if(pRasterFolder==NULL)
		{
			return NULL;
		}
		RasterDataset* pRasterDataset = NULL;
		pRasterDataset = pRasterFolder->GetRasterDataset();

		Raster* pRaster = NULL;
		pRaster = pRasterDataset->GetRaster(r_name);
		if(pRaster==NULL)
		{
			pRasterFolder->Release();
			return NULL;
		}
		pRLayer->SetRaster(pRaster, pRasterDataset);
		return pRLayer;
	}

	Layer* CartoManagerImpl::CreateLayer(int id, const char* name, augeLayerType type, const char* f_name, const char* f_path, g_int source_id, g_int style_id, g_int version, bool visible, const char* web_url)
	{
		Layer* pLayer = NULL;
		switch(type)
		{
		case augeLayerFeature:
			pLayer = CreateFeatureLayer(id, name, f_name, source_id, style_id, version, visible);
			break;
		case augeLayerGraphic:
			pLayer = CreateGraphicLayer(id, name, f_name, source_id, style_id, version, visible);
			break;
		case augeLayerRaster:
 			pLayer = CreateRasterLayer(id, name, f_name, f_path, source_id, version, visible);
			break;
		case augeLayerQuadServer:
			pLayer = CreateQuadServerLayer(id, name, web_url, version, visible);
		}

		return pLayer;
	}

	FeatureLayer* CartoManagerImpl::CreateFeatureLayer(int id, const char* name, const char* f_name, g_int source_id, g_int style_id, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance(); 

		FeatureWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(source_id));
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

		FeatureLayer* pFLayer = pCartoFactory->CreateFeatureLayer();
		if(pFLayer==NULL)
		{
			pFeatureClass->Release();
			return NULL;
		}
		pFLayer->SetName(name);
		pFLayer->SetID(id);
		pFLayer->SetVersion(version);
		pFLayer->SetVisiable(visible);
		pFLayer->SetFeatureClass(pFeatureClass);

		Style* pStyle = GetStyle(style_id, pFeatureClass);
		pFLayer->SetStyle(pStyle);

		return pFLayer;
	}

	GraphicLayer* CartoManagerImpl::CreateGraphicLayer(int id, const char* name, const char* f_name, g_int source_id, g_int style_id, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance(); 

		FeatureWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(source_id));
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

		GraphicLayer* pGLayer = pCartoFactory->CreateGraphicLayer();
		if(pGLayer==NULL)
		{
			pFeatureClass->Release();
			return NULL;
		}
		pGLayer->SetName(name);
		pGLayer->SetID(id);
		pGLayer->SetVersion(version);
		pGLayer->SetVisiable(visible);
		pGLayer->SetFeatureClass(pFeatureClass);

		Style* pStyle = GetStyle(style_id, pFeatureClass);
		pGLayer->SetStyle(pStyle);

		return pGLayer;
	}

	RasterLayer* CartoManagerImpl::CreateRasterLayer(int id, const char* name, const char* r_name, const char* r_path, g_int source_id, g_int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

		RasterLayer* pRLayer = pCartoFactory->CreateRasterLayer();
		pRLayer->SetName(name);

		RasterWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<RasterWorkspace*>(pConnManager->GetWorkspaceById(source_id));
		if(pWorkspace==NULL)
		{
			return NULL;
		}

		RasterFolder* pRasterFolder = NULL;
		pRasterFolder = pWorkspace->GetFolder(r_path);
		if(pRasterFolder==NULL)
		{
			return NULL;
		}
		RasterDataset* pRasterDataset = NULL;
		pRasterDataset = pRasterFolder->GetRasterDataset();

		Raster* pRaster = NULL;
		pRaster = pRasterDataset->GetRaster(r_name);
		if(pRaster==NULL)
		{
			pRasterFolder->Release();
			return NULL;
		}
		pRLayer->SetRaster(pRaster, pRasterDataset);

		pRLayer->SetID(id);
		pRLayer->SetVersion(version);
		pRLayer->SetVisiable(visible);

		return pRLayer;
	}

	QuadServerLayer* CartoManagerImpl::CreateQuadServerLayer(int id, const char* name, const char* url, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance(); 

		QuadServerLayer* pLayer = pCartoFactory->CreateQuadServerLayer();
		if(pLayer==NULL)
		{
			return NULL;
		}

		pLayer->SetName(name);
		pLayer->SetID(id);
		pLayer->SetVersion(version);
		pLayer->SetVisiable(visible);
		pLayer->SetURL(url);
		return pLayer;
	}

	RESULTCODE CartoManagerImpl::RemoveLayers(const char* mapName)
	{
		if(mapName==NULL)
		{
			return AG_FAILURE;
		}

		g_int mapID = GetMapID(mapName);
		if(mapID<0)
		{
			return AG_FAILURE;
		}
		return RemoveLayers(mapID);
	}

	RESULTCODE CartoManagerImpl::UpdateMapLayers(Map* pMap)
	{
		if(pMap==NULL)
		{
			return AG_FAILURE;
		}

		char layge_id[AUGE_NAME_MAX];
		char layers[AUGE_MSG_MAX];
		memset(layers,0,AUGE_MSG_MAX);

		Layer* pLayer = NULL;
		int count = pMap->GetLayerCount();
		for(int i=count-1;i>=0;i--)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				sprintf(layge_id,"%d",pLayer->GetID());				
				strcat(layers,layge_id);
				if(i)
				{
					strcat(layers,",");
				}
			}
		}

		char sql[AUGE_SQL_MAX];
		sprintf(sql, "update g_map set m_layers='%s' where gid=%d", layers,pMap->GetID());
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE CartoManagerImpl::UpdateMapLayers(g_uint mapID, const char* layers)
	{
		if(mapID<0||layers==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX];
		sprintf(sql, "update g_map set m_layers='%s' where gid=%d", layers,mapID);
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE CartoManagerImpl::UpdateMapLayers(const char* mapName, const char* layers)
	{
		if(mapName==NULL||layers==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX];
		sprintf(sql, "update g_map set m_layers='%s' where m_name=%d", layers,mapName);
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE CartoManagerImpl::GetMapLayers(g_uint mapID, char* layers)
	{
		if(layers==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX];
		sprintf(sql, "select m_layers='%s' from g_map where gid=%d", mapID);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}

		strcpy(layers, pResult->GetString(0,0));
		pResult->Release();
		//return m_pConnection->ExecuteSQL(sql);
		return AG_SUCCESS;
	}

	RESULTCODE CartoManagerImpl::RemoveLayers(g_uint map_id)
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"delete from g_layer where m_id=%d", map_id);
		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE CartoManagerImpl::RemoveLayer(const char* mapName, const char* layerName)
	{
		if(mapName==NULL||layerName==NULL)
		{
			return AG_FAILURE;
		}

		g_int mapID = GetMapID(mapName);
		if(mapID<0)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot find map [%s].", mapName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
		}

		char sql[AUGE_SQL_MAX];
		g_sprintf(sql, "delete from g_layer where m_id=%d and l_name='%s'", mapID, layerName);

		return m_pConnection->ExecuteSQL(sql);
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

	EnumStyle* CartoManagerImpl::GetStyles()
	{
		GResultSet* pResult = NULL;
		const char* sql = "select gid, s_name,s_type from g_style";
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		FeatureStyleImpl* pStyle = NULL;
		StyleReaderImpl reader;
		EnumStyleImpl* pEnums = new EnumStyleImpl();
		g_int count = pResult->GetCount();
		for(g_int i=0; i<count; i++)
		{
			g_int id = pResult->GetInt(i,0);
			const char* name = pResult->GetString(i,1);
			const char* type = pResult->GetString(i,2);

			//pStyle = reader.Read(text, strlen(text), NULL);	
			pStyle = new FeatureStyleImpl();
			if(pStyle==NULL)
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("Bad Style XML Document", __FILE__,__LINE__);
				//pLogger->Debug(text);
				continue;
			}
			pStyle->SetID(id);
			pStyle->SetName(name);
			pEnums->Add(pStyle);

			switch(pStyle->GetType())
			{
			case augeStyleFeature:
				{
					GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
					augeGeometryType gtype = pGeometryFactory->DecodeGeometryType(type);

					FeatureStyle* pFStyle = static_cast<FeatureStyle*>(pStyle);
					pFStyle->SetGeometryType(gtype);
				}
				break;
			case augeStyleRaster:
				break;
			}
		}
		pResult->Release();

		return pEnums;
	}

	Style* CartoManagerImpl::GetStyle(g_uint id, FeatureClass* pFeatureClass)
	{	
		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select s_name,s_text,s_type from g_style where gid=%d", id);
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
		const char* type = pResult->GetString(0,2);

		Style* pStyle = NULL;
		StyleReaderImpl reader;
		pStyle = reader.Read(text, strlen(text), pFeatureClass);		

		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
			pStyle->SetName(name);

			switch(pStyle->GetType())
			{
			case augeStyleFeature:
				{
					GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
					augeGeometryType gtype = pGeometryFactory->DecodeGeometryType(type);

					FeatureStyle* pFStyle = static_cast<FeatureStyle*>(pStyle);
					pFStyle->SetGeometryType(gtype);
				}
				break;
			case augeStyleRaster:
				break;
			}
		}
		pResult->Release();

		return pStyle;
	}

	Style* CartoManagerImpl::GetStyle(const char* name, FeatureClass* pFeatureClass)
	{
		if(name==NULL)
		{
			return NULL;
		}

		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select gid,s_text,s_type from g_style where s_name='%s'", name);
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
		const char* type = pResult->GetString(0,2);

		Style* pStyle = NULL;
		StyleReaderImpl reader;
		pStyle = reader.Read(text, strlen(text), pFeatureClass);
		pResult->Release();

		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
			pStyle->SetName(name);

			switch(pStyle->GetType())
			{
			case augeStyleFeature:
				{
					GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
					augeGeometryType gtype = pGeometryFactory->DecodeGeometryType(type);

					FeatureStyle* pFStyle = static_cast<FeatureStyle*>(pStyle);
					pFStyle->SetGeometryType(gtype);
				}
				break;
			case augeStyleRaster:
				break;
			}
		}

		return pStyle;
	}

	char* CartoManagerImpl::GetStyleText(g_uint id)
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
		const char* text = pResult->GetString(0,1);
		char* result = strdup(text);

		pResult->Release();
		return result;
	}

	char* CartoManagerImpl::GetStyleText(const char* name)
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
		const char* text = pResult->GetString(0,1);
		char* result = strdup(text);

		pResult->Release();
		return result;
	}

	g_int CartoManagerImpl::CreateStyle(const char* name, Style* pStyle, augeGeometryType type)
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
			return rc;
		}

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		const char* gtype = pGeometryFactory->Encode(type);

		std::string sql;
		sql.append("insert into g_style (s_name, s_text,s_type) values('");
		sql.append(name);
		sql.append("','");
		sql.append((char*)buffer);
		sql.append("','");
		sql.append(gtype);
		sql.append("') returning gid");

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			pxDoc->Release();
			return rc;
		}

		g_uint gid = pResult->GetInt(0,0);

		pResult->Release();
		pxDoc->Release();

		return gid;
	}

	RESULTCODE CartoManagerImpl::CreateStyle(const char* name, const char* text, augeGeometryType type)
	{
		if(name==NULL||text==NULL)
		{
			return AG_FAILURE;
		}

		if(!IsValiad(text))
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("Invalid style xml document");
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

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		const char* gtype = pGeometryFactory->Encode(type);

		std::string sql;
		sql.append("insert into g_style (s_name, s_text, s_type) values('");
		sql.append(name);
		sql.append("','");
		sql.append((char*)text);
		sql.append("','");
		sql.append(gtype);
		sql.append("') returning gid");

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			return -1;
		}

		g_uint gid = pResult->GetInt(0,0);

		pResult->Release();

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
			return rc;
		}

		//std::string sql;
		//sql.append("update g_style set s_text='");
		//sql.append((char*)buffer);
		//sql.append("' where s_name='");
		//sql.append(name);
		//sql.append("'");

		//rc = m_pConnection->ExecuteSQL(sql.c_str());
		//pxDoc->Release();

		//return (rc==AG_SUCCESS);

		rc = UpdateStyle(name, (char*)buffer);
		pxDoc->Release();

		return rc;
	}

	RESULTCODE CartoManagerImpl::UpdateStyle(const char* name, const char* text)
	{	
		if(name==NULL||text==NULL)
		{
			return AG_FAILURE;
		}

		std::string sql;
		sql.append("select gf_style_update(1,'");
		sql.append(name);
		sql.append("','");
		sql.append(text);
		sql.append("')");

		return m_pConnection->ExecuteSQL(sql.c_str());
	}

	//RESULTCODE CartoManagerImpl::UpdateStyle(const char* name, const char* text)
	//{
	//	if(name==NULL||text==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	std::string sql;
	//	sql.append("update g_style set s_text='");
	//	sql.append(text);
	//	sql.append("' where s_name='");
	//	sql.append(name);
	//	sql.append("'");

	//	return m_pConnection->ExecuteSQL(sql.c_str());
	//}

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

	g_int CartoManagerImpl::GetStyleID(const char* name)
	{
		if(name==NULL)
		{
			return -1;
		}
		g_int styleID = -1;
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select gid from g_style where s_name='%s'", name);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}
		if(!pResult->GetCount())
		{
			return -1;
		}

		styleID = pResult->GetInt(0,0);
		pResult->Release();

		return styleID;
	}

	bool CartoManagerImpl::IsStyleUpdated(Style* pStyle)
	{
		if(pStyle==NULL)
		{
			return false;
		}

		return false;
	}

	bool CartoManagerImpl::IsValiad(const char* text)
	{
		XParser parser;
		XDocument* pxDoc = parser.ParseMemory(text);
		if(pxDoc==NULL)
		{
			return false;
		}
		pxDoc->Release();
		return true;
	}

	EnumColorMap* CartoManagerImpl::GetColorMaps()
	{
		EnumColorMapImpl* pMaps = new EnumColorMapImpl();
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		Renderer* pRenderer = pCartoFactory->CreateRenderer2D(200,20);

		const char* sql = "select gid, s_color, e_color from g_color_map";
		GResultSet* pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet!=NULL)
		{
			int r,g,b,a;
			GColor s_color,e_color;

			char i_name[AUGE_NAME_MAX];
			char i_path[AUGE_PATH_MAX];
			char c_dir[AUGE_PATH_MAX];
			char m_dir[AUGE_PATH_MAX];
			auge_get_cwd(c_dir, AUGE_PATH_MAX);
#ifdef WIN32
			auge_make_path(m_dir, NULL, c_dir, "colormap", NULL);
#else
			char p_dir[AUGE_PATH_MAX];
			auge_get_parent_dir(c_dir, p_dir, AUGE_PATH_MAX);
			auge_make_path(m_dir, NULL, p_dir, "colormap", NULL);
#endif
			auge_mkdir(m_dir);

			int count = pResultSet->GetCount();
			for(int i=0; i<count; i++)
			{
				int gid = pResultSet->GetInt(i,0);
				const char* s_c = pResultSet->GetString(i,1);
				const char* e_c = pResultSet->GetString(i,2);				
				sscanf(s_c, "#%2x%2x%2x%2x",&r,&g,&b,&a);
				s_color.Set(r,g,b,a);
				sscanf(e_c, "#%2x%2x%2x%2x",&r,&g,&b,&a);
				e_color.Set(r,g,b,a);
				ColorMapImpl* pColorMap = new ColorMapImpl();
				pColorMap->Create(gid, s_color, e_color);
				pMaps->Add(pColorMap);

				//sprintf(i_name, "colormap/%d.png", gid);
				sprintf(i_name, "%d.png", gid);
				auge_make_path(i_path, NULL, m_dir, i_name, NULL);				
				pColorMap->SetImagePath(i_name);
				pColorMap->SetAbsoluteImagePath(i_path);

				pRenderer->DrawColorMap(pColorMap);
				pRenderer->SaveAsImage(i_path);
			}

			pResultSet->Release();
		}
		pRenderer->Release();
		return pMaps;
	}

	ColorMap* CartoManagerImpl::GetColorMap(g_int id)
	{
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select gid, s_color, e_color from g_color_map where gid=%d",id);
		GResultSet* pResultSet = m_pConnection->ExecuteQuery(sql);
		if(pResultSet==NULL)
		{
			return NULL;
		}

		int count = pResultSet->GetCount();
		if(count==0)
		{
			pResultSet->Release();
			return NULL;
		}

		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		Renderer* pRenderer = pCartoFactory->CreateRenderer2D(200,20);

		int r,g,b,a;
		GColor s_color,e_color;

		char i_name[AUGE_NAME_MAX];
		char i_path[AUGE_PATH_MAX];
		char c_dir[AUGE_PATH_MAX];
		char m_dir[AUGE_PATH_MAX];
		auge_get_cwd(c_dir, AUGE_PATH_MAX);
#ifdef WIN32
		auge_make_path(m_dir, NULL, c_dir, "colormap", NULL);
#else
		char p_dir[AUGE_PATH_MAX];
		auge_get_parent_dir(c_dir, p_dir, AUGE_PATH_MAX);
		auge_make_path(m_dir, NULL, p_dir, "colormap", NULL);
#endif
		auge_mkdir(m_dir);

		int i=0;		
		int gid = pResultSet->GetInt(i,0);
		const char* s_c = pResultSet->GetString(i,1);
		const char* e_c = pResultSet->GetString(i,2);				
		sscanf(s_c, "#%2x%2x%2x%2x",&r,&g,&b,&a);
		s_color.Set(r,g,b,a);
		sscanf(e_c, "#%2x%2x%2x%2x",&r,&g,&b,&a);
		e_color.Set(r,g,b,a);
		ColorMapImpl* pColorMap = new ColorMapImpl();
		pColorMap->Create(gid, s_color, e_color);

		sprintf(i_name, "colormap/%d.png", gid);
		auge_make_path(i_path, NULL, m_dir, i_name, NULL);				
		pColorMap->SetImagePath(i_name);
		pColorMap->SetAbsoluteImagePath(i_path);

		pRenderer->DrawColorMap(pColorMap);
		pRenderer->SaveAsImage(i_path);

		pResultSet->Release();
		pRenderer->Release();

		return pColorMap;
	}



















	//////////////////////////////////////////////////////////////////////////
	// Map Operation With User Begin
	//////////////////////////////////////////////////////////////////////////
	g_uint CartoManagerImpl::GetCount(g_uint user)
	{
		if(user<=0)
		{
			return 0;
		}
		if(m_pConnection==NULL)
		{
			return 0;
		}
		const char* sql = "select count(*) from g_map m, g_user u where m.user_id = u.gid";
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = atoi(pResult->GetString(0,0));
		pResult->Release();
		return count;
	}

	Map* CartoManagerImpl::GetMap(g_uint user_id, const char* mapName)
	{
		if(mapName==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select gid,m_name,m_uri,version,minx,miny,maxx,maxy,srid,thumbnail,v_minx,v_miny,v_maxx,v_maxy from g_map where m_name='%s' and user_id=%d", mapName, user_id);

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

		Map* pMap = CreateMap(pResult, 0);

		pResult->Release();

		return pMap;
	}

	EnumMap* CartoManagerImpl::GetMaps(g_uint user)
	{
		if(m_pConnection==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX];
		sprintf(sql, "select gid,m_name,m_uri,version,minx,miny,maxx,maxy,srid,thumbnail,v_minx,v_miny,v_maxx,v_maxy from g_map where user_id=%d", user);
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumMapImpl* pEnumMap = new EnumMapImpl();
		int count = pResult->GetCount();
		for(int i=0; i<count; i++)
		{
			Map* pMap = CreateMap(pResult, i);
			if(pMap!=NULL)
			{
				pEnumMap->Add(pMap);
			}
		}

		pResult->Release();
		return pEnumMap;
	}

	//Map* CartoManagerImpl::LoadMap(g_uint user_id, const char* mapName)
	//{
	//	if(mapName==NULL || m_pConnection==NULL)
	//	{
	//		return NULL;
	//	}

	//	Map* pMap = GetMap(user_id, mapName);
	//	if(pMap==NULL)
	//	{
	//		return NULL;
	//	}
	//	g_int mid = pMap->GetID();
	//	if(mid<0)
	//	{
	//		return NULL;
	//	}

	//	char sql[AUGE_SQL_MAX] = {0};
	//	g_snprintf(sql, AUGE_SQL_MAX, "select gid,l_name,l_type,f_name,d_id,s_id,version,visible,r_b,w_b,q_b,web_url from g_layer where m_id=%d order by gid", mid);

	//	GResultSet* pResult = NULL;
	//	pResult = m_pConnection->ExecuteQuery(sql);
	//	if(pResult==NULL)
	//	{
	//		return NULL;
	//	}

	//	Layer* pLayer = NULL;
	//	int gid, d_id,s_id,l_type,version,visible;
	//	const char* l_name, *f_name,*web_url;
	//	g_int count = pResult->GetCount();
	//	for(g_int i=0; i<count; i++)
	//	{
	//		gid = pResult->GetInt(i, 0);
	//		l_name = pResult->GetString(i, 1);
	//		l_type = pResult->GetInt(i, 2);
	//		f_name = pResult->GetString(i, 3);
	//		d_id   = pResult->GetInt(i, 4);
	//		s_id   = pResult->GetInt(i, 5);
	//		version= pResult->GetInt(i, 6);
	//		visible= pResult->GetInt(i, 7);
	//		web_url= pResult->GetString(i,11);

	//		pLayer = CreateLayer(gid, l_name, (augeLayerType)l_type, f_name, d_id, s_id, version, visible, web_url);
	//		if(pLayer!=NULL)
	//		{
	//			pMap->AddLayer(pLayer);
	//			//switch(pLayer->GetType())
	//			//{
	//			//case augeLayerFeature:
	//			//	{
	//			//		FeatureLayer* pFeatureLayer = static_cast<FeatureLayer*>(pLayer);
	//			//		FeatureClass* pFeatureClass = pFeatureLayer->GetFeatureClass();
	//			//		Style* pStyle = GetStyle(s_id, pFeatureClass);
	//			//		pFeatureLayer->SetStyle(pStyle);
	//			//	}
	//			//	break;
	//			//case augeLayerRaster:
	//			//	break;
	//			//}
	//		}
	//	}
	//	pResult->Release();
	//	return pMap;
	//}

	Map* CartoManagerImpl::LoadMap(g_uint user_id, const char* name)
	{
		if(name==NULL || m_pConnection==NULL)
		{
			return NULL;
		}

		Map* pMap = GetMap(user_id, name);
		if(pMap==NULL)
		{
			return NULL;
		}
		g_int mid = pMap->GetID();
		if(mid<0)
		{
			return NULL;
		}

		LoadLayers(pMap);

		return pMap;

		//return LoadMap(mid);
	}

	Map* CartoManagerImpl::CreateMap(g_uint user_id, const char* name)
	{
		if(name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_map (m_name, user_id) values('%s',%d) returning gid", name, user_id);

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

	Map* CartoManagerImpl::CreateMap(g_uint user_id, const char* name, g_uint srid, double xmin, double ymin, double xmax, double ymax)
	{
		if(name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_map (m_name, minx, miny, maxx, maxy,srid,user_id) values('%s',%f,%f,%f,%f,%d,%d) returning gid", name, xmin, ymin, xmax, ymax,srid,user_id);

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

	RESULTCODE CartoManagerImpl::RemoveMap(g_uint user_id, const char* name)
	{
		if( name == NULL )
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "delete from g_map where m_name='%s' and user_id=%d", name,user_id);
		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);
		if(rc==AG_SUCCESS)
		{

		}
		return rc;
	}

	RESULTCODE CartoManagerImpl::SaveMap(g_uint user_id, const char* name, g_uint srid, GEnvelope& extent, GEnvelope& viewer, const char* layer_ids)
	{
		if(name==NULL||layer_ids==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		g_snprintf(sql, AUGE_SQL_MAX, "update g_map set   minx=%f,  miny=%f,  maxx=%f,  maxy=%f," \
													   "v_minx=%f,v_miny=%f,v_maxx=%f,v_maxy=%f," \
													   "m_layers='%s', srid=%d " \
													   "where m_name='%s'",\
													   extent.m_xmin, extent.m_ymin, extent.m_xmax, extent.m_ymax,
													   viewer.m_xmin, viewer.m_ymin, viewer.m_xmax, viewer.m_ymax,
													   layer_ids,
													   srid,
													   name);
		return m_pConnection->ExecuteSQL(sql);
	}

	g_int CartoManagerImpl::GetMapID(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return -1;
		}
		g_int mapID = -1;
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select gid from g_map where m_name='%s' and %d", name, user_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}
		if(!pResult->GetCount())
		{
			return -1;
		}

		mapID = pResult->GetInt(0,0);
		pResult->Release();

		return mapID;
	}

	Layer* CartoManagerImpl::CreateLayer(g_uint user_id, const char* name, augeLayerType type, const char* f_name, g_uint map_id, g_uint source_id, g_uint style_id)
	{
		if(name==NULL||f_name==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		if(HasLayer(name, map_id))
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Layer [%s] already exists.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			return NULL;
		}

		Layer* pLayer = NULL;
		switch(type)
		{
		case augeLayerFeature:
			pLayer = CreateFeatureLayer(name, f_name, source_id);
			break;
		case augeLayerGraphic:
			pLayer = CreateGraphicLayer(name, f_name, source_id);
			break;
		case augeLayerRaster:
			break;
		}

		if(pLayer==NULL)
		{
			return NULL;
		}


		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_layer (l_name,l_type,f_name,m_id,d_id,s_id) values('%s',%d,'%s',%d,%d,%d) returning gid", name, type, f_name, map_id, source_id,style_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		int gid = pResult->GetInt(0,0);
		pResult->Release();
		pLayer->SetID(gid);

		return pLayer;
	}

	Layer* CartoManagerImpl::CreateWebLayer(g_uint user_id, const char* name, augeLayerType type, const char* url, g_uint map_id)
	{
		if(name==NULL||url==NULL||m_pConnection==NULL)
		{
			return NULL;
		}

		if(HasLayer(name, map_id))
		{
			char msg[AUGE_MSG_MAX] = {0};
			g_sprintf(msg, "Layer [%s] already exists.", name);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			return NULL;
		}

		WebLayer* pWebLayer = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		switch(type)
		{
		case augeLayerWMS:
			break;
		case augeLayerWMTS:
			break;
		case augeLayerWFS:
			break;
		case augeLayerQuadServer:
			pWebLayer = pCartoFactory->CreateQuadServerLayer();
			break;
		}
		if(pWebLayer==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into g_layer (l_name,l_type,f_name,m_id,d_id,s_id, web_url, user_id) values('%s',%d,'%s',%d,%d,%d,'%s',%d) returning gid", name, type, "", map_id, "", "", url, user_id);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		int gid = pResult->GetInt(0,0);
		pResult->Release();

		pWebLayer->SetID(gid);
		return pWebLayer;
	}

	Layer* CartoManagerImpl::CreateLayer(g_uint user_id, int id, const char* name, augeLayerType type, const char* f_name, g_int source_id, g_int style_id, g_int version, bool visible, const char* web_url)
	{
		Layer* pLayer = NULL;
		//switch(type)
		//{
		//case augeLayerFeature:
		//	pLayer = CreateFeatureLayer(user, id, name, f_name, source_id, style_id, version, visible);
		//	break;
		//case augeLayerGraphic:
		//	pLayer = CreateGraphicLayer(user, id, name, f_name, source_id, style_id, version, visible);
		//	break;
		//case augeLayerRaster:
		//	break;
		//case augeLayerQuadServer:
		//	pLayer = CreateQuadServerLayer(user, id, name, web_url, version, visible);
		//}

		return pLayer;
	}

	FeatureLayer* CartoManagerImpl::CreateFeatureLayer(g_uint user_id, int id, const char* name, const char* f_name, g_int source_id, g_int style_id, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance(); 

		FeatureWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(source_id));
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

		FeatureLayer* pFLayer = pCartoFactory->CreateFeatureLayer();
		if(pFLayer==NULL)
		{
			pFeatureClass->Release();
			return NULL;
		}
		pFLayer->SetName(name);
		pFLayer->SetID(id);
		pFLayer->SetVersion(version);
		pFLayer->SetVisiable(visible);
		pFLayer->SetFeatureClass(pFeatureClass);

		Style* pStyle = GetStyle(style_id, pFeatureClass);
		pFLayer->SetStyle(pStyle);

		return pFLayer;
	}

	GraphicLayer* CartoManagerImpl::CreateGraphicLayer(g_uint user_id, int id, const char* name, const char* f_name, g_int source_id, g_int style_id, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance(); 

		FeatureWorkspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = dynamic_cast<FeatureWorkspace*>(pConnManager->GetWorkspaceById(source_id));
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

		GraphicLayer* pGLayer = pCartoFactory->CreateGraphicLayer();
		if(pGLayer==NULL)
		{
			pFeatureClass->Release();
			return NULL;
		}
		pGLayer->SetName(name);
		pGLayer->SetID(id);
		pGLayer->SetVersion(version);
		pGLayer->SetVisiable(visible);
		pGLayer->SetFeatureClass(pFeatureClass);

		Style* pStyle = GetStyle(style_id, pFeatureClass);
		pGLayer->SetStyle(pStyle);

		return pGLayer;
	}

	QuadServerLayer* CartoManagerImpl::CreateQuadServerLayer(g_uint user_id, int id, const char* name, const char* url, int version, bool visible)
	{
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance(); 

		QuadServerLayer* pLayer = pCartoFactory->CreateQuadServerLayer();
		if(pLayer==NULL)
		{
			return NULL;
		}

		pLayer->SetName(name);
		pLayer->SetID(id);
		pLayer->SetVersion(version);
		pLayer->SetVisiable(visible);
		pLayer->SetURL(url);
		return pLayer;
	}

	//////////////////////////////////////////////////////////////////////////
	bool CartoManagerImpl::HasStyle(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select count(*) from g_style where s_name='%s' and user_id=%d", name, user_id);
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = pResult->GetInt(0,0);
		pResult->Release();
		return (count>0);
	}

	g_uint CartoManagerImpl::GetStyleCount(g_uint user_id)
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select count(*) g_style g_map where user_id=%d", user_id);
		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		g_uint count = pResult->GetInt(0,0);
		pResult->Release();
		return count; 
	}

	EnumStyle* CartoManagerImpl::GetStyles(g_uint user_id)
	{
		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select gid, s_name,s_type from g_style where user_id=%d", user_id);
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		FeatureStyleImpl* pStyle = NULL;
		StyleReaderImpl reader;
		EnumStyleImpl* pEnums = new EnumStyleImpl();
		g_int count = pResult->GetCount();
		for(g_int i=0; i<count; i++)
		{
			g_int id = pResult->GetInt(i,0);
			const char* name = pResult->GetString(i,1);
			const char* type = pResult->GetString(i,2);

			//pStyle = reader.Read(text, strlen(text), NULL);	
			pStyle = new FeatureStyleImpl();
			if(pStyle==NULL)
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("Bad Style XML Document", __FILE__,__LINE__);
				//pLogger->Debug(text);
				continue;
			}
			pStyle->SetID(id);
			pStyle->SetName(name);
			pEnums->Add(pStyle);

			switch(pStyle->GetType())
			{
			case augeStyleFeature:
				{
					GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
					augeGeometryType gtype = pGeometryFactory->DecodeGeometryType(type);

					FeatureStyle* pFStyle = static_cast<FeatureStyle*>(pStyle);
					pFStyle->SetGeometryType(gtype);
				}
				break;
			case augeStyleRaster:
				break;
			}
		}
		pResult->Release();

		return pEnums;
	}

	Style* CartoManagerImpl::GetStyle(g_uint user_id, const char* name, FeatureClass* pFeatureClass)
	{
		if(name==NULL)
		{
			return NULL;
		}

		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select gid,s_text,s_type from g_style where s_name='%s' and user_id=%d", name, user_id);
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
		const char* type = pResult->GetString(0,2);

		Style* pStyle = NULL;
		StyleReaderImpl reader;
		pStyle = reader.Read(text, strlen(text), pFeatureClass);
		pResult->Release();

		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
			pStyle->SetName(name);

			switch(pStyle->GetType())
			{
			case augeStyleFeature:
				{
					GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
					augeGeometryType gtype = pGeometryFactory->DecodeGeometryType(type);

					FeatureStyle* pFStyle = static_cast<FeatureStyle*>(pStyle);
					pFStyle->SetGeometryType(gtype);
				}
				break;
			case augeStyleRaster:
				break;
			}
		}

		return pStyle;
	}
	
	char* CartoManagerImpl::GetStyleText(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		GResultSet* pResult = NULL;
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX,"select gid,s_text from g_style where s_name='%s' and user_id=%d order by s_name", name, user_id);
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}
		if(!pResult->GetCount())
		{
			return NULL;
		}
		const char* text = pResult->GetString(0,1);
		char* result = strdup(text);

		pResult->Release();
		return result;
	}

	g_int CartoManagerImpl::CreateStyle(g_uint user_id, const char* name, Style* pStyle, augeGeometryType type)
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
			return rc;
		}

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		const char* gtype = pGeometryFactory->Encode(type);

		char str[AUGE_NAME_MAX];
		g_sprintf(str, "%d", user_id);

		std::string sql;
		sql.append("insert into g_style (s_name, s_text,s_type, user_id) values('");
		sql.append(name);
		sql.append("','");
		sql.append((char*)buffer);
		sql.append("','");
		sql.append(gtype);
		sql.append("','");
		sql.append(str);
		sql.append("') returning gid");

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			pxDoc->Release();
			return rc;
		}

		g_uint gid = pResult->GetInt(0,0);

		pResult->Release();
		pxDoc->Release();

		return gid;
	}

	RESULTCODE CartoManagerImpl::CreateStyle(g_uint user_id, const char* name, const char* text, augeGeometryType type)
	{
		if(name==NULL||text==NULL)
		{
			return AG_FAILURE;
		}

		if(!IsValiad(text))
		{
			GError* pError = augeGetErrorInstance();
			pError->SetError("Invalid style xml document");
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

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		const char* gtype = pGeometryFactory->Encode(type);

		char str[AUGE_NAME_MAX];
		g_sprintf(str, "%d", user_id);

		std::string sql;
		sql.append("insert into g_style (s_name, s_text, s_type, user_id) values('");
		sql.append(name);
		sql.append("','");
		sql.append((char*)text);
		sql.append("','");
		sql.append(gtype);
		sql.append("','");
		sql.append(str);
		sql.append("') returning gid");

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			return -1;
		}

		g_uint gid = pResult->GetInt(0,0);

		pResult->Release();

		return gid;
	}

	RESULTCODE CartoManagerImpl::UpdateStyle(g_uint user_id, const char* name, Style* pStyle)
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
			return rc;
		}

		//std::string sql;
		//sql.append("update g_style set s_text='");
		//sql.append((char*)buffer);
		//sql.append("' where s_name='");
		//sql.append(name);
		//sql.append("'");

		//rc = m_pConnection->ExecuteSQL(sql.c_str());
		//pxDoc->Release();

		//return (rc==AG_SUCCESS);

		rc = UpdateStyle(user_id, name, (char*)buffer);
		pxDoc->Release();

		return rc;
	}

	RESULTCODE CartoManagerImpl::UpdateStyle(g_uint user_id, const char* name, const char* text)
	{	
		if(name==NULL||text==NULL)
		{
			return AG_FAILURE;
		}

		char str[AUGE_NAME_MAX];
		g_sprintf(str, "%d", user_id);

		std::string sql;
		sql.append("select gf_style_update(1,'");
		sql.append(name);
		sql.append("','");
		sql.append(text);
		sql.append("','");
		sql.append(str);
		sql.append("')");

		return m_pConnection->ExecuteSQL(sql.c_str());
	}

	//RESULTCODE CartoManagerImpl::UpdateStyle(const char* name, const char* text)
	//{
	//	if(name==NULL||text==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	std::string sql;
	//	sql.append("update g_style set s_text='");
	//	sql.append(text);
	//	sql.append("' where s_name='");
	//	sql.append(name);
	//	sql.append("'");

	//	return m_pConnection->ExecuteSQL(sql.c_str());
	//}

	RESULTCODE CartoManagerImpl::RemoveStyle(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "delete from g_style where s_name='%s'", name);

		return m_pConnection->ExecuteSQL(sql);
	}

	g_int CartoManagerImpl::GetStyleID(g_uint user_id, const char* name)
	{
		if(name==NULL)
		{
			return -1;
		}
		g_int styleID = -1;
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "select gid from g_style where s_name='%s'", name);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return -1;
		}
		if(!pResult->GetCount())
		{
			return -1;
		}

		styleID = pResult->GetInt(0,0);
		pResult->Release();

		return styleID;
	}

	bool CartoManagerImpl::IsStyleUpdated(g_uint user_id, Style* pStyle)
	{
		if(pStyle==NULL)
		{
			return false;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	// Map Operation With User End
	//////////////////////////////////////////////////////////////////////////


	
	//////////////////////////////////////////////////////////////////////////
	RESULTCODE CartoManagerImpl::CreateMapTable()
	{
		const char* sql =   "CREATE TABLE g_map(" \
							"  gid serial NOT NULL," \
							"  user_id integer NOT NULL DEFAULT 1," \
							"  m_name character varying(32) NOT NULL," \
							"  m_layers character varying(4096)," \
							"  m_uri  character varying(128) NOT NULL," \
							"  srid integer DEFAULT 4326," \
							"  version integer DEFAULT 1," \
							"  minx double precision DEFAULT -180," \
							"  maxx double precision DEFAULT -90," \
							"  miny double precision DEFAULT  180," \
							"  maxy double precision DEFAULT  90," \
							"  v_minx double precision DEFAULT -180," \
							"  v_maxx double precision DEFAULT -90," \
							"  v_miny double precision DEFAULT  180," \
							"  v_maxy double precision DEFAULT  90," \
							"  m_uuid character varying(128) DEFAULT ''::character varying," \
							"  CONSTRAINT g_map_pkey PRIMARY KEY (gid )," \
							"  CONSTRAINT g_map_user_fk FOREIGN KEY (user_id)" \
							"  REFERENCES g_user (gid) MATCH SIMPLE " \
							"  ON UPDATE NO ACTION ON DELETE NO ACTION," \
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
							"	l_name character varying(32) NOT NULL," \
							"	l_type integer NOT NULL DEFAULT 0," \
							"	f_name character varying(32) NOT NULL," \
							"	f_path character varying(256) DEFAULT ''::character varying," \
							"	user_id integer NOT NULL DEFAULT 1," \
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
							"	CONSTRAINT g_layer_user_fkey FOREIGN KEY (user_id)" \
							"	REFERENCES g_user (gid) MATCH SIMPLE" \
							"	ON UPDATE NO ACTION ON DELETE NO ACTION" \
							/*"	CONSTRAINT g_layer_s_id_fkey FOREIGN KEY (s_id) " \
							/"	REFERENCES g_style (gid) MATCH SIMPLE " \
							/"	ON UPDATE NO ACTION ON DELETE NO ACTION," \
							"	CONSTRAINT g_layer_l_name_key UNIQUE (l_name ) " \*/
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
			"	s_text text NOT NULL," \
			"	s_type character varying(16) NOT NULL," \
			"	version integer DEFAULT 1," \
			"	user_id integer NOT NULL DEFAULT 1," \
			"   CONSTRAINT g_style_pkey PRIMARY KEY (gid)" \
			"	CONSTRAINT g_style_user_fkey FOREIGN KEY (user_id)" \
			"		REFERENCES g_user (gid) MATCH SIMPLE" \
			"		ON UPDATE NO ACTION ON DELETE NO ACTION" \
			")";

		RESULTCODE rc = AG_SUCCESS;
		rc = m_pConnection->ExecuteSQL(sql);
		return (rc == AG_SUCCESS);
	}

	RESULTCODE CartoManagerImpl::CreateColorMapTable()
	{
		const char* sql =   
			"CREATE TABLE g_color_map" \
			"(" \
			"	gid serial NOT NULL," \
			"	s_color character varying(16)," \
			"	e_color character varying(16)," \
			"	CONSTRAINT g_color_map_pkey PRIMARY KEY (gid)" \
			")";
		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);
		return (rc==AG_SUCCESS);
	}

	RESULTCODE CartoManagerImpl::InitColorMap()
	{
		const char* color_map[] = {"#FF0000FF", "#00FF00FF"};
		size_t count = sizeof(color_map) / sizeof(const char*) / 2;
		const char* format = "insert into g_color_map(s_color, e_color) values('%s','%s')";
		char sql[AUGE_SQL_MAX];
		for(int i=0; i<count; i+=2)
		{
			g_snprintf(sql, AUGE_SQL_MAX, format, color_map[i],color_map[i+1]);
			m_pConnection->ExecuteSQL(sql);
		}
		return AG_SUCCESS;
	}

	//CREATE TABLE g_layer(gid serial NOT NULL,type integer NOT NULL,version integer,l_name character varying(32) NOT NULL,f_name character varying(32) NOT NULL,s_id integer NOT NULL,m_id integer,CONSTRAINT g_layer_m_id_fkey FOREIGN KEY (m_id)REFERENCES g_map (gid) MATCH SIMPLEON UPDATE NO ACTION ON DELETE NO ACTION,CONSTRAINT g_layer_s_id_fkey FOREIGN KEY (s_id)REFERENCES g_data_source (gid) MATCH SIMPLEON UPDATE NO ACTION ON DELETE NO ACTION,CONSTRAINT g_layer_l_name_key UNIQUE (l_name))


	//////////////////////////////////////////////////////////////////////////
	RESULTCODE CartoManagerImpl::SetStyle(g_uint layerID, g_uint styleID)
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "update g_layer set s_id=%d where gid=%d", styleID, layerID);
		return m_pConnection->ExecuteSQL(sql);
	}
}
