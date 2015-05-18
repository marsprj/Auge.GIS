#include "DataEngineTileMongo.h"
#include "TileWorkspaceMongo.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineTileMongo g_mongoTileEngine;
		return &g_mongoTileEngine;
	}

	DataEngineTileMongo::DataEngineTileMongo():
	m_handler(NULL)
	{
		mongoc_init();
	}

	DataEngineTileMongo::~DataEngineTileMongo()
	{
		mongoc_cleanup();
	}

	const char*	DataEngineTileMongo::GetID()
	{
		return "tilemgo";
	}

	const char*	DataEngineTileMongo::GetDescription()
	{
		return "Mongo Tile Store";
	}


	const char*	DataEngineTileMongo::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineTileMongo::GetHandler()
	{
		return m_handler;
	}

	void DataEngineTileMongo::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineTileMongo::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineTileMongo::CreateWorkspace()
	{
		return (new TileWorkspaceMongo());
	}
}