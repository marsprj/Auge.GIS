#include "DataEngineImpl.h"
#include "WorkspaceRaster.h"

#include <gdal/gdal_priv.h>

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineRasterDB g_rasterEngineDB;
		return &g_rasterEngineDB;
	}

	DataEngineRasterDB::DataEngineRasterDB():
	m_handler(NULL)
	{
		//GDALAllRegister();
	}

	DataEngineRasterDB::~DataEngineRasterDB()
	{

	}

	const char*	DataEngineRasterDB::GetID()
	{
		return "RasterDB";
	}

	const char*	DataEngineRasterDB::GetDescription()
	{
		return "Raster Data Engine";
	}

	const char*	DataEngineRasterDB::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineRasterDB::GetHandler()
	{
		return m_handler;
	}

	void DataEngineRasterDB::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineRasterDB::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineRasterDB::CreateWorkspace()
	{
		return (new WorkspaceRasterDB());
	}
}