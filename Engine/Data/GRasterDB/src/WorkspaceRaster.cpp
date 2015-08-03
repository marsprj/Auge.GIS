#include "AugeCore.h"
#include "DataEngineImpl.h"
#include "EnumDataSetImpl.h"
#include "WorkspaceRaster.h"
#include "RasterDatasetImpl.h"

#include <gdal/gdal_priv.h>

namespace auge
{
	WorkspaceRasterDB::WorkspaceRasterDB()
	{
		m_pCnnection = NULL;
		g_catalog_table = "g_raster";
		g_config_table  = "g_raster_config";
#ifdef WIN32
		m_repository = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload";
#else
		m_repository = "/opt/auge/upload";
#endif
	}

	WorkspaceRasterDB::~WorkspaceRasterDB()
	{
		Close();
	}

	const char*	WorkspaceRasterDB::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceRasterDB::SetName(const char* name)
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

	const char* WorkspaceRasterDB::GetRepository()
	{
		return m_repository.c_str();
	}

	RESULTCODE WorkspaceRasterDB::SetConnectionString(const char* conn_string)
	{
		return AG_SUCCESS;
	}

	const char*	WorkspaceRasterDB::GetConnectionString()
	{
		return NULL;
	}

	void WorkspaceRasterDB::SetConnection(GConnection* pConnection)
	{
		if(m_pCnnection!=NULL)
		{
			m_pCnnection->Close();
			m_pCnnection->Release();
			m_pCnnection = NULL;
		}
		m_pCnnection = pConnection;
	}

	RESULTCODE WorkspaceRasterDB::Open()
	{
		if(m_pCnnection->IsOpen())
		{
			return AG_SUCCESS;
		}
		RESULTCODE rc = m_pCnnection->Open();
		if(rc==AG_SUCCESS)
		{
			if(!m_pCnnection->HasTable(g_catalog_table.c_str()))
			{
				CreateCatalogTable();
			}
			if(!m_pCnnection->HasTable(g_config_table.c_str()))
			{
				CreateConfigTable();
			}
		}
		return rc;
	}

	void WorkspaceRasterDB::Close()
	{
		if(m_pCnnection!=NULL)
		{
			m_pCnnection->Close();
			m_pCnnection->Release();
			m_pCnnection = NULL;
		}
	}

	bool WorkspaceRasterDB::IsOpen()
	{
		if(m_pCnnection==NULL)
		{
			return false;
		}
		return m_pCnnection->IsOpen();
	}

	EnumDataSet* WorkspaceRasterDB::GetDataSets()
	{
		return GetRasterDatasets();
	}

	DataSet* WorkspaceRasterDB::OpenDataSet(const char* name)
	{
		return OpenRasterDataset(name);
	}

	RESULTCODE WorkspaceRasterDB::RemoveDataSet(const char* name)
	{
		return RemoveRaster(name, NULL);
	}

	EnumDataSet* WorkspaceRasterDB::GetRasterDatasets()
	{
		char sql[AUGE_SQL_MAX] = {0};
		const char* format = "select gid,name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid from %s";
		g_snprintf(sql, AUGE_SQL_MAX, format,	g_catalog_table.c_str());

		GResultSet* pResult = m_pCnnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumDataSetImpl* pEnumDataset = new EnumDataSetImpl();
		g_uint count = pResult->GetCount();
		for(g_uint i=0; i<count; i++)
		{
			g_uint		gid	 = pResult->GetInt(i,0);
			const char* name = pResult->GetString(i,1);
			const char* alias= pResult->GetString(i,2);
			const char* fmt	 = pResult->GetString(i,3);
			const char* path = pResult->GetString(i,4);
			g_uint		nband= pResult->GetInt(i,5);
			g_int		srid = pResult->GetInt(i,6);
			g_uint		width= pResult->GetInt(i,7);
			g_uint		height=pResult->GetInt(i,8);
			double		xmin = pResult->GetDouble(i,9);
			double		ymin = pResult->GetDouble(i,10);
			double		xmax = pResult->GetDouble(i,11);
			double		ymax = pResult->GetDouble(i,12);
			const char* uuid = pResult->GetString(i,13);

			RasterDatasetImpl* pDataset = new RasterDatasetImpl();
			pDataset->Create(this,gid, name, alias, fmt, path, nband, srid, width, height, xmin, ymin, xmax, ymax, uuid);

			pEnumDataset->Add(pDataset);
		}

		pResult->Release();

		return pEnumDataset;
	}

