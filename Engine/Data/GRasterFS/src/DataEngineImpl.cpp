#include "DataEngineImpl.h"
#include "WorkspaceRaster.h"

#include <gdal/gdal_priv.h>

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineRasterFS g_rasterEngineFS;
		return &g_rasterEngineFS;
	}

	DataEngineRasterFS::DataEngineRasterFS():
	m_handler(NULL)
	{
		//GDALAllRegister();
	}

	DataEngineRasterFS::~DataEngineRasterFS()
	{

	}

	const char*	DataEngineRasterFS::GetID()
	{
		return "rasterfs";
	}

	const char*	DataEngineRasterFS::GetDescription()
	{
		return "File System Raster Engine";
	}


	const char*	DataEngineRasterFS::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineRasterFS::GetHandler()
	{
		return m_handler;
	}

	void DataEngineRasterFS::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineRasterFS::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineRasterFS::CreateWorkspace()
	{
		return (new WorkspaceRaster());
	}
}