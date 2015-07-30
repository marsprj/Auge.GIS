#include "FeatureCursorStatic.h"
#include "FeatureClassPgs.h"
#include "FeaturePgs.h"
#include "WorkspacePgs.h"
#include "ConnectionPgs.h"
#include "SQLBuilder.h"

namespace auge
{
	FeatureCursorStatic::FeatureCursorStatic():
	m_cursor(0),
	m_pgResult(NULL),
	m_geom_findex(-1),
	m_has_more_data(true),
	m_fetch_count(200),
	m_fetched_count(0),
	m_last_fetched_count(0)
	{
		m_pWorkspace = NULL;
		m_pFeatureClass = NULL;

	}

	FeatureCursorStatic::~FeatureCursorStatic()
	{
		if(m_pgResult!=NULL)
		{
			PQclear(m_pgResult);
			m_pgResult = NULL;
		}
		
		ConnectionPgs& pgConnection = m_pWorkspace->m_pgConnection;

		pgConnection.EndTransaction();

		AUGE_SAFE_RELEASE(m_pFeatureClass);
	}

	FeatureClass* FeatureCursorStatic::GetFeatureClass()
	{
		if(m_pFeatureClass==NULL)
		{
			Fetch();
		}
		return m_pFeatureClass;
	}

	Feature* FeatureCursorStatic::NextFeature()
	{
		if(m_cursor>=m_fetched_count)
		{
			ClearResult();
			bool ret = Fetch();
			if(!ret)
			{
				return NULL;
			}
			m_cursor=0;
		}

		FeaturePgs* pFeature = new FeaturePgs();
		if(!pFeature->Create(m_cursor++, m_geom_findex, m_pgResult, m_pFeatureClass))
		{
			pFeature->Release();
			return NULL;
		}

		return pFeature;
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
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();
		m_pWorkspace = pFeatureClass->m_pWorkspace;
		ConnectionPgs& pgConnection = m_pWorkspace->m_pgConnection;
		
		GFields	*pFields = m_pFeatureClass->GetFields();
		GField	*pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			m_geom_findex = -1;
		}
		else
		{
			m_geom_findex = pFields->FindField(pField->GetName());
		}


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

	bool FeatureCursorStatic::Create(GEnvelope& extent, FeatureClassPgs* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();

		m_pWorkspace = pFeatureClass->m_pWorkspace;
		ConnectionPgs& pgConnection = m_pWorkspace->m_pgConnection;

		GFields	*pFields = m_pFeatureClass->GetFields();
		GField	*pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			m_geom_findex = -1;
		}
		else
		{
			m_geom_findex = pFields->FindField(pField->GetName());
		}


		RESULTCODE rc = AG_SUCCESS;
		rc = pgConnection.StartTransaction();
		if(rc!=AG_SUCCESS)
		{
			return false;
		}

		rc = OpenCursor(extent);
		if(rc!=AG_SUCCESS)
		{
			pgConnection.EndTransaction();
			return false;
		}

