#include "AugeCore.h"
//#include "DataEngineImpl.h"
#include "WorkspaceRaster.h"
//#include "RasterImpl.h"

#include <gdal/gdal_priv.h>

namespace auge
{
	WorkspaceRasterDB::WorkspaceRasterDB()
	{
		m_pCnnection = NULL;
		g_catalog_table = "g_raster_catalog";
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
		return NULL;
	}

	DataSet* WorkspaceRasterDB::OpenDataSet(const char* name)
	{
		return NULL;
		//return OpenRaster(name);
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
		const char* imgfmt=pRaster->GetFormat();
		const char* path = pRaster->GetPath();
		g_int		srid = pRaster->GetSRID();
		g_uint		width= pRaster->GetWidth();
		g_uint		height=pRaster->GetHeight();
		g_uint		nband= pRaster->GetBandCount(); 
		GEnvelope& extent = pRaster->GetExtent();

		char sql[AUGE_SQL_MAX] = {0};
		const char* format = "insert into g_raster_catalog (name,alias,format,band_count,srid,width,height,minx,miny,maxx,maxy,uuid) values('%s','%s','%s',%d,%d,%d,%d,%f,%f,%f,%f,'%s')";
		g_snprintf(sql, AUGE_SQL_MAX, format,	name,
												alias,
												imgfmt,
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
		const char* format = "CREATE TABLE %s" \
									"(" \
									"	gid serial NOT NULL," \
									"	name character varying(32)," \
									"	alias character varying(32)," \
									"	format character varying(8)," \
									"	band_count integer," \
									"	srid integer," \
									"	width integer DEFAULT 256," \
									"	height integer DEFAULT 256" \
									"	minx double precision," \
									"	miny double precision," \
									"	maxx double precision," \
									"	maxy double precision," \
									"	uuid character varying(128)" \
									")";
		g_sprintf(sql, format, g_catalog_table.c_str());
		return m_pCnnection->ExecuteSQL(sql);
	}
	//////////////////////////////////////////////////////////////////////////
	// Database Operation Begin
	//////////////////////////////////////////////////////////////////////////
}