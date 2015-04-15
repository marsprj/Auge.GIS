#include "WorkspacePgs.h"
#include "FeatureClassPgs.h"
#include "DataEngineImpl.h"
#include "SQLBuilder.h"
#include "EnumDataSetImpl.h"

namespace auge
{	
	WorkspacePgs::WorkspacePgs():
	m_schema("public")
	{

	}

	WorkspacePgs::~WorkspacePgs()
	{
	}

	DataEngine*	WorkspacePgs::GetEngine()
	{
		return augeGetDataEngineInstance();
	}

	const char* WorkspacePgs::GetName()
	{
		return m_name.c_str();
	}

	void WorkspacePgs::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}
	
	RESULTCODE WorkspacePgs::SetConnectionString(const char* conn_string)
	{
		return m_pgConnection.SetConnectionString(conn_string);
	}
	
	const char*	WorkspacePgs::GetConnectionString()
	{
		return m_pgConnection.GetConnectionString();
	}

	RESULTCODE WorkspacePgs::Open()
	{
		return m_pgConnection.Open();
	}

	void WorkspacePgs::Close()
	{
		m_pgConnection.Close();
	}

	bool WorkspacePgs::IsOpen()
	{
		return m_pgConnection.IsOpen();
	}

	FeatureClass* WorkspacePgs::OpenFeatureClass(const char* name)
	{
		if((name==NULL)||(!m_pgConnection.IsOpen()))
		{
			return NULL;
		}

		FeatureClassPgs* pFeatureClass = new FeatureClassPgs();
		if(!pFeatureClass->Create(name, this))
		{
			pFeatureClass->Release();
			pFeatureClass = NULL;
		}

		return pFeatureClass;
	}

	RESULTCODE WorkspacePgs::CreateFeatureClass(const char* name, GFields* pFields)
	{
		if(name==NULL||pFields==NULL)
		{
			return AG_FAILURE;
		}
		if(pFields->Count()==0)
		{
			return AG_FAILURE;
		}
		GField* pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		rc = CreateTable(name, pFields);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}
		rc = AddGeometryColumn(name, pField);
		if(rc!=AG_SUCCESS)
		{
			RemoveTable(name);
			return rc;
		}

		return AG_SUCCESS;
	}

	RESULTCODE WorkspacePgs::RemoveFeatureClass(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_SUCCESS;
		rc = RemoveTable(name);
		if(rc!=AG_SUCCESS)
		{
			return AG_SUCCESS;
		}

		return UnRegiseterGeometryColumn(name);
	}

	EnumDataSet* WorkspacePgs::GetDataSets()
	{
		const char* sql = "select f_table_name from geometry_columns";

		PGresult* pgResult = m_pgConnection.PgExecute(sql);
		if(pgResult==NULL)
		{
			return NULL;
		}

		const char* className = NULL;
		FeatureClass* pFeatureClass = NULL;	
		EnumDataSetImpl *pEnum = new EnumDataSetImpl();

		int nTuples = PQntuples(pgResult);
		for(int i=0;i<nTuples; i++)
		{
			className = PQgetvalue(pgResult, i, 0);
			pFeatureClass = OpenFeatureClass(className);
			if(pFeatureClass!=NULL)
			{
				pEnum->Add(pFeatureClass);
			}
		}

		PQclear(pgResult);

		return pEnum;
	}

	DataSet* WorkspacePgs::OpenDataSet(const char* name)
	{
		return OpenFeatureClass(name);
	}

	//////////////////////////////////////////////////////////////////////////
	// 
	//////////////////////////////////////////////////////////////////////////

	RESULTCODE WorkspacePgs::CreateTable(const char* name, GFields* pFields)
	{
		std::string sql;
		SQLBuilder::BuildCreateTable(sql, name, pFields);
		return m_pgConnection.ExecuteSQL(sql.c_str());
	}

	RESULTCODE WorkspacePgs::RemoveTable(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "drop table %s",name);
		return m_pgConnection.ExecuteSQL(sql);
	}


	bool WorkspacePgs::UnRegisterLayer(long lid)
	{
		//std::string strsql;
		//RESULTCODE rc = AG_SUCCESS;
		//AgSQLBuilder* pSQLBuilder = AgSQLBuilder::GetInstance();
		//rc = pSQLBuilder->BuildUnRegisterLayerSQL(strsql, lLayerID);
		//if(rc!=AG_SUCCESS)
		//{
		//	return false;
		//}
		//PGresult* pgResult = NULL;
		//pgResult = PgExecute(GetPgConnectionWrite(), strsql.c_str(), strsql.length());
		//if(pgResult==NULL)
		//{
		//	return false;
		//}
		//if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		//{
		//	PQclear(pgResult);
		//	return false;
		//}
		//PQclear(pgResult);
		return true;
	}

	//========================================================================
	// PostGIS geometry_columns ±í²Ù×÷
	//========================================================================
	//RESULTCODE WorkspacePgs::RegiseterGeometryColumn(const char* name, AgField* pGeomField)
	//{
	//	PGconn* pgConnection = GetPgConnectionWrite();
	//	if(pgConnection==NULL)
	//	{
	//		return AG_BAD_CONNECTION;
	//	}

	//	long srid = 0;
	//	const char* szLayerType = NULL;
	//	const char* szSpatialColumn = NULL;
	//	AgGeometryDef* pGeometryDef = pGeomField->GetGeometryDef();
	//	AgUtilPGS* pUtil = AgUtilPGS::GetInstance();

	//	srid		= pGeometryDef->GetSRID();
	//	szSpatialColumn = pGeomField->GetName();
	//	szLayerType = pUtil->GeometryTypeAsString(pGeometryDef->GeometryType());
	//	if(strcmp(szLayerType, "LINE")==0)
	//	{
	//		szLayerType = "LINESTRING";
	//	}

	//	const char* szDatabase = m_workspaceInfo.GetValue(AUGE_DB_DATABASE);
	//	const char* szUser     = m_workspaceInfo.GetValue(AUGE_DB_USER);

	//	if(szDatabase==NULL||szUser==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	RESULTCODE rc;
	//	std::string strsql;
	//	AgSQLBuilder *pSQLBuilder = AgSQLBuilder::GetInstance();
	//	rc = pSQLBuilder->BuildRegisterGeometryColumnSQL(strsql,
	//		"public",
	//		name, 
	//		szSpatialColumn,
	//		srid,
	//		2,
	//		szLayerType);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		return rc;
	//	}

	//	PGresult *pgResult = PgExecute(pgConnection, strsql.c_str(), strsql.length());
	//	if(pgResult==NULL)
	//	{
	//		return AG_FAILURE;
	//	}

	//	ExecStatusType status = PQresultStatus(pgResult);
	//	if(status!=PGRES_COMMAND_OK)
	//	{
	//		PQclear(pgResult);
	//		return AG_FAILURE;
	//	}

	//	return AG_SUCCESS;
	//}

	RESULTCODE WorkspacePgs::AddGeometryColumn(const char* name, GField* pField)
	{
		GeometryDef* pGeometryDef = pField->GetGeometryDef();
		if(pGeometryDef==NULL)
		{
			return AG_FAILURE;
		}

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();


		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select addgeometrycolumn('%s','%s','%s',%d,'%s',%d)",
									  m_schema.c_str(),
									  name,
									  pField->GetName(),
									  pGeometryDef->GetSRID(),
									  pGeometryFactory->Encode(pGeometryDef->GeometryType()),
									  pGeometryDef->GetDimension());
		return m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE WorkspacePgs::RegiseterGeometryColumn(const char* name, GField* pField)
	{
		GeometryDef* pGeometryDef = pField->GetGeometryDef();
		if(pGeometryDef==NULL)
		{
			return AG_FAILURE;
		}

		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		

		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "insert into geometry_columns (f_table_catalog, \
																	 f_table_schema, \
																	 f_table_name, \
																	 f_geometry_column, \
																	 coord_dimension,\
																	 srid,\
																	 type),\
																	 ('%s','%s','%s','%s',%d,%d,'%s')",
																	 m_pgConnection.GetDatabase(),
																	 m_schema.c_str(),
																	 name,
																	 pField->GetName(),
																	 2,
																	 pGeometryDef->GetSRID(),
																	 pGeometryFactory->Encode(pGeometryDef->GeometryType()));
		return m_pgConnection.ExecuteSQL(sql);

		return AG_SUCCESS;
	}

	RESULTCODE WorkspacePgs::UnRegiseterGeometryColumn(const char* name)
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "delete from geometry_columns where f_table_name='%s' and f_table_schema='%s'",name,m_schema.c_str());
		return m_pgConnection.ExecuteSQL(sql);
	}
}
