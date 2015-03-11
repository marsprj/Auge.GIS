#include "WorkspacePgs.h"
#include "ConnectionPgs.h"
#include "FeatureCursorPgs.h"
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
	m_srid(4326),
	m_dimension(2)
	{
		FieldFactory* pFactory = augeGetFieldFactoryInstance();
		m_pFields = pFactory->CreateFields();
	}

	FeatureClassPgs::~FeatureClassPgs()
	{
		AUGE_SAFE_RELEASE(m_pFields);
	}

	const char*	FeatureClassPgs::GetName()
	{
		return m_name.c_str();
	}

	g_uint FeatureClassPgs::GetSRID()
	{
		return m_srid;
	}

	GEnvelope& FeatureClassPgs::GetExtent()
	{
		if(!m_extent.IsValid())
		{
			char sql[AUGE_SQL_MAX];
			g_snprintf(sql, AUGE_SQL_MAX, "select st_extent(%s) from %s", m_geom_filed_name.c_str(), m_name.c_str());

			PGresult* pgResult = NULL;
			pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql);
			if(PQresultStatus(pgResult)==PGRES_TUPLES_OK)
			{
				const char* value = PQgetvalue(pgResult, 0, 0);
				sscanf(value, "BOX(%lf %lf,%lf %lf)", &m_extent.m_xmin,&m_extent.m_ymin,&m_extent.m_xmax,&m_extent.m_ymax);
			}
			PQclear(pgResult);
		}
		return m_extent;
	}

	GFields* FeatureClassPgs::GetFields()
	{
		return m_pFields;
	}

	GField* FeatureClassPgs::GetField(const char* name)
	{
		return m_pFields->GetField(name);
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

		if(!GetMetaData())
		{
			return false;
		}

		if(!CreateFields())
		{
			return false;
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
		std::string sql;
		SQLBuilder::BuildQuery(sql,this);

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		if(pgResult==NULL)
		{
			return NULL;
		}

		FeatureCursorPgs* pCursor = new FeatureCursorPgs();
		if(!pCursor->Create(this, pgResult))
		{
			pCursor->Release();
			pCursor = NULL;
		}
		return pCursor;
	}

	FeatureCursor* FeatureClassPgs::Query(GEnvelope& extent, augeCursorType type/*=augeStaticCursor*/)
	{
		std::string sql;
		SQLBuilder::BuildQuery(sql,extent,this);

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		if(pgResult==NULL)
		{
			return NULL;
		}

		FeatureCursorPgs* pCursor = new FeatureCursorPgs();
		if(!pCursor->Create(this, pgResult))
		{
			pCursor->Release();
			pCursor = NULL;
		}
		return pCursor;
	}

	FeatureCursor* FeatureClassPgs::Query(GFilter* pFilter, augeCursorType type/*=augeStaticCursor*/)
	{
		std::string sql;
		SQLBuilder::BuildQuery(sql,pFilter,this);

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		if(pgResult==NULL)
		{
			return NULL;
		}

		FeatureCursorPgs* pCursor = new FeatureCursorPgs();
		if(!pCursor->Create(this, pgResult))
		{
			pCursor->Release();
			pCursor = NULL;
		}
		return pCursor;
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

		PQclear(pgResult);

		return true;
	}


	GField*	FeatureClassPgs::CreateField(int col, PGresult* pgResult)
	{
		Oid pgType		= PQftype(pgResult, col);
		const char* name= PQfname(pgResult, col);
		
		augeFieldType type = m_pWorkspace->m_pgConnection.GetFieldType(pgType);
		if(type==augeFieldTypeNone)
		{
			return NULL;
		}

		GField	 *pField = NULL;
		GField_2 *pField_2 = NULL;
		FieldFactory* pFactory = augeGetFieldFactoryInstance();

		pField = pFactory->CreateField();
		pField_2 = pField->Field_2();

		pField_2->SetName(name);
		pField_2->SetAlias(name);
		pField_2->SetType(type);
		pField_2->SetLength(PQfsize(pgResult, col));

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
		}

		return pField;
	}

	bool FeatureClassPgs::GetMetaData()
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
}