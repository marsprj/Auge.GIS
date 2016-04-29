#include "GetThumbnailHandler.h"
#include "GetThumbnailRequest.h"
#include "GetThumbnailResponse.h"

#include "AugeUser.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeRaster.h"

namespace auge
{
	GetThumbnailHandler::GetThumbnailHandler()
	{

	}

	GetThumbnailHandler::~GetThumbnailHandler()
	{

	}

	const char*	GetThumbnailHandler::GetName()
	{
		return "GetThumbnail";
	}

	const char*	GetThumbnailHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetThumbnailHandler::ParseRequest(rude::CGI& cgi)
	{
		GetThumbnailRequest* pRequest = new GetThumbnailRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetThumbnailHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetThumbnailHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetThumbnailHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetThumbnailRequest* pRequest = static_cast<GetThumbnailRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* GetThumbnailHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		GetThumbnailRequest* pRequest = static_cast<GetThumbnailRequest*>(pWebRequest);

		const char* folder_path = pRequest->GetPath();
		const char* raster_name=pRequest->GetRasterName();
		char root_path[AUGE_PATH_MAX];
		auge_make_user_raster_root(root_path, AUGE_PATH_MAX, pWebContext->GetUserRoot(), pUser->GetName());

		const char* sourceName = pRequest->GetSourceName();
		if(sourceName==NULL)
		{
			const char* msg = "Parameter [SourceName] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

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

		RasterFolder* pFolder = pRasterWorkspace->GetFolder(folder_path);
		if(pFolder==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Path [%s] does not exist.", folder_path);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		WebResponse* pWebResponse = NULL;
		RESULTCODE rc = AG_FAILURE;
		RasterDataset* pRasterDataset = pFolder->GetRasterDataset();
		if(raster_name==NULL)
		{
			EnumRaster* pRasters = pRasterDataset->GetRasters();
			GetThumbnailResponse* pGetThumbnailResponse = new GetThumbnailResponse(pRequest);
			pGetThumbnailResponse->SetRasters(pRasters);
			pWebResponse = pGetThumbnailResponse;
		}
		else
		{
			Raster* pRaster = pRasterDataset->GetRaster(raster_name);
			if(pRaster==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Raster [%s/%s] does not exist.", folder_path, raster_name);
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error(msg, __FILE__, __LINE__);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse =  pExpResponse;
			}
			else
			{
				GetThumbnailResponse* pGetThumbnailResponse = new GetThumbnailResponse(pRequest);
				pGetThumbnailResponse->SetRaster(pRaster);
				pWebResponse = pGetThumbnailResponse;
			}
		}
		pFolder->Release();		

		return pWebResponse;
	}
}