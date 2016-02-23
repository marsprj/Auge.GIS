#include "GetPoiHandler.h"
#include "GetPoiRequest.h"
#include "GetPoiResponse.h"

#include "AugeProcessor.h"
#include "AugeUser.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeRaster.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

namespace auge
{
//#define AUGE_POI_TABLE	"poi_51ditu"
//#define AUGE_POI_TABLE	"poi_146m"
#define AUGE_POI_TABLE	"poi_cams"
#define AUGE_POI_SOURCE	"poi"

	GetPoiHandler::GetPoiHandler()
	{

	}

	GetPoiHandler::~GetPoiHandler()
	{

	}

	const char*	GetPoiHandler::GetName()
	{
		return "GetPoi";
	}

	const char*	GetPoiHandler::GetDescription()
	{
		return "POI检索服务";
	}

	WebRequest*	GetPoiHandler::ParseRequest(rude::CGI& cgi)
	{
		GetPoiRequest* pRequest = new GetPoiRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetPoiHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetPoiHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetPoiHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetPoiRequest* pRequest = static_cast<GetPoiRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* GetPoiHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{	
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		GetPoiRequest* pRequest = static_cast<GetPoiRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		const char* name = pRequest->GetName();
		g_uint name_count= pRequest->GetNameCount();
		
		if((name==NULL)&&(name_count==0))
		{
			const char* msg = "未定义POI查询条件";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(name!=NULL)
		{
			pWebResponse = SearchByName(pRequest,pWebContext,pUser);
		}
		else
		{
			pWebResponse = SearchByNames(pRequest,pWebContext,pUser);
		}

		return pWebResponse;
	}

	WebResponse* GetPoiHandler::SearchByName(GetPoiRequest* pRequest, WebContext* pWebContext, User* pUser)
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		const char* name = pRequest->GetName();
		GEnvelope& extent= pRequest->GetExtent();

		g_uint limit = pRequest->GetLimit();
		g_uint offset = pRequest->GetOffset();

		g_uint srid = AUGE_DEFAULT_SRID;

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		//pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), AUGE_POI_SOURCE);
		pWorkspace = pConnManager->GetWorkspace(-1, AUGE_POI_SOURCE);
		if(pWorkspace==NULL)
		{
			const char* msg = "Fail to connect poi database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		GConnection* pConnection = pWorkspace->GetConnection();
		if(pConnection==NULL)
		{
			const char* msg = "Fail to connect poi database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		if(extent.IsValid())
		{
			//const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
			//g_sprintf(sql, format, AUGE_POI_TABLE, name, limit, offset);

			const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') and  ST_Intersects(pos, st_geomfromtext('POLYGON((%.7f %.7f,%.7f %.7f,%.7f %.7f,%.7f %.7f,%.7f %.7f))',%d)) limit %d  offset %d";
			g_sprintf(sql, format, AUGE_POI_TABLE, 
									name,
									extent.m_ymin,extent.m_xmin,
									extent.m_ymin,extent.m_xmax,
									extent.m_ymax,extent.m_xmax,
									extent.m_ymax,extent.m_xmin,
									extent.m_ymin,extent.m_xmin,
									srid,
									limit, 
									offset);
		}
		else
		{
			const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
			g_sprintf(sql, format, AUGE_POI_TABLE, name, limit, offset);
		}

		GResultSet* pResult = pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		GetPoiResponse* pResponse = new GetPoiResponse(pRequest);
		pResponse->SetResultset(pResult);
		return pResponse;
	}

	WebResponse* GetPoiHandler::SearchByNames(GetPoiRequest* pRequest, WebContext* pWebContext, User* pUser)
	{
		GError* pError = augeGetErrorInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		const char* name = NULL;
		g_uint name_count = pRequest->GetNameCount();
		GEnvelope& extent= pRequest->GetExtent();

		g_uint limit = pRequest->GetLimit();
		g_uint offset = pRequest->GetOffset();

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnManager->GetWorkspace(-1, AUGE_POI_SOURCE);
		if(pWorkspace==NULL)
		{
			const char* msg = "Fail to connect poi database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		GConnection* pConnection = pWorkspace->GetConnection();
		if(pConnection==NULL)
		{
			const char* msg = "Fail to connect poi database";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		std::string sql;
		if(extent.IsValid())
		{
			//const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
			//g_sprintf(sql, format, AUGE_POI_TABLE, name, limit, offset);

			char str[AUGE_SQL_MAX];			
			sql.append("select gid,name,lat,lon,address,cdate,adcode,type from ");
			sql.append(AUGE_POI_TABLE);
			sql.append(" where (");
			
			for(g_uint i=0; i<name_count; i++)
			{
				name = pRequest->GetName(i);
				g_snprintf(str, AUGE_SQL_MAX, "tsv@@ to_tsquery('%s')", name);

				if(i>0)
				{
					sql.append(" or ");
				}
				sql.append(str);
			}
			sql.append(")");

			g_snprintf(str,AUGE_SQL_MAX," ST_Intersects(pos, st_geomfromtext('POLYGON((%.7f %.7f,%.7f %.7f,%.7f %.7f,%.7f %.7f,%.7f %.7f))',%d))", 
				extent.m_ymin,extent.m_xmin,
				extent.m_ymin,extent.m_xmax,
				extent.m_ymax,extent.m_xmax,
				extent.m_ymax,extent.m_xmin,
				extent.m_ymin,extent.m_xmin,
				AUGE_DEFAULT_SRID);

			sql.append(" and (");
			sql.append(str);
			sql.append(")");

			g_snprintf(str, AUGE_SQL_MAX, " limit %d  offset %d", limit, offset);
			sql.append(str);
		}
		else
		{
			//const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
			//g_sprintf(sql, format, AUGE_POI_TABLE, name, limit, offset);

			char str[AUGE_SQL_MAX];			
			sql.append("select gid,name,lat,lon,address,cdate,adcode,type from ");
			sql.append(AUGE_POI_TABLE);
			sql.append(" where (");

			for(g_uint i=0; i<name_count; i++)
			{
				name = pRequest->GetName(i);
				g_snprintf(str, AUGE_SQL_MAX, "tsv@@ to_tsquery('%s')", name);

				if(i>0)
				{
					sql.append(" or ");
				}
				sql.append(str);
			}
			sql.append(")");

			g_snprintf(str, AUGE_SQL_MAX, " limit %d  offset %d", limit, offset);
			sql.append(str);
		}

		GResultSet* pResult = pConnection->ExecuteQuery(sql.c_str());
		if(pResult==NULL)
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		GetPoiResponse* pResponse = new GetPoiResponse(pRequest);
		pResponse->SetResultset(pResult);
		return pResponse;
	}

	//WebResponse* GetPoiHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	//{
	//	GError* pError = augeGetErrorInstance();
	//	GLogger* pLogger = augeGetLoggerInstance();
	//	GetPoiRequest* pRequest = static_cast<GetPoiRequest*>(pWebRequest);

	//	WebResponse* pWebResponse = NULL;

	//	const char* name = pRequest->GetName();
	//	GEnvelope& extent= pRequest->GetExtent();
	//	g_uint limit = pRequest->GetLimit();
	//	g_uint offset = pRequest->GetOffset();

	//	if(name==NULL||strlen(name)==0)
	//	{
	//		const char* msg = "Parameter [name] is NULL";
	//		pError->SetError(msg);
	//		pLogger->Error(msg, __FILE__, __LINE__);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	Workspace* pWorkspace = NULL;
	//	ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
	//	//pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), AUGE_POI_SOURCE);
	//	pWorkspace = pConnManager->GetWorkspace(-1, AUGE_POI_SOURCE);
	//	if(pWorkspace==NULL)
	//	{
	//		const char* msg = "Fail to connect poi database";
	//		pError->SetError(msg);
	//		pLogger->Error(msg, __FILE__, __LINE__);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	GConnection* pConnection = pWorkspace->GetConnection();
	//	if(pConnection==NULL)
	//	{
	//		const char* msg = "Fail to connect poi database";
	//		pError->SetError(msg);
	//		pLogger->Error(msg, __FILE__, __LINE__);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	//const char* name_encoded = NULL;
	//	//if(pWebContext->IsIE())
	//	//	//name_encoded = auge_encoding_convert("GBK","UTF-8",name,strlen(name));
	//	//	name_encoded = name;
	//	//else
	//	//	//name_encoded = name;
	//	//	name_encoded = auge_encoding_convert("UTF-8","GBK",name,strlen(name));

	//	char sql[AUGE_SQL_MAX];
	//	memset(sql, 0, AUGE_SQL_MAX);
	//	if(extent.IsValid())
	//	{
	//		const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
	//		g_sprintf(sql, format, AUGE_POI_TABLE, name, limit, offset);
	//	}
	//	else
	//	{
	//		const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
	//		g_sprintf(sql, format, AUGE_POI_TABLE, name, limit, offset);
	//	}

	//	GResultSet* pResult = pConnection->ExecuteQuery(sql);
	//	if(pResult==NULL)
	//	{
	//		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	GetPoiResponse* pResponse = new GetPoiResponse(pRequest);
	//	pResponse->SetResultset(pResult);
	//	return pResponse;
	//}
}