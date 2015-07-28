#include "RemoveRasterHandler.h"
#include "RemoveRasterRequest.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeUser.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

namespace auge
{
	extern void rds_get_raster_repository(char* raster_repository, size_t size, const char* user_name, WebContext* pWebContext);

	RemoveRasterHandler::RemoveRasterHandler()
	{

	}

	RemoveRasterHandler::~RemoveRasterHandler()
	{

	}

	const char*	RemoveRasterHandler::GetName()
	{
		return "RemoveRaster";
	}

	WebRequest*	RemoveRasterHandler::ParseRequest(rude::CGI& cgi)
	{
		RemoveRasterRequest* pRequest = new RemoveRasterRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RemoveRasterHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveRasterHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* RemoveRasterHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		RemoveRasterRequest* pRequest = static_cast<RemoveRasterRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* RemoveRasterHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		RemoveRasterRequest* pRequest = static_cast<RemoveRasterRequest*>(pWebRequest);

		//const char* root_path = pWebContext->GetUploadPath();		
		const char* rqut_path = pRequest->GetRasterPath();
		const char* raster_name=pRequest->GetRasterName();
		const char* raster_path=pRequest->GetRasterPath();

		//计算raster的根目录
		char raster_repository[AUGE_PATH_MAX];
		memset(raster_repository, 0, AUGE_PATH_MAX);
		rds_get_raster_repository(raster_repository, AUGE_PATH_MAX, pUser->GetName(), pWebContext);
		char raster_local_folder[AUGE_PATH_MAX];
		auge_make_path(raster_local_folder, NULL, raster_repository, raster_path, NULL);
		char raster_local_path[AUGE_PATH_MAX];
		auge_make_path(raster_local_path, NULL, raster_local_folder, raster_name, NULL);


		if(raster_name==NULL)
		{
			const char* msg = "Parameter [rasterName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}
		if(raster_path==NULL)
		{
			const char* msg = "Parameter [rasterPath] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}
		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			const char* msg = "Parameter [SourceName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}
		//if(rqut_path==NULL)
		//{
		//	const char* msg = "Parameter [Path] is NULL";
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	pLogger->Error(msg,__FILE__,__LINE__);

		//	return pExpResponse;
		//}

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pRasterWorkspace = NULL;
		ConnectionManager* pConnectionManager = NULL;
		pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(pUser->GetID(), sourceName);
		if(pWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Cannot Get DataSource [%s]", sourceName);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		pRasterWorkspace = dynamic_cast<RasterWorkspace*>(pWorkspace);
		//if(pRasterWorkspace==NULL)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	memset(msg,0,AUGE_MSG_MAX);
		//	g_sprintf(msg,"Cannot Get DataSource [%s]", sourceName);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	pLogger->Error(msg,__FILE__,__LINE__);

		//	return pExpResponse;
		//}

		RESULTCODE rc = pRasterWorkspace->RemoveRaster(raster_name, raster_path);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);

			return pExpResponse; 
		}

		auge_remove_file(raster_local_path);
		
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}