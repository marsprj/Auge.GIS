#include "DataEngineImpl.h"
#include "WorkspacePgs.h"
#include "ConnectionPgs.h"
#include <gdal.h>

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEnginePgs g_pgEngine;
		return &g_pgEngine;
	}

	DataEnginePgs::DataEnginePgs():
	m_handler(NULL)
	{
		
	}

	DataEnginePgs::~DataEnginePgs()
	{

	}

	const char*	DataEnginePgs::GetID()
	{
		return "Postgres";
	}

	const char*	DataEnginePgs::GetDescription()
	{
		return "PostgreSQL Data Engine";
	}


	const char*	DataEnginePgs::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEnginePgs::GetHandler()
	{
		return m_handler;
	}

	void DataEnginePgs::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEnginePgs::CreateConnection()
	{
		return (new ConnectionPgs());
	}

	Workspace* DataEnginePgs::CreateWorkspace()
	{
		return (new WorkspacePgs());
	}
}