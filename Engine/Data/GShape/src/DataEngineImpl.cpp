#include "DataEngineImpl.h"
#include "WorkspaceShp.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineShp g_shpEngine;
		return &g_shpEngine;
	}

	DataEngineShp::DataEngineShp():
	m_handler(NULL)
	{

	}

	DataEngineShp::~DataEngineShp()
	{
	}

	const char*	DataEngineShp::GetID()
	{
		return "Shape";
	}

	const char*	DataEngineShp::GetDescription()
	{
		return "Shape file Data Engine";
	}

	const char*	DataEngineShp::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineShp::GetHandler()
	{
		return m_handler;
	}

	void DataEngineShp::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineShp::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineShp::CreateWorkspace()
	{
		return (new WorkspaceShp());
	}
}