#include "DataEngineImpl.h"
#include "WorkspaceMy.h"
#include "ConnectionMy.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineMy g_pgEngine;
		return &g_pgEngine;
	}

	DataEngineMy::DataEngineMy():
	m_handler(NULL)
	{

	}

	DataEngineMy::~DataEngineMy()
	{

	}

	const char*	DataEngineMy::GetID()
	{
		return "MySQL";
	}

	const char*	DataEngineMy::GetDescription()
	{
		return "MySQL Data Engine";
	}


	const char*	DataEngineMy::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineMy::GetHandler()
	{
		return m_handler;
	}

	void DataEngineMy::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineMy::CreateConnection()
	{
		return (new ConnectionMy());
	}

	Workspace* DataEngineMy::CreateWorkspace()
	{
		return (new WorkspaceMy());
	}
}