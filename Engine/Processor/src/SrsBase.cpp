#include "SrsBase.h"
#include "AugeCore.h"

namespace auge
{
	SrsBase::SrsBase()
	{

	}

	SrsBase::~SrsBase()
	{

	}

	RESULTCODE SrsBase::Open()
	{
		char cwd[AUGE_PATH_MAX];
		memset(cwd, 0, AUGE_PATH_MAX);
		auge_get_cwd(cwd,AUGE_PATH_MAX);

		char dbpath[AUGE_PATH_MAX];
		memset(dbpath, 0, AUGE_PATH_MAX);

		auge_make_path(dbpath,NULL,cwd,"srs.db",NULL);

		sqlite3_enable_shared_cache(1);
		int ret = sqlite3_open(dbpath, &m_psqlite);
		if(ret!=SQLITE_OK)
		{
			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	void SrsBase::Close()
	{
		sqlite3_close(m_psqlite);
	}

	RESULTCODE SrsBase::GetProj4Text(g_uint srid, char* text, size_t size)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select proj4text from spatial_ref where srid=%d",srid);

		int ret = SQLITE_OK;
		sqlite3_stmt *pstmt = NULL;
		ret = sqlite3_prepare_v2(m_psqlite, sql, strlen(sql), &pstmt, NULL);
		ret = sqlite3_step(pstmt);
		if(ret == SQLITE_ROW)
		{
			const char* value = (const char*)sqlite3_column_text(pstmt, 0);
			strcpy(text,value);
			ret = sqlite3_finalize(pstmt);

			return AG_SUCCESS;
		}
		ret = sqlite3_finalize(pstmt);
		
		return AG_FAILURE;
	}

	RESULTCODE SrsBase::GetSrsText(g_uint srid, char* text, size_t size)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select srtext from spatial_ref where srid=%d",srid);

		int ret = SQLITE_OK;
		sqlite3_stmt *pstmt = NULL;
		ret = sqlite3_prepare_v2(m_psqlite, sql, strlen(sql), &pstmt, NULL);
		ret = sqlite3_step(pstmt);
		if(ret == SQLITE_ROW)
		{
			const char* value = (const char*)sqlite3_column_text(pstmt, 0);
			strcpy(text,value);
			ret = sqlite3_finalize(pstmt);

			return AG_SUCCESS;
		}
		ret = sqlite3_finalize(pstmt);

		return AG_FAILURE;
	}

}