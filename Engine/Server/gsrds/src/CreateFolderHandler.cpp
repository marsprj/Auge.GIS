#include "CreateFolderHandler.h"
#include "CreateFolderRequest.h"

#include "AugeUser.h"
#include "AugeWebCore.h"

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

		//const char* root_path = pWebContext->GetUploadPath();
		char raster_repository[AUGE_PATH_MAX];
		memset(raster_repository, 0, AUGE_PATH_MAX);
		rds_get_raster_repository(raster_repository, AUGE_PATH_MAX, pUser->GetName(), pWebContext);

		const char* rqut_path = pRequest->GetRasterPath();
		if(rqut_path==NULL)
		{
			const char* msg = "Parameter [Path] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		char local_path[AUGE_PATH_MAX];
		memset(local_path,0,AUGE_PATH_MAX);
		auge_make_path(local_path,NULL,raster_repository,rqut_path+1,NULL);

		if(!g_access(local_path,4))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Path [%s] already existed.", rqut_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		int ret = auge_mkdir(local_path);
		if(ret)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Failed to create folder [%s].", rqut_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}