#include "FeatureCursorStatic.h"
#include "FeatureClassPgs.h"
#include "FeaturePgs.h"
#include "WorkspacePgs.h"
#include "ConnectionPgs.h"
#include "SQLBuilder.h"

namespace auge
{
	FeatureCursorStatic::FeatureCursorStatic():
	m_cursor(-1),
	m_pgResult(NULL),
	m_geom_findex(-1),
	m_has_more_data(true),
	m_fetch_count(200),
	m_fetched_count(0),
	m_last_fetched_count(0)
	{
		m_pWorkspace;
	}

	FeatureCursorStatic::~FeatureCursorStatic()
	{
		if(m_pgResult!=NULL)
		{
			PQclear(m_pgResult);
			m_pgResult = NULL;
		}
		AUGE_SAFE_RELEASE(m_pFeatureClass);

		WorkspacePgs* pWorkspace = m_pFeatureClass->m_pWorkspace;
		ConnectionPgs& pgConnection = pWorkspace->m_pgConnection;

		pgConnection.StartTransaction();
	}

	FeatureClass* FeatureCursorStatic::GetFeatureClass()
	{
		return m_pFeatureClass;
	}

	Feature* FeatureCursorStatic::NextFeature()
	{
		//if(m_cursor<0||(unsigned)m_cursor>=m_count)
		//{
		//	return NULL;
		//}

		//int index = -1;
		//FeaturePgs* pFeature = new FeaturePgs();
		//if(pFeature!=NULL)
		//{
		//	index = m_cursor;
		//	if(!pFeature->Create(index, m_geom_findex, m_pgResult, m_pFeatureClass))
		//	{
		//		delete pFeature;
		//		pFeature = NULL;
		//	}
		//}

		//m_cursor++;
		//return pFeature;
		return NULL;
	}

	void FeatureCursorStatic::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//bool FeatureCursorStatic::Create(FeatureClassPgs* pFeatureClass, PGresult* pgResult)
	//{
	//	m_pFeatureClass = pFeatureClass;
	//	m_pFeatureClass->AddRef();

	//	m_pgResult = pgResult;
	//	m_count = PQntuples(m_pgResult);
	//	m_cursor = 0;

	//	GFields	*pFields = m_pFeatureClass->GetFields();
	//	GField	*pField = pFields->GetGeometryField();
	//	if(pField==NULL)
	//	{
	//		m_geom_findex = -1;
	//	}
	//	else
	//	{
	//		m_geom_findex = pFields->FindField(pField->GetName());
	//	}

	//	return true;
	//}

	bool FeatureCursorStatic::Create(FeatureClassPgs* pFeatureClass)
	{
		m_pWorkspace = pFeatureClass->m_pWorkspace;
		ConnectionPgs& pgConnection = m_pWorkspace->m_pgConnection;

		RESULTCODE rc = AG_SUCCESS;
		rc = pgConnection.StartTransaction();
		if(rc!=AG_SUCCESS)
		{
			return false;
		}

		rc = OpenCursor();
		if(rc!=AG_SUCCESS)
		{
			pgConnection.EndTransaction();
			return false;
		}

		return true;
	}

	RESULTCODE FeatureCursorStatic::OpenCursor()
	{
		memset(m_cursor_name, 0, AUGE_NAME_MAX);
		auge_generate_uuid(m_cursor_name, AUGE_NAME_MAX);

		std::string sql;
		SQLBuilder::BuildQueryCursor(sql, m_cursor_name, m_pFeatureClass);

		PGresult* pgResult = NULL;
		pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql.c_str());
		if(pgResult==NULL)
		{
			return AG_FAILURE;
		}


		return AG_SUCCESS;
	}

}