#include "DataEngineImpl.h"
#include "WorkspaceCsv.h"

namespace auge
{
	DataEngine* augeGetDataEngineInstance()
	{
		static DataEngineCsv g_csvEngine;
		return &g_csvEngine;
	}

	DataEngineCsv::DataEngineCsv():
	m_handler(NULL)
	{

	}

	DataEngineCsv::~DataEngineCsv()
	{

	}

	const char*	DataEngineCsv::GetID()
	{
		return "CsvFile";
	}

	const char*	DataEngineCsv::GetDescription()
	{
		return "CSV File Engine";
	}


	const char*	DataEngineCsv::GetLibraryPath()
	{
		return m_path.c_str();
	}

	void* DataEngineCsv::GetHandler()
	{
		return m_handler;
	}

	void DataEngineCsv::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	GConnection* DataEngineCsv::CreateConnection()
	{
		return NULL;
	}

	Workspace* DataEngineCsv::CreateWorkspace()
	{
		return (new WorkspaceCsv());
	}
}