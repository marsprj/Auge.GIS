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
#define AUGE_POI_TABLE	"poi_51ditu"
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
		return "POI¼ìË÷·þÎñ";
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
		GEnvelope& extent= pRequest->GetExtent();
		g_uint limit = pRequest->GetLimit();
		g_uint offset = pRequest->GetOffset();

		if(name==NULL||strlen(name)==0)
		{
			const char* msg = "Parameter [Path] is NULL";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), AUGE_POI_SOURCE);
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

		const char* name_encoded = NULL;
		if(pWebContext->IsIE())
			//name_encoded = auge_encoding_convert("GBK","UTF-8",name,strlen(name));
			name_encoded = name;
		else
			//name_encoded = name;
			name_encoded = auge_encoding_convert("UTF-8","GBK",name,strlen(name));

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		if(extent.IsValid())
		{
			const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
			g_sprintf(sql, format, AUGE_POI_TABLE, name_encoded, limit, offset);
		}
		else
		{
			const char* format = "select gid,name,lat,lon,address,cdate,adcode,type from %s where tsv@@ to_tsquery('%s') limit %d  offset %d";
			g_sprintf(sql, format, AUGE_POI_TABLE, name_encoded, limit, offset);
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
}