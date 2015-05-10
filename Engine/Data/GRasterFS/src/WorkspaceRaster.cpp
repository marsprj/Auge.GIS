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
		return OpenRasterDataset(name);
	}

	RESULTCODE WorkspaceRaster::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
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

	EnumDataSet* WorkspaceRaster::GetRasterDatasets()
	{
		return NULL;
	}
	
	RasterDataset* WorkspaceRaster::OpenRasterDataset(const char* name)
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

	RESULTCODE WorkspaceRaster::AddRaster(Raster* pRaster)
	{
		return AG_SUCCESS;
	}

	Raster*	WorkspaceRaster::OpenRaster(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		g_char raster_path[AUGE_PATH_MAX];
		memset(raster_path,0,AUGE_PATH_MAX);
		auge_make_path(raster_path, NULL, m_path.c_str(), name, NULL);

		RasterIO* pRasterIO = augeGetRasterIOInstance();
		return pRasterIO->Read(raster_path);
	}


}