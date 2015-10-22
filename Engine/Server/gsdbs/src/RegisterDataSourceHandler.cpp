#include "RegisterDataSourceHandler.h"
#include "RegisterDataSourceRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	RegisterDataSourceHandler::RegisterDataSourceHandler()
	{

	}

	RegisterDataSourceHandler::~RegisterDataSourceHandler()
	{

	}

	const char*	RegisterDataSourceHandler::GetName()
	{
		return "RegisterDataSource";
	}

	const char*	RegisterDataSourceHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RegisterDataSourceHandler::ParseRequest(rude::CGI& cgi)
	{
		RegisterDataSourceRequest* pRequest = new RegisterDataSourceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RegisterDataSourceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RegisterDataSourceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RegisterDataSourceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		char source_uri[AUGE_PATH_MAX];
		const char* name = NULL;
		const char* engine = NULL;
		const char* uri = NULL;
		augeWorkspaceType type = augeWorkspaceUnknown;
		WebResponse* pWebResponse = NULL;
		RegisterDataSourceRequest* pRequest = static_cast<RegisterDataSourceRequest*>(pWebRequest);

		name = pRequest->GetName();
		engine = pRequest->GetDataEngine();
		uri = pRequest->GetURI();
		type = pRequest->GetSourceType();
		if(uri==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Parameter [URI] is NULL");
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);
			return pExpResponse;
		}
		
		memset(source_uri, 0, AUGE_PATH_MAX);
		if(type==augeWorkspaceRaster)
		{	
			char cwd[AUGE_PATH_MAX];
			memset(cwd, 0, AUGE_PATH_MAX);
			auge_get_cwd(cwd, AUGE_PATH_MAX);

			char user_path[AUGE_PATH_MAX];
			memset(user_path, 0, AUGE_PATH_MAX);
#ifdef WIN32
			char home_path[AUGE_PATH_MAX];
			memset(home_path, 0, AUGE_PATH_MAX); 
			auge_get_parent_dir(cwd, home_path, AUGE_PATH_MAX);
			auge_make_path(user_path, home_path, "user", pUser->GetName(), NULL);
#else
			auge_make_path(user_path, cwd, "user", pUser->GetName(), NULL);
#endif
			auge_make_path(user_path, cwd, "user", pUser->GetName(), NULL);
			char rds_path[AUGE_PATH_MAX];
			memset(rds_path, 0, AUGE_PATH_MAX);
			auge_make_path(rds_path, NULL, user_path, "rds", NULL);
		
			strcpy(source_uri, uri);
			strcat(source_uri, ";repository=");
			strcat(source_uri, rds_path);
		}
		else
		{
			strcpy(source_uri, uri);
		}

		ConnectionManager*	pConnManager = augeGetConnectionManagerInstance();
		RESULTCODE rc = pConnManager->Register(pUser->GetID(), name, engine, source_uri, type);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* RegisterDataSourceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}