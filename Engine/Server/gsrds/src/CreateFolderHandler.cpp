#include "CreateFolderHandler.h"
#include "CreateFolderRequest.h"

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

	CreateFolderHandler::CreateFolderHandler()
	{

	}

	CreateFolderHandler::~CreateFolderHandler()
	{

	}

	const char*	CreateFolderHandler::GetName()
	{
		return "CreateFolder";
	}

	WebRequest*	CreateFolderHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateFolderRequest* pRequest = new CreateFolderRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	CreateFolderHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateFolderHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* CreateFolderHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		CreateFolderRequest* pRequest = static_cast<CreateFolderRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* CreateFolderHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		CreateFolderRequest* pRequest = static_cast<CreateFolderRequest*>(pWebRequest);

		const char* source_name = pRequest->GetSourceName();
		const char* folder_path = pRequest->GetPath();

		if(folder_path==NULL)
		{
			const char* msg = "Parameter [Path] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		if(source_name==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pRasterWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnManager->GetWorkspace(pUser->GetID(), source_name);
		if(pWorkspace==NULL)
		{ 
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get DataSource [%s]", source_name);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		pRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);

		RasterFolder* pFolder = pRasterWorkspace->CreateFolder(folder_path);
		if(pFolder==NULL)
		{
			GError* pError = augeGetErrorInstance();			
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}