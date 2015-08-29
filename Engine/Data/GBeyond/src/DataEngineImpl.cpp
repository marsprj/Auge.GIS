#include "DataEngineImpl.h"
#include "WorkspaceByd.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineByd g_bydEngine;
		return &g_bydEngine;
	}

	DataEngineByd::DataEngineByd():
	m_handler(NULL)
	{
		
	}

	DataEngineByd::~DataEngineByd()
	{

	}

	const char*	DataEngineByd::GetID()
	{
		return "Beyond";
	}

	const char*	DataEngineByd::GetDescription()
	{
		return "Beyond Data Engine";
	}


	const char*	DataEngineByd::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineByd::GetHandler()
	{
		return m_handler;
	}

	void DataEngineByd::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineByd::CreateConnection()
	{
		return NULL;//return (new ConnectionPgs());
	}

	Workspace* DataEngineByd::CreateWorkspace()
	{
		return (new WorkspaceByd());
	}
}