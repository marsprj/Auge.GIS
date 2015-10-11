#include "WorkspaceByd.h"
#include "AugeCore.h"
#include "AugeField.h"
#include "DataEngineImpl.h"
#include "EnumDataSetImpl.h"
#include "FeatureClassByd.h"
#include "SQLBuilder.h"

namespace auge
{	
	WorkspaceByd::WorkspaceByd()
	{
		m_user = -1;
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

	void WorkspaceByd::SetUser(g_uint user)
	{
		m_user = user;
	}

	g_int WorkspaceByd::GetUser()
	{
		return m_user;
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

		pLogger->Info("Open BeyonDB Connection....",__FILE__, __LINE__);

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
		GField* pField = pFields->GetGeometryField();
		if(!RegisterGeometryColumn(name, pField))
		{
			DropTable(name);
			return NULL;
		}

		return OpenFeatureClass(name);
	}

	RESULTCODE WorkspaceByd::RemoveFeatureClass(const char* name)
	{
		FeatureClass* pFeatureClass = OpenFeatureClass(name);
		if(pFeatureClass==NULL)
		{
			return AG_SUCCESS;
		}

		GField* pGeomField = pFeatureClass->GetFields()->GetGeometryField();

		DropTable(name);
		if(pGeomField!=NULL)
		{
			UnRegisterGeometryColumn(name, pGeomField->GetName());
		}

		pFeatureClass->Release();

		return AG_SUCCESS;
	}

	EnumDataSet* WorkspaceByd::GetDataSets()
	{
		return GetFeatureClasses();
	}

	EnumDataSet* WorkspaceByd::GetFeatureClasses()
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		FeatureClass* pFeatureClass = NULL;
		EnumDataSetImpl* pDatasets = new EnumDataSetImpl();

		const char* name = NULL;
		std::vector<std::string> names;
		std::vector<std::string>::iterator iter;

		GetDatasetNames(names);
		for(iter=names.begin(); iter!=names.end(); iter++)
		{
			name = (*iter).c_str();
			pFeatureClass = OpenFeatureClass(name);
			if(pFeatureClass!=NULL)
			{
				pDatasets->Add(pFeatureClass);
			}
		}

		return pDatasets;
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

	bool WorkspaceByd::DropTable(const char *name)
	{
		std::string sql;
		SQLBuilder::BuildDropTable(sql, name);

		RESULTCODE rc = ExecuteSQL(sql.c_str());
		return (rc==AG_SUCCESS);
	}

	void WorkspaceByd::GetDatasetNames(std::vector<std::string>& names)
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		std::string sql;
		const char* user = m_props.GetValue(AUGE_DB_USER);
		SQLBuilder::BuildGetDatasetNames(sql, user);

		const char* fname = NULL;
		CPPIInt2 field = 0;
		
		CPPIRow* row = NULL;
		CPPIDataItem* item = NULL;
		CPPIResultSet* rest = NULL;
		CPPIStatement* stmt = m_pbydConnction->CreateStatement();

		rest = stmt->ExecuteQuery(sql.c_str());
		if(rest==NULL)
		{
			CPPIString msg = m_pbydConnction->GetErrorMessage()->GetErrorString();

			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);

			m_pbydConnction->CloseStatement(stmt);
		}
		else
		{
			FeatureClass* pFeatureClass = NULL;
			while(rest->Next()==CS_OK)
			{
				row = rest->GetRow();
				item = row->GetItem(field);
				if(item!=NULL)
				{
					//CPPIString str = ;
					//CPPIDataType type = item->GetDataType();
					//const char* fname = str.c_str();
					//CPPISizeType len = str.length();
					//CPPISizeType size = str.size();

					std::string sfname = item->GetAsString().Trim();
					names.push_back(sfname);
				}
				
			}

			stmt->CloseResultSet(rest);
			m_pbydConnction->CloseStatement(stmt);
		}
	}

	bool WorkspaceByd::RegisterGeometryColumn(const char* className, GField* pField)
	{
		char sql[AUGE_SQL_MAX];
		SQLBuilder::BuildRegisterGeometryColumn(sql, AUGE_SQL_MAX, className, pField);
		RESULTCODE rc = ExecuteSQL(sql);
		return (rc==AG_SUCCESS);
	}

	bool WorkspaceByd::UnRegisterGeometryColumn(const char* className, const char* fieldName)
	{
		char sql[AUGE_SQL_MAX];
		SQLBuilder::BuildUnRegisterGeometryColumn(sql, AUGE_SQL_MAX, className, fieldName);
		RESULTCODE rc = ExecuteSQL(sql);
		return (rc==AG_SUCCESS);
	}
}
