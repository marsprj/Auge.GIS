#include "WorkspaceByd.h"
#include "AugeCore.h"
#include "DataEngineImpl.h"
#include "FeatureClassByd.h"
#include "SQLBuilder.h"

namespace auge
{	
	WorkspaceByd::WorkspaceByd()
	{
		m_pbydEnvironment = NULL;
		m_pbydConnction = NULL;
	}

	WorkspaceByd::~WorkspaceByd()
	{
		Close();
		if(m_pbydEnvironment!=NULL)
		{
			CPPIEnvironment::TerminateEnvironment(m_pbydEnvironment);
			m_pbydEnvironment = NULL;
		}
	}

	DataEngine*	WorkspaceByd::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspaceByd::GetName()
	{
		return m_name.c_str();
	}

	void WorkspaceByd::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspaceByd::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}

		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}
		m_connstring = conn_string;
		m_props.Parse(conn_string);		
		return AG_SUCCESS;
	}

	const char* WorkspaceByd::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	RESULTCODE WorkspaceByd::Open()
	{
		const char* szServer   = m_props.GetValue(AUGE_DB_SERVER);
		const char* szInstance = m_props.GetValue(AUGE_DB_INSTANCE);
		const char* szDatabase = m_props.GetValue(AUGE_DB_DATABASE);
		const char* szUser     = m_props.GetValue(AUGE_DB_USER);
		const char* szPassword = m_props.GetValue(AUGE_DB_PASSWORD);

		if(szServer==NULL||szDatabase==NULL||szUser==NULL||szPassword==NULL)
		{
			return AG_FAILURE;
		}

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		m_pbydEnvironment = CPPIEnvironment::CreateEnvironment();
		if(m_pbydEnvironment==NULL)
		{
			const char* msg = "Create BeyonDB CPPI Access Environment Failed!";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			return AG_FAILURE;
		}

		m_pbydConnction = m_pbydEnvironment->CreateConnection(szServer,szInstance,szDatabase,szUser,szPassword);
		if(m_pbydConnction==NULL)
		{
			CPPIString msg = m_pbydConnction->GetErrorMessage()->GetErrorString();
			
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			CPPIEnvironment::TerminateEnvironment(m_pbydEnvironment);
			m_pbydEnvironment = NULL;

			return AG_FAILURE;
		}

		CPPIStatus status = m_pbydConnction->Connect();
		if(status!=CS_OK)
		{
			CPPIString msg = m_pbydConnction->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			CPPIEnvironment::TerminateEnvironment(m_pbydEnvironment);
			m_pbydEnvironment = NULL;

			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	void WorkspaceByd::Close()
	{
		if(m_pbydConnction!=NULL)
		{
			m_pbydEnvironment->CloseConnection(m_pbydConnction);
			m_pbydConnction = NULL;
		}
	}

	bool WorkspaceByd::IsOpen()
	{
		if(m_pbydConnction==NULL)
		{
			return false;
		}
		return m_pbydConnction->IsOpen();
	}

	GConnection* WorkspaceByd::GetConnection()
	{
		return NULL;
	}

	DataSet* WorkspaceByd::OpenDataSet(const char* name)
	{
		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspaceByd::RemoveDataSet(const char* name)
	{
		return RemoveFeatureClass(name);
	}

	FeatureClass* WorkspaceByd::OpenFeatureClass(const char* name)
	{
		if(name==NULL)
		{
			return NULL;
		}

		FeatureClassByd* pFeatureClass = new FeatureClassByd();
		if(!pFeatureClass->Create(name, this))
		{
			pFeatureClass->Release();
			pFeatureClass = NULL;
		}

		return pFeatureClass;
	}

	FeatureClass* WorkspaceByd::CreateFeatureClass(const char* name, GFields* pFields)
	{
		if(name==NULL||pFields==NULL)
		{
			return NULL;
		}

		if(!CreateSequence(name))
		{
			//return NULL;
		}
		if(!CreateTable(name, pFields))
		{
			return NULL;
		}

		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspaceByd::RemoveFeatureClass(const char* name)
	{
		return AG_SUCCESS;
	}

	EnumDataSet* WorkspaceByd::GetDataSets()
	{
		return GetFeatureClasses();
	}

	EnumDataSet* WorkspaceByd::GetFeatureClasses()
	{
		return NULL;
	}

	RESULTCODE WorkspaceByd::ExecuteSQL(const char* sql)
	{
		if(sql==NULL)
		{
			return AG_FAILURE;
		}

		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		CPPIStatus status = CS_OK;
		CPPIStatement* stmt = m_pbydConnction->CreateStatement();
		status = stmt->ExecuteSQL(sql);

		if(status!=CS_OK)
		{
			CPPIString msg = m_pbydConnction->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pbydConnction->CloseStatement(stmt);
			return AG_FAILURE;
		}

		status = m_pbydConnction->Commit();
		if(status!=CS_OK)
		{
			CPPIString msg = m_pbydConnction->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pbydConnction->CloseStatement(stmt);
			return AG_FAILURE;
		}
		m_pbydConnction->CloseStatement(stmt);

		return AG_SUCCESS;
	}

	bool WorkspaceByd::CreateSequence(const char* name)
	{
		std::string sql;
		SQLBuilder::BuildCreateSequence(sql, name);

		RESULTCODE rc = ExecuteSQL(sql.c_str());
		return (rc==AG_SUCCESS);
	}

	bool WorkspaceByd::CreateTable(const char* name, GFields* pFields)
	{
		std::string sql;
		SQLBuilder::BuildCreateTable(sql, name, pFields);

		RESULTCODE rc = ExecuteSQL(sql.c_str());
		return (rc==AG_SUCCESS);
	}
}
