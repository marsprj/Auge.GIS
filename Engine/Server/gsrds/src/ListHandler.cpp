#include "ListHandler.h"
#include "ListRequest.h"
#include "ListResponse.h"

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
	extern void rds_get_raster_repository(char* raster_repository, size_t size, const char* user_name, WebContext* pWebContext);

	ListHandler::ListHandler()
	{

	}

	ListHandler::~ListHandler()
	{

	}

	const char*	ListHandler::GetName()
	{
		return "List";
	}

	const char*	ListHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	ListHandler::ParseRequest(rude::CGI& cgi)
	{
		ListRequest* pRequest = new ListRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	ListHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	ListHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* ListHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		ListRequest* pRequest = static_cast<ListRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* ListHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		ListRequest* pRequest = static_cast<ListRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		const char* path = pRequest->GetPath();

		if(sourceName==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pRasterWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get DataSource [%s]", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		if(!pWorkspace->IsOpen())
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Connect to DataSource [%s]", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		pRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		RasterFolder* pFolder = pRasterWorkspace->GetFolder(path);
		if(pFolder==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Path [%s] does not exist.", path);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		ListResponse* pResponse = new ListResponse(pRequest);
		pResponse->SetFolder(pFolder);

		return pResponse;
	}
}
