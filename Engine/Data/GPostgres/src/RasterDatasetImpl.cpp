#include "RasterDatasetImpl.h"
#include "WorkspacePgs.h"
#include "AugeRaster.h"
#include "EnumRasterImpl.h"

namespace auge
{
	RasterDatasetImpl::RasterDatasetImpl()
	{
		//m_pRaster = NULL;
		m_pFolder = NULL;
		m_pWoskspace = NULL;
	}

	RasterDatasetImpl::~RasterDatasetImpl()
	{
		m_pFolder = NULL;
		m_pWoskspace = NULL;
	}

	augeDataSetType	RasterDatasetImpl::GetType()
	{	
		return augeDataSetRaster;
	}

	Raster*	RasterDatasetImpl::GetRaster()
	{
		//if(m_pRaster==NULL)
		//{
		//	char raster_path[AUGE_PATH_MAX];
		//	memset(raster_path,0,AUGE_PATH_MAX);
		//	auge_make_path(raster_path, NULL, m_path.c_str(), m_name.c_str(),NULL);

		//	char local_path[AUGE_PATH_MAX];
		//	memset(local_path,0,AUGE_PATH_MAX);
		//	auge_make_path(local_path, NULL, m_pWoskspace->GetRepository(), raster_path+1, NULL);

		//	RasterIO* pRasterIO = augeGetRasterIOInstance();
		//	
		//	m_pRaster = pRasterIO->Read(local_path);
		//	if(m_pRaster!=NULL)
		//	{
		//		m_pRaster->SetPath(m_path.c_str());
		//	}
		//}
		//return m_pRaster;
		return NULL;
	}

	const char* RasterDatasetImpl::GetName()
	{
		return m_name.c_str();
	}

	EnumRaster*	RasterDatasetImpl::GetRasters()
	{
		const char* format = "select gid,name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid,dataset from %s where dataset=%d";

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWoskspace->g_raster_table.c_str(), m_pFolder->GetID());

		GResultSet* pResult = m_pWoskspace->m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		EnumRasterImpl* pRasters = new EnumRasterImpl();

		g_uint count = pResult->GetCount();
		for(g_uint i=0; i<count; i++)
		{
			g_uint		gid	 = pResult->GetInt(i,0);
			const char* name = pResult->GetString(i,1);
			const char* alias= pResult->GetString(i,2);
			const char* fmt	 = pResult->GetString(i,3);
			//const char* path = pResult->GetString(i,4);
			g_uint		nband= pResult->GetInt(i,5);
			g_int		srid = pResult->GetInt(i,6);
			g_uint		width= pResult->GetInt(i,7);
			g_uint		height=pResult->GetInt(i,8);
			double		xmin = pResult->GetDouble(i,9);
			double		ymin = pResult->GetDouble(i,10);
			double		xmax = pResult->GetDouble(i,11);
			double		ymax = pResult->GetDouble(i,12);
			const char* uuid = pResult->GetString(i,13);
			g_uint	dataset  = pResult->GetInt(i,14);

			char raster_path[AUGE_PATH_MAX];
			memset(raster_path, 0, AUGE_PATH_MAX);
			auge_make_path(raster_path, NULL, m_pFolder->GetLocalPath(), name, NULL);

			Raster* pRaster = NULL;
			RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
			pRaster = pRasterFactory->CreateRaster(name, alias, format, raster_path, nband, srid, width, height, xmin, ymin, xmax, ymax, uuid);

			if(pRaster!=NULL)
			{
				pRaster->SetID(gid);
				pRasters->Add(pRaster);
			}
		}
		pResult->Release();
		
