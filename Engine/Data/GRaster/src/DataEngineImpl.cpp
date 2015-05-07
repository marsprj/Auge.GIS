#include "DataEngineImpl.h"
#include "WorkspaceRaster.h"

#include "gdal_priv.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineRaster g_rasterEngine;
		return &g_rasterEngine;
	}

	DataEngineRaster::DataEngineRaster():
	m_handler(NULL)
	{
		GDALAllRegister();
	}

	DataEngineRaster::~DataEngineRaster()
	{

	}

	const char*	DataEngineRaster::GetID()
	{
		return "Raster";
	}

	const char*	DataEngineRaster::GetDescription()
	{
		return "File System Raster Engine";
	}


	const char*	DataEngineRaster::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineRaster::GetHandler()
	{
		return m_handler;
	}

	void DataEngineRaster::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineRaster::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineRaster::CreateWorkspace()
	{
		return (new WorkspaceRaster());
	}
}