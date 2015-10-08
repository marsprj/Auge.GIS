#include "AugeCore.h"
#include "DataEngineImpl.h"
#include "WorkspaceCsv.h"
#include "DataSetCvs.h"

namespace auge
{	
	WorkspaceCsv::WorkspaceCsv()
	{

	}

	WorkspaceCsv::~WorkspaceCsv()
	{

	}

	DataEngine*	WorkspaceCsv::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspaceCsv::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceCsv::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}

	void WorkspaceCsv::SetUser(g_uint user)
	{
		m_user = user;
	}

	g_int WorkspaceCsv::GetUser()
	{
		return m_user;
	}
	
	RESULTCODE WorkspaceCsv::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		m_connstring = conn_string;
		size_t epos = m_connstring.find_first_of("=");
		if(epos==std::string::npos)
		{
			m_connstring.clear();
			return AG_FAILURE;
		}

		m_path = m_connstring.substr(epos+1, m_connstring.length()-epos-1);
		return AG_SUCCESS;
	}

	const char* WorkspaceCsv::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE WorkspaceCsv::Open()
	{
		GError* pError = augeGetErrorInstance();

		if(m_path.empty())
		{
			return AG_FAILURE;
		}

		if(g_access(m_path.c_str(), 4))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg,"Path [%s] doesn't exist.", m_path.c_str());
			pError->SetError(msg);

			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	void WorkspaceCsv::Close()
	{

	}

	bool WorkspaceCsv::IsOpen()
	{
		return true;
	}

	GConnection* WorkspaceCsv::GetConnection()
	{
		return NULL;
	}

	EnumDataSet* WorkspaceCsv::GetDataSets()
	{
		return NULL;
	}

	DataSet* WorkspaceCsv::OpenDataSet(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		char csv_path[AUGE_PATH_MAX];
		auge_make_path(csv_path, NULL, m_path.c_str(), name, "csv");

		if(g_access(csv_path,4))
		{
			return NULL;
		}

		DataSetCsv* pDataSet = new DataSetCsv();
		pDataSet->Create(name, csv_path, this);
		return pDataSet;
	}

	RESULTCODE WorkspaceCsv::RemoveDataSet(const char* name)
	{
		return AG_SUCCESS;
	}

	FeatureClass* WorkspaceCsv::OpenFeatureClass(const char* name)
	{
		return NULL;
	}

	FeatureClass* WorkspaceCsv::CreateFeatureClass(const char* name, GFields* pFields)
	{
		return NULL;
	}

	RESULTCODE WorkspaceCsv::RemoveFeatureClass(const char* name)
	{
		return AG_SUCCESS;
	}

	EnumDataSet* WorkspaceCsv::GetFeatureClasses()
	{
		return NULL;
	}
}
