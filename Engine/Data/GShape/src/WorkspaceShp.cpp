#include "AugeCore.h"
#include "WorkspaceShp.h"
#include "DataEngineImpl.h"
#include "FeatureClassShp.h"

namespace auge
{	
	WorkspaceShp::WorkspaceShp()
	{

	}

	WorkspaceShp::~WorkspaceShp()
	{

	}

	DataEngine*	WorkspaceShp::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspaceShp::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceShp::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspaceShp::SetConnectionString(const char* conn_string)
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

	const char* WorkspaceShp::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE WorkspaceShp::Open()
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

	void WorkspaceShp::Close()
	{

	}

	bool WorkspaceShp::IsOpen()
	{
		return true;
	}

	DataSet* WorkspaceShp::OpenDataSet(const char* name)
	{
		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspaceShp::RemoveDataSet(const char* name)
	{
		return RemoveFeatureClass(name);
	}

	FeatureClass* WorkspaceShp::OpenFeatureClass(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		FeatureClassShp* pFeatureClass = new FeatureClassShp();
		if(!pFeatureClass->Create(name, this))
		{
			pFeatureClass->Release();
			pFeatureClass = NULL;
		}

		return pFeatureClass;
	}

	RESULTCODE WorkspaceShp::CreateFeatureClass(const char* name, GFields* pFields)
	{
		return AG_SUCCESS;
	}

	RESULTCODE WorkspaceShp::RemoveFeatureClass(const char* name)
	{
		return AG_SUCCESS;
	}

	//EnumDataSet* WorkspaceShp::GetDataSets()
	//{
	//	return NULL;
	//}

	EnumDataSet* WorkspaceShp::GetFeatureClasses()
	{
		return NULL;
	}
}
