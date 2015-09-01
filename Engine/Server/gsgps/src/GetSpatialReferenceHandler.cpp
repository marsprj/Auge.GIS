#include "GetSpatialReferenceHandler.h"
#include "GetSpatialReferenceRequest.h"
#include "GetSpatialReferenceResponse.h"
#include "AugeService.h"
#include "AugeXML.h"

namespace auge
{
	GetSpatialReferenceHandler::GetSpatialReferenceHandler()
	{
		m_psqlite = NULL;
	}

	GetSpatialReferenceHandler::~GetSpatialReferenceHandler()
	{
		CloseBase();
	}

	const char*	GetSpatialReferenceHandler::GetName()
	{
		return "GetSpatialReference";
	}

	const char*	GetSpatialReferenceHandler::GetDescription()
	{
		return "¿Õ¼ä²Î¿¼";
	}

	WebRequest*	GetSpatialReferenceHandler::ParseRequest(rude::CGI& cgi)
	{
		GetSpatialReferenceRequest* pRequest = new GetSpatialReferenceRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetSpatialReferenceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		GetSpatialReferenceRequest* pRequest = new GetSpatialReferenceRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetSpatialReferenceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetSpatialReferenceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetSpatialReferenceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		Begin(pUser);

		GetSpatialReferenceRequest* pRequest = static_cast<GetSpatialReferenceRequest*>(pWebRequest);

		sqlite3* psqlite3 = OpenBase();
		if(psqlite3==NULL)
		{
			const char* msg = "Fail to open spatial reference database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		sqlite3_stmt *pstmt = Query(pRequest);
		if(pstmt==NULL)
		{
			const char* msg = "Fail to query spatial reference database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}
		GetSpatialReferenceResponse* pResponse = new GetSpatialReferenceResponse(pRequest);
		pResponse->SetSqlite(pstmt);
		pResponse->SetWebContenxt(pWebContext);

		End();

		return pResponse;
	}
	
	sqlite3* GetSpatialReferenceHandler::OpenBase()
	{
		if(m_psqlite!=NULL)
		{
			return m_psqlite;
		}

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
			m_psqlite = NULL;
			return NULL;
		}

		return m_psqlite;
	}

	void GetSpatialReferenceHandler::CloseBase()
	{
		if(m_psqlite!=NULL)
		{
			sqlite3_close(m_psqlite);
			m_psqlite = NULL;
		}
	}

	sqlite3_stmt* GetSpatialReferenceHandler::Query(GetSpatialReferenceRequest* pRequest)
	{
		g_int  srid	 = pRequest->GetSRID();
		g_uint count = pRequest->GetCount();
		g_uint offset= pRequest->GetOffset();
		count = count>500	? 500 : count;
		offset= offset>65535? 0 : offset;

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		if(srid<0)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select srid,srtext,proj4text from spatial_ref limit %d offset %d", count, offset);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select srid,srtext,proj4text from spatial_ref where srid=%d",srid);
		}

		sqlite3_stmt *pstmt = NULL;
		int ret = sqlite3_prepare_v2(m_psqlite, sql, strlen(sql), &pstmt, NULL);
		if(ret != SQLITE_OK)
		{
			sqlite3_finalize(pstmt);
			return NULL;
		}
		return pstmt;
	}
}