	RasterDataset* WorkspaceRasterDB::OpenRasterDataset(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		char sql[AUGE_SQL_MAX] = {0};
		const char* format = "select gid,name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid from %s where name='%s'";
		g_snprintf(sql, AUGE_SQL_MAX, format,	g_catalog_table.c_str(), name);

		GResultSet* pResult = m_pCnnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		g_uint		gid	 = pResult->GetInt(0,0);
		//const char* name = pResult->GetString(0,1);
		const char* alias= pResult->GetString(0,2);
		const char* fmt	 = pResult->GetString(0,3);
		const char* path = pResult->GetString(0,4);
		g_uint		nband= pResult->GetInt(0,5);
		g_int		srid = pResult->GetInt(0,6);
		g_uint		width= pResult->GetInt(0,7);
		g_uint		height=pResult->GetInt(0,8);
		double		xmin = pResult->GetDouble(0,9);
		double		ymin = pResult->GetDouble(0,10);
		double		xmax = pResult->GetDouble(0,11);
		double		ymax = pResult->GetDouble(0,12);
		const char* uuid = pResult->GetString(0,13);

		RasterDatasetImpl* pDataset = new RasterDatasetImpl();
		pDataset->Create(this,gid, name, alias, fmt, path, nband, srid, width, height, xmin, ymin, xmax, ymax, uuid);

		pResult->Release();

		return pDataset;
	}

	DataEngine*	WorkspaceRasterDB::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	void WorkspaceRasterDB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	Raster*	WorkspaceRasterDB::OpenRaster(const char* name)
	{
		//if(name==NULL)
		//{
		//	return NULL;
		//}

		//g_char raster_path[AUGE_PATH_MAX];
		//memset(raster_path,0,AUGE_PATH_MAX);
		//auge_make_path(raster_path, NULL, m_path.c_str(), name, NULL);

		//GDALDataset* poDataset = (GDALDataset*)GDALOpen(raster_path, GA_ReadOnly);		
		//if(poDataset==NULL)
		//{
		//	const char* msg = CPLGetLastErrorMsg();
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);
		//	return NULL;
		//}

		//RasterImpl* pRaster = new RasterImpl();
		//pRaster->Create(name, poDataset, this,raster_path);

		//return pRaster;

		return NULL;
	}

	RESULTCODE WorkspaceRasterDB::AddRaster(Raster* pRaster)
	{
		if(pRaster==NULL)
		{
			return AG_FAILURE;
		}
		char uuid[AUGE_PATH_MAX] = {0};
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		const char* name = pRaster->GetName();
		const char* alias= pRaster->GetAlias();
		const char* fmt  = pRaster->GetFormat();
		const char* path = pRaster->GetPath();
		g_int		srid = pRaster->GetSRID();
		g_uint		width= pRaster->GetWidth();
		g_uint		height=pRaster->GetHeight();
		g_uint		nband= pRaster->GetBandCount(); 
		GEnvelope& extent = pRaster->GetExtent();

		char sql[AUGE_SQL_MAX] = {0};
		const char* format = "insert into g_raster (name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid) values('%s','%s','%s','%s',%d,%d,%d,%d,%f,%f,%f,%f,'%s')";
		g_snprintf(sql, AUGE_SQL_MAX, format,	name,
												alias,
												fmt,
												path,
												nband,
												srid,
												width,
												height,
												extent.m_xmin,
												extent.m_ymin,
												extent.m_xmax,
												extent.m_ymax,
												uuid);
		return m_pCnnection->ExecuteSQL(sql);
	}

	RESULTCODE WorkspaceRasterDB::RemoveRaster(const char* name, const char* path)
	{
		if(name==NULL||path==NULL)
		{
			return AG_FAILURE;
		}

		char sql[AUGE_SQL_MAX] = {0};
		g_sprintf(sql, "delete from g_raster where name='%s' and path='%s'",name, path);
		return m_pCnnection->ExecuteSQL(sql);
	}

	RESULTCODE WorkspaceRasterDB::RemoveRaster(g_uint id)
	{
		return AG_SUCCESS;
	}

	//////////////////////////////////////////////////////////////////////////
	// Database Operation Begin
	//////////////////////////////////////////////////////////////////////////
	RESULTCODE WorkspaceRasterDB::CreateCatalogTable()
	{
		char sql[AUGE_SQL_MAX];
		const char* format = "CREATE TABLE %s(" \
									"	gid serial NOT NULL," \
									"	name character varying(32)," \
									"	alias character varying(32)," \
									"	format character varying(8)," \
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
									"	CONSTRAINT g_raster_catalog_pk PRIMARY KEY (gid)," \
									"	CONSTRAINT g_raster_catalog_name_uk UNIQUE (name, format)" \
									")";
		g_sprintf(sql, format, g_catalog_table.c_str());
		return m_pCnnection->ExecuteSQL(sql);
	}

	RESULTCODE WorkspaceRasterDB::CreateConfigTable()
	{
		char sql[AUGE_SQL_MAX];
		const char* format = "CREATE TABLE %s(" \
			"	gid serial NOT NULL," \
			"	key character varying(32) NOT NULL," \
			"	value character varying(64) NOT NULL" \
			")";
		g_sprintf(sql, format, g_config_table.c_str());
		return m_pCnnection->ExecuteSQL(sql);
	}
	//////////////////////////////////////////////////////////////////////////
	// Database Operation Begin
	//////////////////////////////////////////////////////////////////////////
}