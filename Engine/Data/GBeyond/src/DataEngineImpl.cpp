#include "DataEngineImpl.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEnginePgs g_bydEngine;
		return &g_bydEngine;
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
		return "Beyond";
	}

	const char*	DataEnginePgs::GetDescription()
	{
		return "Beyond Data Engine";
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
		return NULL;//return (new ConnectionPgs());
	}

	Workspace* DataEnginePgs::CreateWorkspace()
	{
		return NULL;//return (new WorkspacePgs());
	}
}