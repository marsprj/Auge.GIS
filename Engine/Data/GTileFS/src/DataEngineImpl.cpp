#include "DataEngineImpl.h"
#include "WorkspaceTFS.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineTFS g_pgEngine;
		return &g_pgEngine;
	}

	DataEngineTFS::DataEngineTFS():
	m_handler(NULL)
	{

	}

	DataEngineTFS::~DataEngineTFS()
	{

	}

	const char*	DataEngineTFS::GetID()
	{
		return "TileFS";
	}

	const char*	DataEngineTFS::GetDescription()
	{
		return "Tile Store FileSystem";
	}


	const char*	DataEngineTFS::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineTFS::GetHandler()
	{
		return m_handler;
	}

	void DataEngineTFS::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineTFS::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineTFS::CreateWorkspace()
	{
		return (new TileWorkspaceFS());
	}
}