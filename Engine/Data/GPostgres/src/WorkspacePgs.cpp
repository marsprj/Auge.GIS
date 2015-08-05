#include "WorkspacePgs.h"
#include "FeatureClassPgs.h"
#include "DataEngineImpl.h"
#include "SQLBuilder.h"
#include "EnumDataSetImpl.h"
#include "RasterDatasetImpl.h"
#include "AttributeDataSetPgs.h"

#include "RasterFolderImpl.h"

namespace auge
{	
	WorkspacePgs::WorkspacePgs():
	m_schema("public")
	{
		g_feature_catalog_table = "g_feature_catalog";
		g_raster_table  = "g_raster";
		g_raster_folder_table = "g_raster_folder";


//#ifdef WIN32
//		m_raster_repository = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload";
//#else
//		m_raster_repository = "/opt/auge/upload";
//#endif
	}

	WorkspacePgs::~WorkspacePgs()
	{
	}

	DataEngine*	WorkspacePgs::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspacePgs::GetName()
	{
		return m_name.c_str();
	}

	void WorkspacePgs::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspacePgs::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		GPropertySet props;
		props.Parse(conn_string);
		const char* repository = props.GetValue(AUGE_DB_REPOSITORY);
		if(repository==NULL)
			m_raster_repository.clear();
		else
			m_raster_repository = repository;

