#include "WorkspacePgs.h"
#include "ConnectionPgs.h"
#include "FeatureCursorPgs.h"
#include "FeatureCursorStatic.h"
#include "FeatureClassPgs.h"
#include "FeatureInsertCommandPgs.h"
#include "FeatureObj.h"

#include "SQLBuilder.h"

#include <AugeCore.h>

namespace auge
{
	FeatureClassPgs::FeatureClassPgs():
	m_pWorkspace(NULL),
	m_pFields(NULL),
	m_schema("public"),
	m_geom_type(augeGTNull),
	m_srid(AUGE_DEFAULT_SRID),
	m_dimension(2),
	m_feature_count(0)
	{
		FieldFactory* pFactory = augeGetFieldFactoryInstance();
		m_pFields = pFactory->CreateFields();
	}

	FeatureClassPgs::~FeatureClassPgs()
	{
		AUGE_SAFE_RELEASE(m_pFields);
	}

	augeDataSetType FeatureClassPgs::GetType()
	{
		return augeDataSetFeature;
	}

	const char*	FeatureClassPgs::GetName()
	{
		return m_name.c_str();
	}

	g_uint FeatureClassPgs::GetSRID()
	{
		return m_srid;
	}

	const char* FeatureClassPgs::GetUUID()
	{
		return m_uuid.c_str();
	}

	GEnvelope& FeatureClassPgs::GetExtent()
	{
		if(!m_extent.IsValid())
		{
			//char sql[AUGE_SQL_MAX];
			//g_snprintf(sql, AUGE_SQL_MAX, "select st_extent(%s) from %s", m_geom_filed_name.c_str(), m_name.c_str());

			//PGresult* pgResult = NULL;
			//pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql);
			//if(PQresultStatus(pgResult)==PGRES_TUPLES_OK)
			//{
			//	const char* value = PQgetvalue(pgResult, 0, 0);
			//	sscanf(value, "BOX(%lf %lf,%lf %lf)", &m_extent.m_xmin,&m_extent.m_ymin,&m_extent.m_xmax,&m_extent.m_ymax);
			//}
			//PQclear(pgResult);
		}
		return m_extent;
	}

	g_uint FeatureClassPgs::GetCount()
	{
		return m_feature_count;

		//std::string sql;
		//SQLBuilder::BuildCount(sql, this);

		//PGresult* pgResult =  m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		//g_uint count = atoi(PQgetvalue(pgResult,0,0));
		//PQclear(pgResult);

		//return count;
	}

	g_uint FeatureClassPgs::GetCount(GEnvelope& extent)
	{
		std::string sql;
		SQLBuilder::BuildCount(sql, extent, this);
		PGresult* pgResult =  m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		g_uint count = atoi(PQgetvalue(pgResult,0,0));
		PQclear(pgResult);

		return count;
	}

	g_uint FeatureClassPgs::GetCount(GFilter* pFilter)
	{
		std::string sql;
		SQLBuilder::BuildCount(sql, pFilter, this);
		PGresult* pgResult =  m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		g_uint count = atoi(PQgetvalue(pgResult,0,0));
		PQclear(pgResult);

		return count;
	}

	GFields* FeatureClassPgs::GetFields()
	{
		return m_pFields;
	}

	GField* FeatureClassPgs::GetField(const char* name)
	{
		return m_pFields->GetField(name);
	}

