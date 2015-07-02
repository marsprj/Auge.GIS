#include "GetRasterHandler.h"
#include "GetRasterRequest.h"
#include "GetRasterResponse.h"

#include "AugeWebCore.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

namespace auge
{
	GetRasterHandler::GetRasterHandler()
	{

	}

	GetRasterHandler::~GetRasterHandler()
	{

	}

	const char*	GetRasterHandler::GetName()
	{
		return "GetRaster";
	}

	WebRequest*	GetRasterHandler::ParseRequest(rude::CGI& cgi)
	{
		GetRasterRequest* pRequest = new GetRasterRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetRasterHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetRasterHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetRasterHandler::Execute(WebRequest* pWebRequest)
	{
		GetRasterRequest* pRequest = static_cast<GetRasterRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* GetRasterHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		GetRasterRequest* pRequest = static_cast<GetRasterRequest*>(pWebRequest);

		const char* root_path = pWebContext->GetUploadPath();
		const char* rqut_path = pRequest->GetPath();
		const char* raster_name=pRequest->GetName();

		if(raster_name==NULL)
		{
			const char* msg = "Parameter [Name] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

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
		auge_make_path(local_path,NULL,root_path,rqut_path+1,NULL);

		char raster_path[AUGE_PATH_MAX];
		memset(raster_path,0,AUGE_PATH_MAX);
		auge_make_path(raster_path,NULL,local_path,raster_name,NULL);

		if(g_access(raster_path,4))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Folder [%s] does not have Raster [%s] in folder  does not exist.", rqut_path, raster_name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		GetRasterResponse* pGetRasterResponse = new GetRasterResponse(pRequest);
		pGetRasterResponse->SetPath(raster_path);

		return pGetRasterResponse;
	}
}