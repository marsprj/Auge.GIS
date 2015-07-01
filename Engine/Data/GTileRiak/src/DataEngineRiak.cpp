#include "DataEngineRiak.h"
#include "TileWorkspaceRiak.h"

#include "riack.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineRiak g_riakTileEngine;
		return &g_riakTileEngine;
	}

	DataEngineRiak::DataEngineRiak():
	m_handler(NULL)
	{
		riack_init();
	}

	DataEngineRiak::~DataEngineRiak()
	{
		riack_cleanup();
	}

	const char*	DataEngineRiak::GetID()
	{
		return "TileRiak";
	}

	const char*	DataEngineRiak::GetDescription()
	{
		return "Riak Tile Store";
	}


	const char*	DataEngineRiak::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineRiak::GetHandler()
	{
		return m_handler;
	}

	void DataEngineRiak::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineRiak::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineRiak::CreateWorkspace()
	{
		TileWorkspaceRiak* pWorkspace = new TileWorkspaceRiak();
		return pWorkspace;
		//return (new TileWorkspaceRiak());
	}
}
