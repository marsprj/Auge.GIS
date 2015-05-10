#include "AugeCore.h"
#include "DataEngineImpl.h"
#include "WorkspaceRaster.h"
//#include "RasterImpl.h"
#include "RasterDatasetImpl.h"

#include <gdal/gdal_priv.h>

namespace auge
{
	WorkspaceRaster::WorkspaceRaster()
	{
		
	}

	WorkspaceRaster::~WorkspaceRaster()
	{
		
	}

	const char*	WorkspaceRaster::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceRaster::SetName(const char* name)
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

	RESULTCODE WorkspaceRaster::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		m_connection_string = conn_string;
		size_t epos = m_connection_string.find_first_of("=");
		if(epos==std::string::npos)
		{
			m_connection_string.clear();
			return AG_FAILURE;
		}

		m_path = m_connection_string.substr(epos+1, m_connection_string.length()-epos-1);
		return AG_SUCCESS;
	}

	const char*	WorkspaceRaster::GetConnectionString()
	{
		return m_connection_string.c_str();
	}

	RESULTCODE WorkspaceRaster::Open()
	{
		GError* pError = augeGetErrorInstance();

		if(m_path.empty())
		{
			return AG_FAILURE;
		}

		if(g_access(m_path.c_str(), 4))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg,"Path [%s] doesn't exist.", m_path.c_str());
			pError->SetError(msg);

			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	void WorkspaceRaster::Close()
	{

	}

	bool WorkspaceRaster::IsOpen()
	{
		return true;
	}

	EnumDataSet* WorkspaceRaster::GetDataSets()
	{
		return NULL;
	}

	DataSet* WorkspaceRaster::OpenDataSet(const char* name)
	{
		char raster_path[AUGE_NAME_MAX] = {0};

		Raster* pRaster = OpenRaster(name);
		if(pRaster==NULL)
		{
			return NULL;
		}

		RasterDatasetImpl* pRasterDataset = new RasterDatasetImpl();
		pRasterDataset->Create(pRaster);
		return pRasterDataset;
	}

	DataEngine*	WorkspaceRaster::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	void WorkspaceRaster::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	Raster*	WorkspaceRaster::OpenRaster(const char* name)
	{
		return NULL;
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
		//
		//RasterImpl* pRaster = new RasterImpl();
		//pRaster->Create(name, poDataset, this,raster_path);

		//return pRaster;
	}

	void WorkspaceRaster::SetConnection(GConnection* pConnection)
	{

	}

	RESULTCODE WorkspaceRaster::AddRaster(Raster* pRaster)
	{
		return NULL;
	}

	RESULTCODE WorkspaceRaster::RemoveRaster(g_uint id)
	{
		return AG_SUCCESS;
	}

}