		return m_pgConnection.SetConnectionString(conn_string);
	}
	
	const char*	WorkspacePgs::GetConnectionString()
	{
		return m_pgConnection.GetConnectionString();
	}
	
	RESULTCODE WorkspacePgs::Open()
	{	
		if(m_pgConnection.IsOpen())
		{
			return AG_SUCCESS;
		}
		RESULTCODE rc = m_pgConnection.Open();
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		if(!m_pgConnection.HasTable(g_feature_catalog_table.c_str()))
		{
			CreateFeatureCatalogTable();
		}
		
		if(!m_pgConnection.HasTable(g_raster_folder_table.c_str()))
		{
			CreateRasterFolderTable();
		}

		if(!m_pgConnection.HasTable(g_raster_table.c_str()))
		{
			CreateRasterTable();
		}
		m_raster_root_folder.Create(0, "/", "/", "/", this);
		return rc;
	}

	void WorkspacePgs::Close()
	{
		m_pgConnection.Close();
	}

	bool WorkspacePgs::IsOpen()
	{
		return m_pgConnection.IsOpen();
	}

	FeatureClass* WorkspacePgs::OpenFeatureClass(const char* name)
	{
		if((name==NULL)||(!m_pgConnection.IsOpen()))
		{
			return NULL;
		}

		FeatureClassPgs* pFeatureClass = new FeatureClassPgs();
		if(!pFeatureClass->Create(name, this))
		{
			pFeatureClass->Release();
			pFeatureClass = NULL;
		}

		return pFeatureClass;
	}

	//FeatureClass* WorkspacePgs::CreateFeatureClass(const char* name, GFields* pFields)
	//{
	//	if(name==NULL||pFields==NULL)
	//	{
	//		return NULL;
	//	}
	//	if(pFields->Count()==0)
	//	{
	//		return NULL;
	//	}
	//	GField* pField = pFields->GetGeometryField();
	//	if(pField==NULL)
	//	{
	//		return NULL;
	//	}

	//	RESULTCODE rc = AG_FAILURE;
	//	rc = CreateTable(name, pFields);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		return NULL;
	//	}
	//	rc = AddGeometryColumn(name, pField);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		RemoveTable(name);
	//		return NULL;
	//	}

	//	return OpenFeatureClass(name);
	//}

	FeatureClass* WorkspacePgs::CreateFeatureClass(const char* name, GFields* pFields)
	{
		if(name==NULL||pFields==NULL)
		{
			return NULL;
		}
		if(pFields->Count()==0)
		{
			return NULL;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = CreateTable(name, pFields);
		if(rc!=AG_SUCCESS)
		{
			return NULL;
		}
		GField* pField = pFields->GetGeometryField();
		if(pField!=NULL)
		{
			rc = AddGeometryColumn(name, pField);
			if(rc!=AG_SUCCESS)
			{
				RemoveTable(name);
				return NULL;
			}
		}

		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspacePgs::RemoveFeatureClass(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_SUCCESS;
		rc = RemoveTable(name);
		if(rc!=AG_SUCCESS)
		{
			return AG_SUCCESS;
		}

		return UnRegiseterGeometryColumn(name);
	}

	EnumDataSet* WorkspacePgs::GetFeatureClasses()
	{
		const char* sql = "select f_table_name from geometry_columns";

		PGresult* pgResult = m_pgConnection.PgExecute(sql);
		if(pgResult==NULL)
		{
			return NULL;
		}

		const char* className = NULL;
		FeatureClass* pFeatureClass = NULL;	
		EnumDataSetImpl *pEnum = new EnumDataSetImpl();

		int nTuples = PQntuples(pgResult);
		for(int i=0;i<nTuples; i++)
		{
			className = PQgetvalue(pgResult, i, 0);
			pFeatureClass = OpenFeatureClass(className);
			if(pFeatureClass!=NULL)
			{
				pEnum->Add(pFeatureClass);
			}
		}

		PQclear(pgResult);

		return pEnum;
	}

	EnumDataSet* WorkspacePgs::GetDataSets()
	{
		EnumDataSetImpl* pEnumDataset = (EnumDataSetImpl*)(GetFeatureClasses());

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select tablename from pg_tables where schemaname='%s'",m_schema.c_str());

		PGresult* pgResult = m_pgConnection.PgExecute(sql);
		if(pgResult!=NULL)
		{	
			const char* tname = NULL;
			DataSet* pDataset = NULL;
			g_uint count = PQntuples(pgResult);
			for(g_uint i=0; i<count; i++)
			{
				tname = PQgetvalue(pgResult,i,0);
				if((g_stricmp(tname,"spatial_ref_sys")==0)||(g_stricmp(tname,"g_raster")==0))
				{
					continue;
				}

				bool found = false;
				
				pEnumDataset->Reset();
				while((pDataset=pEnumDataset->Next())!=NULL)
				{
					if(g_stricmp(tname,pDataset->GetName())==0)
					{
						found = true;
						break;
					}
				}
				if(!found)
				{
					AttributeDataSetPgs* pattrDataset = new AttributeDataSetPgs();
					pattrDataset->Create(tname, this, pgResult);
					pEnumDataset->Add(pattrDataset);
				}
			}
			
			PQclear(pgResult);
		}

		return pEnumDataset;
	}

	DataSet* WorkspacePgs::OpenDataSet(const char* name)
	{
		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspacePgs::RemoveDataSet(const char* name)
	{
		//return RemoveRasterRequest(name);
		return NULL;
	}

	RESULTCODE WorkspacePgs::RemoveRaster(const char* name, const char* path)
	{
		if(name==NULL||path==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "delete from g_raster where name='%s' and path='%s'",name, path);
		return m_pgConnection.ExecuteSQL(sql);
	}
	//////////////////////////////////////////////////////////////////////////
	// 
	//////////////////////////////////////////////////////////////////////////

	RESULTCODE WorkspacePgs::CreateTable(const char* name, GFields* pFields)
	{
		std::string sql;
		SQLBuilder::BuildCreateTable(sql, name, pFields);
		return m_pgConnection.ExecuteSQL(sql.c_str());
	}

	RESULTCODE WorkspacePgs::RemoveTable(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "drop table %s",name);
		return m_pgConnection.ExecuteSQL(sql);
	}


	bool WorkspacePgs::UnRegisterLayer(long lid)
	{
		//std::string strsql;
		//RESULTCODE rc = AG_SUCCESS;
		//AgSQLBuilder* pSQLBuilder = AgSQLBuilder::GetInstance();
		//rc = pSQLBuilder->BuildUnRegisterLayerSQL(strsql, lLayerID);
		//if(rc!=AG_SUCCESS)
		//{
		//	return false;
		//}
		//PGresult* pgResult = NULL;
		//pgResult = PgExecute(GetPgConnectionWrite(), strsql.c_str(), strsql.length());
		//if(pgResult==NULL)
		//{
		//	return false;
		//}
		//if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		//{
		//	PQclear(pgResult);
		//	return false;
		//}
		//PQclear(pgResult);
		return true;
	}

	//========================================================================
	// PostGIS geometry_columns ±í²Ù×÷
	//========================================================================
	//RESULTCODE WorkspacePgs::RegiseterGeometryColumn(const char* name, AgField* pGeomField)
	//{
	//	PGconn* pgConnection = GetPgConnectionWrite();
	//	if(pgConnection==NULL)
	//	{
	//		return AG_BAD_CONNECTION;
	//	}

	//	long srid = 0;
	//	const char* szLayerType = NULL;
	//	const char* szSpatialColumn = NULL;
	//	AgGeometryDef* pGeometryDef = pGeomField->GetGeometryDef();
	//	AgUtilPGS* pUtil = AgUtilPGS::GetInstance();

	//	srid		= pGeometryDef->GetSRID();
	//	szSpatialColumn = pGeomField->GetName();
	//	szLayerType = pUtil->GeometryTypeAsString(pGeometryDef->GeometryType());
	//	if(strcmp(szLayerType, "LINE")==0)
	//	{
	//		szLayerType = "LINESTRING";
	//	}

	//	const char* szDatabase = m_workspaceInfo.GetValue(AUGE_DB_DATABASE);
	//	const char* szUser     = m_workspaceInfo.GetValue(AUGE_DB_USER);

	//	if(szDatabase==NULL||szUser==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	RESULTCODE rc;
	//	std::string strsql;
	//	AgSQLBuilder *pSQLBuilder = AgSQLBuilder::GetInstance();
	//	rc = pSQLBuilder->BuildRegisterGeometryColumnSQL(strsql,
	//		"public",
	//		name, 
	//		szSpatialColumn,
	//		srid,
	//		2,
	//		szLayerType);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		return rc;
	//	}

	//	PGresult *pgResult = PgExecute(pgConnection, strsql.c_str(), strsql.length());
	//	if(pgResult==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	ExecStatusType status = PQresultStatus(pgResult);
	//	if(status!=PGRES_COMMAND_OK)
	//	{
	//		PQclear(pgResult);
	//		return AG_FAILURE;
	//	}

	//	return AG_SUCCESS;
	//}

	RESULTCODE WorkspacePgs::AddGeometryColumn(const char* name, GField* pField)
	{
		GeometryDef* pGeometryDef = pField->GetGeometryDef();
		if(pGeometryDef==NULL)
		{
			return AG_FAILURE;
		}

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		const char* fname = pField->GetName();
		int srid = pGeometryDef->GetSRID();
		int dimen= pGeometryDef->GetDimension();
		augeGeometryType type = pGeometryDef->GeometryType();
		const char* gtype = pGeometryFactory->Encode(pGeometryDef->GeometryType());


		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select addgeometrycolumn('%s','%s','%s',%d,'%s',%d)",
									  m_schema.c_str(),
									  name,
									  fname,
									  srid,
									  gtype,
									  dimen);
		return m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE WorkspacePgs::RegiseterGeometryColumn(const char* name, GField* pField)
	{
		GeometryDef* pGeometryDef = pField->GetGeometryDef();
		if(pGeometryDef==NULL)
		{
			return AG_FAILURE;
		}

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into geometry_columns (f_table_catalog, \
																	 f_table_schema, \
																	 f_table_name, \
																	 f_geometry_column, \
																	 coord_dimension,\
																	 srid,\
																	 type),\
																	 ('%s','%s','%s','%s',%d,%d,'%s')",
																	 m_pgConnection.GetDatabase(),
																	 m_schema.c_str(),
																	 name,
																	 pField->GetName(),
																	 2,
																	 pGeometryDef->GetSRID(),
																	 pGeometryFactory->Encode(pGeometryDef->GeometryType()));
		return m_pgConnection.ExecuteSQL(sql);

		return AG_SUCCESS;
	}

	RESULTCODE WorkspacePgs::UnRegiseterGeometryColumn(const char* name)
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "delete from geometry_columns where f_table_name='%s' and f_table_schema='%s'",name,m_schema.c_str());
		return m_pgConnection.ExecuteSQL(sql);
	}

	//////////////////////////////////////////////////////////////////////////
	// Raster Begin
	//////////////////////////////////////////////////////////////////////////
	const char*	WorkspacePgs::GetRepository()
	{
		return m_raster_repository.c_str();
	}

	RasterFolder* WorkspacePgs::GetRootFolder()
	{
		m_raster_root_folder.AddRef();
		return &m_raster_root_folder;
	}

	RasterFolder* WorkspacePgs::GetFolder(const char* path)
	{
		if(path==NULL)
		{
			return NULL;
		}

		if(strcmp(path, "/")==0)
		{
			return GetRootFolder();
		}

		const char* format = "select gid,name,alias,path,parent from %s where path='%s'";

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);		
		g_snprintf(sql, AUGE_PATH_MAX, format, g_raster_folder_table.c_str(), path);

		GResultSet* pResult = m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}
		
		g_uint count = pResult->GetCount();
		if(count==0)
		{
			pResult->Release();
			return NULL;
		}

		g_uint id = pResult->GetInt(0, 0);
		const char* name = pResult->GetString(0, 1);
		const char* alias= pResult->GetString(0, 2);
		//const char* path = pResult->GetString(0, 3);
		g_uint parent = pResult->GetInt(0, 4);

		RasterFolderImpl *pFolder = new RasterFolderImpl();
		pFolder->Create(id, name, alias, path, this);
		return pFolder;
	}

	RasterFolder* WorkspacePgs::GetFolder(g_uint id)
	{
		const char* format = "select gid,name,alias,path,parent from %s where gid=%d";

		char sql[AUGE_PATH_MAX];
		memset(sql, 0, AUGE_PATH_MAX);		
		g_snprintf(sql, AUGE_PATH_MAX, format, g_raster_folder_table.c_str(), id);

		GResultSet* pResult = m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}

		g_uint count = pResult->GetCount();
		if(count==0)
		{
			pResult->Release();
			return NULL;
		}

		const char* name = pResult->GetString(0, 1);
		const char* alias= pResult->GetString(0, 2);
		const char* path = pResult->GetString(0, 3);
		g_uint parent = pResult->GetInt(0, 4);

		RasterFolderImpl *pFolder = new RasterFolderImpl();
		pFolder->Create(id, name, alias, path, this);
		return pFolder;
	}

	Raster* WorkspacePgs::GetRaster(const char* path)
	{
		if(path==NULL)
		{
			return NULL;
		}
		char drv[AUGE_DRV_MAX];
		char dir[AUGE_PATH_MAX];
		char name[AUGE_NAME_MAX];
		char ext[AUGE_EXT_MAX];
		auge_split_path(path, drv, dir, name, ext);

		char folder_path[AUGE_PATH_MAX];
		memset(folder_path, 0, AUGE_PATH_MAX);
		auge_make_path(folder_path, drv, dir,NULL,NULL);
		size_t len = strlen(folder_path);
		if(len>1)
		{
			folder_path[len-1] = '\0';
		}

		char raster_name[AUGE_NAME_MAX];
		memset(raster_name, 0, AUGE_NAME_MAX);
		auge_make_path(raster_name, NULL, NULL, name, ext);

		RasterFolder* pFolder = GetFolder(folder_path);
		if(pFolder==NULL)
		{
			return NULL;
		}

		Raster* pRaster = pFolder->GetRasterDataset()->GetRaster(raster_name);

		pFolder->Release();
		return pRaster;
	}

	Raster*	WorkspacePgs::RemoveRaster(const char* path)
	{
		return NULL;
	}

	//EnumDataSet* WorkspacePgs::GetRasterDatasets()
	//{
	//	char sql[AUGE_SQL_MAX] = {0};
	//	const char* format = "select gid,name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid from %s";
	//	g_snprintf(sql, AUGE_SQL_MAX, format,	g_raster_table.c_str());

	//	GResultSet* pResult = m_pgConnection.ExecuteQuery(sql);
	//	if(pResult==NULL)
	//	{
	//		return NULL;
	//	}

	//	EnumDataSetImpl* pEnumDataset = new EnumDataSetImpl();
	//	g_uint count = pResult->GetCount();
	//	for(g_uint i=0; i<count; i++)
	//	{
	//		g_uint		gid	 = pResult->GetInt(i,0);
	//		const char* name = pResult->GetString(i,1);
	//		const char* alias= pResult->GetString(i,2);
	//		const char* fmt	 = pResult->GetString(i,3);
	//		const char* path = pResult->GetString(i,4);
	//		g_uint		nband= pResult->GetInt(i,5);
	//		g_int		srid = pResult->GetInt(i,6);
	//		g_uint		width= pResult->GetInt(i,7);
	//		g_uint		height=pResult->GetInt(i,8);
	//		double		xmin = pResult->GetDouble(i,9);
	//		double		ymin = pResult->GetDouble(i,10);
	//		double		xmax = pResult->GetDouble(i,11);
	//		double		ymax = pResult->GetDouble(i,12);
	//		const char* uuid = pResult->GetString(i,13);

	//		RasterDatasetImpl* pDataset = new RasterDatasetImpl();
	//		pDataset->Create(this,gid, name, alias, fmt, path, nband, srid, width, height, xmin, ymin, xmax, ymax, uuid);

	//		pEnumDataset->Add(pDataset);
	//	}

	//	pResult->Release();

	//	return pEnumDataset;
	//}

	//RasterDataset* WorkspacePgs::OpenRasterDataset(const char* name)
	//{
	//	if(name==NULL)
	//	{
	//		return NULL;
	//	}

	//	char sql[AUGE_SQL_MAX] = {0};
	//	const char* format = "select gid,name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid from %s where name='%s'";
	//	g_snprintf(sql, AUGE_SQL_MAX, format,	g_raster_table.c_str(), name);

	//	GResultSet* pResult = m_pgConnection.ExecuteQuery(sql);
	//	if(pResult==NULL)
	//	{
	//		return NULL;
	//	}

	//	if(pResult->GetCount()==0)
	//	{
	//		pResult->Release();
	//		return NULL;
	//	}

	//	g_uint		gid	 = pResult->GetInt(0,0);
	//	//const char* name = pResult->GetString(0,1);
	//	const char* alias= pResult->GetString(0,2);
	//	const char* fmt	 = pResult->GetString(0,3);
	//	const char* path = pResult->GetString(0,4);
	//	g_uint		nband= pResult->GetInt(0,5);
	//	g_int		srid = pResult->GetInt(0,6);
	//	g_uint		width= pResult->GetInt(0,7);
	//	g_uint		height=pResult->GetInt(0,8);
	//	double		xmin = pResult->GetDouble(0,9);
	//	double		ymin = pResult->GetDouble(0,10);
	//	double		xmax = pResult->GetDouble(0,11);
	//	double		ymax = pResult->GetDouble(0,12);
	//	const char* uuid = pResult->GetString(0,13);

	//	RasterDatasetImpl* pDataset = new RasterDatasetImpl();
	//	pDataset->Create(this,gid, name, alias, fmt, path, nband, srid, width, height, xmin, ymin, xmax, ymax, uuid);

	//	pResult->Release();

	//	return pDataset;
	//}

	EnumDataSet* WorkspacePgs::GetRasterDatasets()
	{
		return NULL;
	}

	RasterDataset* WorkspacePgs::CreateRasterDataset(const char* name)
	{
		return NULL;
	}
	
	RasterDataset* WorkspacePgs::OpenRasterDataset(const char* name)
	{
		return NULL;
	}

	RESULTCODE WorkspacePgs::RemoverRasterDataset(const char* name)
	{
		return AG_FAILURE;
	}

	EnumRaster*	WorkspacePgs::GetRasters()
	{
		return NULL;
	}

	Raster*	WorkspacePgs::OpenRaster(const char* name)
	{
		return NULL;
	}

	//RESULTCODE WorkspacePgs::RemoveRaster(const char* name)
	//{
	//	return AG_FAILURE;
	//}

	RESULTCODE WorkspacePgs::AddRaster(Raster* pRaster)
	{

		if(pRaster==NULL)
		{
			return AG_FAILURE;
		}
		char uuid[AUGE_PATH_MAX] = {0};
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		const char* path = pRaster->GetPath();
		const char* name = pRaster->GetName();
		const char* alias= pRaster->GetAlias();
		const char* fmt  = pRaster->GetFormat();
		
		g_int		srid = pRaster->GetSRID();
		g_uint		width= pRaster->GetWidth();
		g_uint		height=pRaster->GetHeight();
		g_uint		nband= pRaster->GetBandCount(); 
		GEnvelope& extent = pRaster->GetExtent();

		char raster_path[AUGE_PATH_MAX];
		memset(raster_path, 0, AUGE_PATH_MAX);
		auge_make_path(raster_path, NULL, path, name,NULL);
		if(alias==NULL)
		{
			alias = name;
		}
		char local_path[AUGE_PATH_MAX];
		memset(local_path, 0, AUGE_PATH_MAX);
		auge_make_path(local_path, NULL, GetRepository(), raster_path+1,NULL);
		//if(g_access(local_path, 4))
		{
			//pRaster->Save(local_path);
		}

		char sql[AUGE_SQL_MAX] = {0};
		const char* format = "insert into g_raster (name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid) values('%s','%s','%s','%s',%d,%d,%d,%d,%f,%f,%f,%f,'%s')";
		g_snprintf(sql, AUGE_SQL_MAX, format,	name,
												alias,
												fmt,
												path,//raster_path,
												nband,
												srid,
												width,
												height,
												extent.m_xmin,
												extent.m_ymin,
												extent.m_xmax,
												extent.m_ymax,
												uuid);
		return m_pgConnection.ExecuteSQL(sql);
	}
	
	RESULTCODE WorkspacePgs::CreateRasterTable()
	{
		char sql[AUGE_SQL_MAX];
		const char* format = "CREATE TABLE %s(" \
			"	gid serial NOT NULL," \
			"	name character varying(32)," \
			"	alias character varying(32)," \
			"	format character varying(8)," \
			"   dataset integer DEFAULT 0," \
			"	path character varying(256)," \
			"	band_count integer," \
			"	srid integer," \
			"	width integer DEFAULT 256," \
			"	height integer DEFAULT 256," \
			"	minx double precision," \
			"	miny double precision," \
			"	maxx double precision," \
			"	maxy double precision," \
			"	uuid character varying(128)," \
			"	isfile integer DEFAULT 0," \
			"	folder integer DEFAULT 0," \
			"	CONSTRAINT g_raster_pk PRIMARY KEY (gid)," \
			"	CONSTRAINT g_raster_name_uk UNIQUE (name, format)" \
			")";
		g_sprintf(sql, format, g_raster_table.c_str());
		return m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE WorkspacePgs::CreateRasterFolderTable()
	{
		char sql[AUGE_SQL_MAX];
		const char* format = "CREATE TABLE %s(" \
			"	gid serial NOT NULL," \
			"	name character varying(32)," \
			"	alias character varying(32)," \
			"	path character varying(256)," \
			"	parent integer," \
			"	CONSTRAINT g_raster_folder_pk PRIMARY KEY (gid)" \
			")";
		g_sprintf(sql, format, g_raster_folder_table.c_str());
		return m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE WorkspacePgs::CreateFeatureCatalogTable()
	{
		char sql[AUGE_SQL_MAX];
		const char* format = "	CREATE TABLE %s" \
			"	(" \
			"	  gid serial NOT NULL," \
			"	  name character varying(32)," \
			"	  alias character varying(32)," \
			"	  count integer," \
			"	  minx double precision," \
			"	  miny double precision," \
			"	  maxx double precision," \
			"	  maxy double precision," \
			"	  uuid character varying(128)," \
			"	  CONSTRAINT g_feature_catalog_pk PRIMARY KEY (gid)," \
			"	  CONSTRAINT g_feature_catalog_name_uk UNIQUE (name)" \
			"	)";
		g_sprintf(sql, format, g_feature_catalog_table.c_str());
		return m_pgConnection.ExecuteSQL(sql);
	}
	//////////////////////////////////////////////////////////////////////////
	// Raster End
	//////////////////////////////////////////////////////////////////////////
}