	RESULTCODE FeatureClassPgs::AddField(const char* name, augeFieldType type, g_uint width)
	{
		if(name==NULL||type==augeFieldTypeNone)
		{
			return AG_FAILURE;
		}

		const char* field_type = m_pWorkspace->m_pgConnection.GetFieldType(type);
		if(field_type==NULL)
		{
			return NULL;
		}

		//ALTER TABLE cities_900913 ADD COLUMN aaa double precision;
		const char* format = "alter table %s add column %s %s";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		if(type==augeFieldTypeString)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "alter table %s add column %s %s(%d)", GetName(), name, field_type, width);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "alter table %s add column %s %s", GetName(), name, field_type);
		}
	
		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE FeatureClassPgs::RemoveField(const char* name)
	{
		if(name==NULL)
		{
			return AG_FAILURE;
		}
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "ALTER TABLE %s DROP COLUMN %s", GetName(), name);
		
		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql);
	}

	bool FeatureClassPgs::Create(const char* name, WorkspacePgs* pWorkspace)
	{
		m_name = name;
		m_pWorkspace = pWorkspace;

		//ConnectionPgs& pgConnection = pWorkspace->m_pgConnection;
		//m_oid = pgConnection.GetPgTableOid(name);
		//if(m_oid==NULL)
		//{
		//	return false;
		//}

		if(!GetGeometryInfo())
		{
			//return false;
		}

		if(!GetMetaInfo())
		{

		}

		if(!CreateFields())
		{
			return false;
		}

		GField* pGeomField = m_pFields->GetGeometryField();
		if(pGeomField!=NULL)
		{
			m_srid = pGeomField->GetGeometryDef()->GetSRID();
		}

		return true;
	}

	bool FeatureClassPgs::Create(const char* name, WorkspacePgs* pWorkspace, PGresult* pgResult)
	{
		m_name = name;
		m_pWorkspace = pWorkspace;

		//ConnectionPgs& pgConnection = pWorkspace->m_pgConnection;
		//m_oid = pgConnection.GetPgTableOid(name);
		//if(m_oid==NULL)
		//{
		//	return false;
		//}

		if(!GetGeometryInfo())
		{
			//return false;
		}

		if(!GetMetaInfo())
		{

		}

		if(!CreateFields(pgResult))
		{
			return false;
		}

		GField* pGeomField = m_pFields->GetGeometryField();
		if(pGeomField!=NULL)
		{
			m_srid = pGeomField->GetGeometryDef()->GetSRID();
		}

		return true;
	}

	void FeatureClassPgs::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	FeatureCursor* FeatureClassPgs::Query(augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursor* pCursor = NULL;
		switch(type)
		{
		case augeStaticCursor:
			{
				FeatureCursorStatic* pCursorS = new FeatureCursorStatic();
				if(!pCursorS->Create(this))
				{
					pCursorS->Release();
				}
				else
				{
					pCursor = pCursorS;
				}
			}
			break;
		case augeDynamicCursor:
			{
				std::string sql;
				SQLBuilder::BuildQuery(sql,this);

				PGresult* pgResult = NULL;
				pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
				if(pgResult==NULL)
				{
					return NULL;
				}

				FeatureCursorPgs* pCursorD = new FeatureCursorPgs();
				if(!pCursorD->Create(this, pgResult))
				{
					pCursorD->Release();
				}
				else
				{
					pCursor = pCursorD;
				}
			}
			break;
		}

		return pCursor;
	}

	//FeatureCursor* FeatureClassPgs::Query(augeCursorType type/*=augeStaticCursor*/)
	//{
	//	std::string sql;
	//	SQLBuilder::BuildQuery(sql,this);

	//	PGresult* pgResult = NULL;
	//	pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
	//	if(pgResult==NULL)
	//	{
	//		return NULL;
	//	}

	//	FeatureCursorPgs* pCursor = new FeatureCursorPgs();
	//	if(!pCursor->Create(this, pgResult))
	//	{
	//		pCursor->Release();
	//		pCursor = NULL;
	//	}
	//	return pCursor;
	//}

	FeatureCursor* FeatureClassPgs::Query(GEnvelope& extent, augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursor* pCursor = NULL;
		switch(type)
		{
		case augeStaticCursor:
			{
				FeatureCursorStatic* pCursorS = new FeatureCursorStatic();
				if(!pCursorS->Create(extent, this))
				{
					pCursorS->Release();
				}
				else
				{
					pCursor = pCursorS;
				}
			}
			break;
		case augeDynamicCursor:
			{
				std::string sql;
				SQLBuilder::BuildQuery(sql,extent,this);

				PGresult* pgResult = NULL;
				pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
				if(pgResult==NULL)
				{
					return NULL;
				}

				FeatureCursorPgs* pCursorD = new FeatureCursorPgs();
				if(!pCursorD->Create(this, pgResult))
				{
					pCursorD->Release();
				}
				else
				{
					pCursor = pCursorD;
				}
			}
			break;
		}

		return pCursor;
	}

	FeatureCursor* FeatureClassPgs::Query(GFilter* pFilter, augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursor* pCursor = NULL;
		switch(type)
		{
		case augeStaticCursor:
			{
				FeatureCursorStatic* pCursorS = new FeatureCursorStatic();
				if(!pCursorS->Create(pFilter, this))
				{
					pCursorS->Release();
				}
				else
				{
					pCursor = pCursorS;
				}
			}
			break;
		case augeDynamicCursor:
			{
				std::string sql;
				SQLBuilder::BuildQuery(sql,pFilter,this);

				PGresult* pgResult = NULL;
				pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
				if(pgResult==NULL)
				{
					return NULL;
				}

				FeatureCursorPgs* pCursorD = new FeatureCursorPgs();
				if(!pCursorD->Create(this, pgResult))
				{
					pCursorD->Release();
				}
				else
				{
					pCursor = pCursorD;
				}
			}
			break;
		}

		return pCursor;
	}

	FeatureCursor* FeatureClassPgs::Query(GQuery* pQuery, augeCursorType type/*=augeStaticCursor*/)
	{
		FeatureCursor* pCursor = NULL;
		switch(type)
		{
		case augeStaticCursor:
			{
				FeatureCursorStatic* pCursorS = new FeatureCursorStatic();
				if(!pCursorS->Create(pQuery, this))
				{
					pCursorS->Release();
				}
				else
				{
					pCursor = pCursorS;
				}
			}
			break;
		case augeDynamicCursor:
			{
				std::string sql;
				SQLBuilder::BuildQuery(sql,pQuery,this);

				PGresult* pgResult = NULL;
				pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
				if(pgResult==NULL)
				{
					return NULL;
				}

				FeatureCursorPgs* pCursorD = new FeatureCursorPgs();
				if(!pCursorD->Create(this, pgResult))
				{
					pCursorD->Release();
				}
				else
				{
					pCursor = pCursorD;
				}
			}
			break;
		}

		return pCursor;
		//std::string sql;
		//SQLBuilder::BuildQuery(sql,pQuery,this);

		//PGresult* pgResult = NULL;
		//pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		//if(pgResult==NULL)
		//{
		//	return NULL;
		//}

		//FeatureClassPgs* pNewClass = new FeatureClassPgs();
		//pNewClass->Create(GetName(), m_pWorkspace, pgResult);
		//if(pNewClass==NULL)
		//{
		//	PQclear(pgResult);
		//	return NULL;
		//}

		//FeatureCursorPgs* pCursor = new FeatureCursorPgs();
		//if(!pCursor->Create(pNewClass, pgResult))
		//{	
		//	pCursor->Release();
		//	pCursor = NULL;
		//}
		//pNewClass->Release();
		//return pCursor;
	}

	Cursor* FeatureClassPgs::GetRows()
	{
		return NULL;
	}

	RESULTCODE FeatureClassPgs::RemoveFeature(GFilter* pFilter)
	{
		RESULTCODE rc = AG_FAILURE;
		std::string sql;
		SQLBuilder::BuildDeleteFeature(sql, pFilter, this);
		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		if(pgResult==NULL)
		{
			return AG_FAILURE;
		}
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			rc = AG_FAILURE;
		}
		PQclear(pgResult);
		return AG_SUCCESS;
	}

	Feature* FeatureClassPgs::NewFeature()
	{
		return (new FeatureObj(this));
	}

	//////////////////////////////////////////////////////////////////////////
	bool FeatureClassPgs::CreateFields()
	{
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select * from %s limit 0", m_name.c_str());

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql);
		if(pgResult==NULL)
		{
			return false;
		}

		CreateFields(pgResult);

		PQclear(pgResult);

		return true;
	}

	bool FeatureClassPgs::CreateFields(PGresult* pgResult)
	{	
		m_pFields->Clear();

		GField* pField = NULL;
		int nfields = PQnfields(pgResult);
		for(int i=0; i<nfields; i++)
		{
			pField = CreateField(i, pgResult);
			if(pField!=NULL)
			{
				m_pFields->Add(pField);
			}
		}

		return true;
	}

	GField*	FeatureClassPgs::CreateField(int col, PGresult* pgResult)
	{
		Oid pgType		 = PQftype(pgResult, col);
		const char* fname= PQfname(pgResult, col);
		
		augeFieldType type = augeFieldTypeNone;
		if(!g_stricmp(fname,m_geom_filed_name.c_str()))
		{
			type = augeFieldTypeGeometry;
		}
		else
		{
			type = m_pWorkspace->m_pgConnection.GetFieldType(pgType);
			if(type==augeFieldTypeNone)
			{
				return NULL;
			}
		}

		GField	 *pField = NULL;
		GField_2 *pField_2 = NULL;
		FieldFactory* pFactory = augeGetFieldFactoryInstance();

		pField = pFactory->CreateField();
		pField_2 = pField->Field_2();

		pField_2->SetName(fname);
		pField_2->SetAlias(fname);
		pField_2->SetType(type);


		g_int fsize = 0;
		if(pField->GetType()==augeFieldTypeString)
			fsize = PQfmod(pgResult, col) -4;
		else
			fsize = PQfsize(pgResult, col);

		pField_2->SetLength(fsize);

		if(type==augeFieldTypeGeometry)
		{
			GeometryDef* pGeometryDef = NULL;
			pGeometryDef = pField->GetGeometryDef();
			GeometryDef_2* pGeometryDef_2 = NULL;
			pGeometryDef_2 = pGeometryDef->GetGeometryDef_2();
			pGeometryDef_2->SetDefault(true);
			pGeometryDef_2->SetGeometryType(m_geom_type);
			pGeometryDef_2->SetM(false);
			pGeometryDef_2->SetZ(false);
			pGeometryDef_2->SetSRID(m_srid);
		}

		return pField;
	}

	bool FeatureClassPgs::GetGeometryInfo()
	{
		char sql[AUGE_SQL_MAX] = {0};
		g_snprintf(sql, AUGE_SQL_MAX, "select * from geometry_columns where f_table_name='%s' and f_table_schema='%s'",m_name.c_str(),m_schema.c_str());

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql);
		if(pgResult==NULL)
		{
			return false;
		}

		if(PQntuples(pgResult)==0)
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "FeatureClass [%s] doesn't exist.", m_name.c_str());
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg,__FILE__, __LINE__);

			PQclear(pgResult);
			return false;
		}

		int fn = -1;
		//fn = PQfnumber(pgResult, "f_table_schema");
		//fn = PQfnumber(pgResult, "f_table_name");		
		// coord_dimension
		fn = PQfnumber(pgResult, "coord_dimension");
		m_dimension = atoi(PQgetvalue(pgResult, 0,fn));
		// srid
		fn = PQfnumber(pgResult, "srid");
		m_srid = atoi(PQgetvalue(pgResult, 0,fn));
		// geometry column
		fn = PQfnumber(pgResult, "f_geometry_column");
		m_geom_filed_name = PQgetvalue(pgResult, 0,fn);
		// geometry type;
		fn = PQfnumber(pgResult, "type");
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		const char* type = PQgetvalue(pgResult, 0,fn);
		m_geom_type = pGeometryFactory->DecodeGeometryType(type);

		PQclear(pgResult);

		return true;
	}

	FeatureInsertCommand* FeatureClassPgs::CreateInsertCommand()
	{
		return (new FeatureInsertCommandPgs(this));
	}

	EnumValue* FeatureClassPgs::GetUniqueValue(const char* field, augeOrderType order/*=augeOrderAsc*/)
	{
		if(!field)
		{
			return NULL;
		}

		GField* pField = GetFields()->GetField(field);
		if(pField==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No field[%s] in FeatureClass [%s]", field, GetName());
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return NULL;
		}

		std::string sql;
		SQLBuilder::BuildGetUniqueValueSQL(sql, field, this, order);

		GResultSet*  pResult = m_pWorkspace->m_pgConnection.ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			return NULL;
		}

		GValue* pValue = NULL;
		EnumValue* pEnumValue = new EnumValue();
		int count = pResult->GetCount();
		for(int i=0; i<count; i++)
		{
			switch(pField->GetType())
			{
			case augeFieldTypeInt:
				pValue = new GValue(pResult->GetInt(i,0));
				break;
			case augeFieldTypeDouble:
				pValue = new GValue(pResult->GetDouble(i,0));
				break;
			case augeFieldTypeString:
				pValue = new GValue(pResult->GetString(i,0));
				break;
			}
			if(pValue)
			{
				pEnumValue->Add(pValue);
			}
		}

		pResult->Release();
		return pEnumValue;
	}

	EnumValue* FeatureClassPgs::GetMinMaxValue(const char* field)
	{
		if(!field)
		{
			return NULL;
		}

		GField* pField = GetFields()->GetField(field);
		if(pField==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "No field[%s] in FeatureClass [%s]", field, GetName());
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return NULL;
		}

		std::string sql;
		SQLBuilder::BuildGetMinMaxValueSQL(sql, field, this);

		GResultSet*  pResult = m_pWorkspace->m_pgConnection.ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			return NULL;
		}

		if(pResult->GetCount()==0)
		{
			pResult->Release();
			return NULL;
		}

		GValue* pValue = NULL;
		EnumValue* pEnumValue = new EnumValue();

		for(int i=0; i<2; i++)
		{
			switch(pField->GetType())
			{
			case augeFieldTypeInt:
				pValue = new GValue(pResult->GetInt(0,i));
				break;
			case augeFieldTypeDouble:
				pValue = new GValue(pResult->GetDouble(0,i));
				break;
			case augeFieldTypeString:
				pValue = new GValue(pResult->GetString(0,i));
				break;
			}
			if(pValue)
			{
				pEnumValue->Add(pValue);
			}
		}

		pResult->Release();
		return pEnumValue;
	}

	RESULTCODE FeatureClassPgs::Refresh()
	{
		RESULTCODE rc = AG_SUCCESS;
		if(!HasMetaInfo())
		{
			rc = AddMetaInfo();
		}
		else
		{
			rc = UpdateMetaInfo();
		}
		return rc;
	}

	g_uint FeatureClassPgs::ComputeCount()
	{
		std::string sql;
		SQLBuilder::BuildCount(sql, this);

		PGresult* pgResult =  m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		g_uint count = atoi(PQgetvalue(pgResult,0,0));
		PQclear(pgResult);

		return count;
	}

	RESULTCODE FeatureClassPgs::ComputeExtent(GEnvelope& extent)
	{
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select st_extent(%s) from %s", m_geom_filed_name.c_str(), m_name.c_str());

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql);
		if(PQresultStatus(pgResult)==PGRES_TUPLES_OK)
		{
			const char* value = PQgetvalue(pgResult, 0, 0);
			sscanf(value, "BOX(%lf %lf,%lf %lf)", &extent.m_xmin,&extent.m_ymin,&extent.m_xmax,&extent.m_ymax);
		}
		PQclear(pgResult);
		return AG_SUCCESS;
	}

	bool FeatureClassPgs::HasMetaInfo()
	{
		char sql[AUGE_SQL_MAX];
		g_snprintf(sql, AUGE_SQL_MAX, "select count(*) from %s where name = '%s'", m_pWorkspace->g_feature_catalog_table.c_str(), m_name.c_str());
		GResultSet* pResult = m_pWorkspace->m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return false;
		}
		int count = pResult->GetInt(0,0);
		pResult->Release();
		return count;
	}

	RESULTCODE FeatureClassPgs::AddMetaInfo()
	{
		GEnvelope extent;
		ComputeExtent(extent);
		g_uint count = ComputeCount();

		char uuid[AUGE_PATH_MAX];
		memset(uuid, 0, AUGE_PATH_MAX);
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		const char* format = "insert into %s (name, alias, count, minx, miny, maxx, maxy, uuid) values('%s','%s',%d,%f,%f,%f,%f,'%s')";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		if(extent.IsValid())
		{
			g_snprintf(sql, AUGE_SQL_MAX, format, m_pWorkspace->g_feature_catalog_table.c_str(), 
				GetName(),
				GetName(),	
				count,
				extent.m_xmin,
				extent.m_ymin,
				extent.m_xmax,
				extent.m_ymax,
				uuid);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, format, m_pWorkspace->g_feature_catalog_table.c_str(), 
				GetName(),
				GetName(),	
				count,
				-180.0f,
				-90.0f,
				180.0f,
				90.0f,
				uuid);
		}
		

		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE FeatureClassPgs::RemoveMetaInfo()
	{
		const char* format = "delete from %s where name='%s'";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWorkspace->g_feature_catalog_table.c_str(), GetName());
		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE FeatureClassPgs::UpdateMetaInfo()
	{
		GEnvelope extent;
		ComputeExtent(extent);
		ComputeCount();

		const char* format = "update %s set name='%s', alias='%s', count=%d, minx=%f, miny=%f, maxx=%f, maxy=%f";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWorkspace->g_feature_catalog_table.c_str(), 
			GetName(),
			GetName(),
			extent.m_xmin,
			extent.m_ymin,
			extent.m_xmax,
			extent.m_ymax);

		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql);
	}

	RESULTCODE FeatureClassPgs::GetMetaInfo()
	{
		const char* format = "select name, alias, count, minx, miny, maxx, maxy, uuid from %s where name='%s'";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, m_pWorkspace->g_feature_catalog_table.c_str(), m_name.c_str());

		GResultSet* pResult = m_pWorkspace->m_pgConnection.ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return AG_FAILURE;
		}

		if(pResult->GetCount()==0)
		{
			pResult->Release();
			return AG_FAILURE;
		}

		m_alias = pResult->GetString(0,1);
		m_feature_count = pResult->GetInt(0,2);
		m_extent.m_xmin = pResult->GetDouble(0,3);
		m_extent.m_ymin = pResult->GetDouble(0,4);
		m_extent.m_xmax = pResult->GetDouble(0,5);
		m_extent.m_ymax = pResult->GetDouble(0,6);
		m_uuid = pResult->GetString(0,7);

		pResult->Release();
		return AG_SUCCESS;
	}
}
