#include "GetValueHandler.h"
#include "GetValueRequest.h"
#include "GetValueResponse.h"

#include "AugeWebCore.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

namespace auge
{
	GetValueHandler::GetValueHandler()
	{

	}

	GetValueHandler::~GetValueHandler()
	{

	}

	const char*	GetValueHandler::GetName()
	{
		return "GetValue";
	}

	WebRequest*	GetValueHandler::ParseRequest(rude::CGI& cgi)
	{
		GetValueRequest* pRequest = new GetValueRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetValueHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetValueHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest)
	{
		GetValueRequest* pRequest = static_cast<GetValueRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		GetValueRequest* pRequest = static_cast<GetValueRequest*>(pWebRequest);

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
		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			const char* msg = "Parameter [SourceName] is NULL";
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

		char raster_path[AUGE_PATH_MAX];
		memset(raster_path,0,AUGE_PATH_MAX);
		auge_make_path(raster_path,NULL,root_path,rqut_path+1,NULL);

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

		Workspace* pWorkspace = NULL;
		RasterWorkspace* pRasterWorkspace = NULL;
		ConnectionManager* pConnectionManager = NULL;
		pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(sourceName);
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

		RasterDataset* pRasterDataset = NULL;
		pRasterDataset = pRasterWorkspace->OpenRasterDataset(raster_name);
		if(pRasterDataset==NULL)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Fail to get Raster [%s]", raster_name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);

			return pExpResponse;
		}

		RasterBand* pBand = NULL;
		Raster* pRaster = pRasterDataset->GetRaster();

		int rx=0; ry=0;
		if((pRequest->GetIX()>=0)&&(pRequest->GetIX()>=0))
		{
			rx = pRequest->GetIX();
			ry = pRequest->GetIY();
		}
		else
		{
			pRaster->GetRasterPosition(pRequest->GetX(), pRequest->GetY(),&rx, &ry);
		}
		
		g_uint width = pRaster->GetWidth();
		g_uint height = pRaster->GetHeight();

		g_uint nband = pRaster->GetBandCount();
		for(g_uint i=0; i<nband; i++)
		{
			pBand = pRaster->GetBand(i);
			
		}

		pRasterDataset->Release();

		
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetName());
		return pSusResponse;
	}
}