		return true;
	}

	bool FeatureCursorStatic::Create(GFilter* pFilter, FeatureClassPgs* pFeatureClass)
	{
		m_pFeatureClass = pFeatureClass;
		m_pFeatureClass->AddRef();
		m_pWorkspace = pFeatureClass->m_pWorkspace;
		ConnectionPgs& pgConnection = m_pWorkspace->m_pgConnection;

		GFields	*pFields = m_pFeatureClass->GetFields();
		GField	*pField = pFields->GetGeometryField();
		if(pField==NULL)
		{
			m_geom_findex = -1;
		}
		else
		{
			m_geom_findex = pFields->FindField(pField->GetName());
		}


		RESULTCODE rc = AG_SUCCESS;
		rc = pgConnection.StartTransaction();
		if(rc!=AG_SUCCESS)
		{
			return false;
		}

		rc = OpenCursor(pFilter);
		if(rc!=AG_SUCCESS)
		{
			pgConnection.EndTransaction();
			return false;
		}

		return true;
	}

	bool FeatureCursorStatic::Create(GQuery*  pQuery, FeatureClassPgs* pFeatureClass)
	{
		m_class_name = pFeatureClass->GetName();
		//m_pFeatureClass = pFeatureClass;
		m_pWorkspace = pFeatureClass->m_pWorkspace;
		ConnectionPgs& pgConnection = m_pWorkspace->m_pgConnection;
		
		RESULTCODE rc = AG_SUCCESS;
		rc = pgConnection.StartTransaction();
		if(rc!=AG_SUCCESS)
		{
			return false;
		}

		rc = OpenCursor(pQuery, pFeatureClass);
		if(rc!=AG_SUCCESS)
		{
			pgConnection.EndTransaction();
			return false;
		}

		return true;
	}


	RESULTCODE FeatureCursorStatic::OpenCursor()
	{
		char now[AUGE_PATH_MAX];
		memset(now, 0, AUGE_NAME_MAX);
		auge_get_sys_time_as_string(now, AUGE_PATH_MAX);
		memset(m_cursor_name, 0, AUGE_NAME_MAX);
		g_sprintf(m_cursor_name, "%s_%s", m_pFeatureClass->GetName(), now);
		//auge_generate_uuid(m_cursor_name, AUGE_NAME_MAX);

		std::string sql;
		SQLBuilder::BuildQueryCursor(sql, m_cursor_name, m_pFeatureClass);

		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql.c_str());
	}

	RESULTCODE FeatureCursorStatic::OpenCursor(GEnvelope& extent)
	{
		char now[AUGE_PATH_MAX];
		memset(now, 0, AUGE_NAME_MAX);
		auge_get_sys_time_as_string(now, AUGE_PATH_MAX);
		memset(m_cursor_name, 0, AUGE_NAME_MAX);
		g_sprintf(m_cursor_name, "%s_%s", m_pFeatureClass->GetName(), now);
		//auge_generate_uuid(m_cursor_name, AUGE_NAME_MAX);

		std::string sql;
		SQLBuilder::BuildQueryCursor(sql, m_cursor_name, extent, m_pFeatureClass);

		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql.c_str());
	}

	RESULTCODE FeatureCursorStatic::OpenCursor(GFilter* pFilter)
	{
		char now[AUGE_PATH_MAX];
		memset(now, 0, AUGE_NAME_MAX);
		auge_get_sys_time_as_string(now, AUGE_PATH_MAX);
		memset(m_cursor_name, 0, AUGE_NAME_MAX);
		g_sprintf(m_cursor_name, "%s_%s", m_pFeatureClass->GetName(), now);
		//auge_generate_uuid(m_cursor_name, AUGE_NAME_MAX);

		std::string sql;
		SQLBuilder::BuildQueryCursor(sql, m_cursor_name, pFilter, m_pFeatureClass);

		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql.c_str());
	}

	RESULTCODE FeatureCursorStatic::OpenCursor(GQuery*  pQuery, FeatureClassPgs* pFeatureClass)
	{
		char now[AUGE_PATH_MAX];
		memset(now, 0, AUGE_NAME_MAX);
		auge_get_sys_time_as_string(now, AUGE_PATH_MAX);
		memset(m_cursor_name, 0, AUGE_NAME_MAX);
		g_sprintf(m_cursor_name, "%s_%s", pFeatureClass->GetName(), now);
		//auge_generate_uuid(m_cursor_name, AUGE_NAME_MAX);

		std::string sql;
		SQLBuilder::BuildQueryCursor(sql, m_cursor_name, pQuery, pFeatureClass);

		return m_pWorkspace->m_pgConnection.ExecuteSQL(sql.c_str());
	}

	void FeatureCursorStatic::CloseCursor()
	{
		PGresult* pgResult;
		char sql[AUGE_PATH_MAX];
		sprintf(sql, "CLOSE %s", m_cursor_name);
		m_pWorkspace->m_pgConnection.ExecuteSQL(sql);
	}

	void FeatureCursorStatic::ClearResult()
	{
		if(m_pgResult!=NULL)
		{
			PQclear(m_pgResult);
			m_pgResult = NULL;
		}
	}

	bool FeatureCursorStatic::Fetch()
	{
		if(!m_has_more_data)
		{
			// 位于游标尾部，没有数据了
			return false;
		}
		// 记录上次获取的最大数据
		m_last_fetched_count = m_fetched_count;
		//从数据库获取数据
		char sql[AUGE_PATH_MAX];
		sprintf(sql, "FETCH %d IN %s", m_fetch_count, m_cursor_name);
		m_pgResult = m_pWorkspace->m_pgConnection.PgExecute(sql);
		int status = PQresultStatus(m_pgResult);
		if ( status != PGRES_TUPLES_OK)
		{
			const char* errmsg = PQresultErrorMessage(m_pgResult);
			augeGetLoggerInstance()->Error(errmsg, __FILE__, __LINE__);
			augeGetErrorInstance()->SetError(errmsg);

			fprintf(stderr, "FETCH ALL failed");
			PQclear(m_pgResult);
			return false;
		}
		m_fetched_count = PQntuples(m_pgResult);
		m_has_more_data = (m_fetched_count==m_fetch_count);

		if(m_pFeatureClass==NULL)
		{
			m_pFeatureClass = new FeatureClassPgs();
			m_pFeatureClass->Create(m_class_name.c_str(), m_pWorkspace, m_pgResult);

			GFields	*pFields = m_pFeatureClass->GetFields();
			GField	*pField = pFields->GetGeometryField();
			if(pField==NULL)
			{
				m_geom_findex = -1;
			}
			else
			{
				m_geom_findex = pFields->FindField(pField->GetName());
			}
		}

		return (m_fetched_count>0);
	}
}