		return pRasters;
	}

	Raster*	RasterDatasetImpl::GetRaster(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		const char* format = "select gid,name,alias,format,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid,dataset from %s where name='%s' and dataset=%d";

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWoskspace->g_raster_table.c_str(), name, m_pFolder->GetID());

		GResultSet* pResult = m_pWoskspace->m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		if(pResult->GetCount()==0)
		{
			pResult->Release();
			return NULL;
		}

		g_uint		gid	 = pResult->GetInt(0,0);
		//const char* name = pResult->GetString(0,1);
		const char* alias= pResult->GetString(0,2);
		const char* fmt	 = pResult->GetString(0,3);
		//const char* path = pResult->GetString(0,4);
		g_uint		nband= pResult->GetInt(0,5);
		g_int		srid = pResult->GetInt(0,6);
		g_uint		width= pResult->GetInt(0,7);
		g_uint		height=pResult->GetInt(0,8);
		double		xmin = pResult->GetDouble(0,9);
		double		ymin = pResult->GetDouble(0,10);
		double		xmax = pResult->GetDouble(0,11);
		double		ymax = pResult->GetDouble(0,12);
		const char* uuid = pResult->GetString(0,13);
		g_uint	dataset  = pResult->GetInt(0,14);

		char raster_path[AUGE_PATH_MAX];
		memset(raster_path, 0, AUGE_PATH_MAX);
		auge_make_path(raster_path, NULL, m_pFolder->GetLocalPath(), name, NULL);

		Raster* pRaster = NULL;
		RasterFactory* pRasterFactory = augeGetRasterFactoryInstance();
		pRaster = pRasterFactory->CreateRaster(name, alias, format, raster_path, nband, srid, width, height, xmin, ymin, xmax, ymax, uuid);

		pResult->Release();
		
		if(pRaster==NULL)
		{
			return NULL;
		}

		pRaster->SetID(gid);
		return pRaster;
	}

	//const char*	RasterDatasetImpl::GetAlias()
	//{
	//	return m_alias.c_str();
	//}
	//
	//const char* RasterDatasetImpl::GetFormat()
	//{
	//	return m_format.c_str();
	//}

	//g_uint RasterDatasetImpl::GetWidth()
	//{
	//	return m_width;
	//}

	//g_uint RasterDatasetImpl::GetHeight()
	//{
	//	return m_height;
	//}

	//g_int RasterDatasetImpl::GetSRID()
	//{
	//	return m_srid;
	//}

	//g_uint RasterDatasetImpl::GetBandCount()
	//{
	//	return m_nband;
	//}

	//GEnvelope& RasterDatasetImpl::GetExtent()
	//{
	//	return m_extent;
	//}

	//const char*	RasterDatasetImpl::GetPath()
	//{
	//	return m_path.c_str();
	//}

	////void RasterDatasetImpl::Create(Raster* pRaster)
	////{
	////	if(m_pRaster!=NULL)
	////	{
	////		AUGE_SAFE_RELEASE(m_pRaster);
	////	}
	////	m_pRaster = pRaster;
	////}

	//void RasterDatasetImpl::Create(WorkspacePgs* pWorkspace,g_uint gid,const char* name,const char* alias,const char* format,const char* path,g_uint nband,g_int srid,g_uint width,g_uint height,double xmin,double ymin,double xmax,double ymax,const char* uuid)
	//{
	//	m_id = gid;
	//	m_nband = nband;
	//	m_srid = srid;
	//	m_width = width;
	//	m_height = height;
	//	m_name = name;
	//	m_alias = alias;
	//	m_format = format;
	//	m_path = path;
	//	m_uuid = uuid;
	//	m_extent.Set(xmin, ymin, xmax, ymax);
	//	m_pWoskspace = pWorkspace;
	//}

	void RasterDatasetImpl::Create(const char* name, RasterFolderImpl* pFolder, WorkspacePgs* pWorkspace)
	{
		m_name = name;
		m_pFolder = pFolder;
		m_pWoskspace = pWorkspace;
	}

	RESULTCODE RasterDatasetImpl::AddRaster(const char* name, Raster* pRaster)
	{
		if(name==NULL||pRaster==NULL)
		{
			return AG_FAILURE;
		}

		if(HasRaster(name))
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "Raster [%s] has existed", name);
			augeGetErrorInstance()->SetError(msg);
			return AG_FAILURE;
		}

		char raster_path[AUGE_PATH_MAX];
		memset(raster_path, 0, AUGE_PATH_MAX);
		auge_make_path(raster_path, NULL, m_pFolder->GetLocalPath(), name,NULL);
		if(g_access(raster_path, 4))
		{
			pRaster->Save(raster_path);
		}

		char uuid[AUGE_PATH_MAX] = {0};
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		//const char* path	= pRaster->GetPath();
		const char* alias	= pRaster->GetAlias()==NULL ? name : pRaster->GetAlias();
		const char* fmt		= pRaster->GetFormat();

		g_int		srid	= pRaster->GetSRID();
		g_uint		width	= pRaster->GetWidth();
		g_uint		height	= pRaster->GetHeight();
		g_uint		nband	= pRaster->GetBandCount(); 
		GEnvelope& extent	= pRaster->GetExtent();

		char sql[AUGE_SQL_MAX] = {0};
		const char* format = "insert into g_raster (name,alias,format,dataset,path,band_count,srid,width,height,minx,miny,maxx,maxy,uuid) values('%s','%s','%s',%d,'%s',%d,%d,%d,%d,%f,%f,%f,%f,'%s')";
		g_snprintf(sql, AUGE_SQL_MAX, format,	name,
										alias,
										fmt,
										m_pFolder->GetID(),
										raster_path,//raster_path,
										nband,
										srid,
										width,
										height,
										extent.m_xmin,
										extent.m_ymin,
										extent.m_xmax,
										extent.m_ymax,
										uuid);
		return m_pWoskspace->m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE RasterDatasetImpl::AddRaster(const char* name, const char* raster_path)
	{
		return AG_FAILURE;
	}

	RESULTCODE RasterDatasetImpl::RemoveRaster(const char* name)
	{
		return AG_FAILURE;
	}

	RESULTCODE RasterDatasetImpl::RemoveAllRaster()
	{
		return AG_FAILURE;
	}

	bool RasterDatasetImpl::HasRaster(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		const char* format = "select count(*) from %s where name='%s' and dataset=%d";

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWoskspace->g_raster_table.c_str(), name, m_pFolder->GetID());

		GResultSet* pResult = m_pWoskspace->m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}
		int count = pResult->GetInt(0, 0);
		pResult->Release();
		return (count>0);
	}

}
