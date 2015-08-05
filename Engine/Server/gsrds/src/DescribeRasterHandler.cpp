#include "DescribeRasterHandler.h"
#include "DescribeRasterRequest.h"
#include "DescribeRasterResponse.h"

#include "AugeUser.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeRaster.h"

namespace auge
{
	DescribeRasterHandler::DescribeRasterHandler()
	{

	}

	DescribeRasterHandler::~DescribeRasterHandler()
	{

	}

	const char*	DescribeRasterHandler::GetName()
	{
		return "DescribeRaster";
	}

	WebRequest*	DescribeRasterHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeRasterRequest* pRequest = new DescribeRasterRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	DescribeRasterHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeRasterHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeRasterHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		DescribeRasterRequest* pRequest = static_cast<DescribeRasterRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* DescribeRasterHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		DescribeRasterRequest* pRequest = static_cast<DescribeRasterRequest*>(pWebRequest);

		const char* root_path = pWebContext->GetUploadPath();
		const char* folder_path = pRequest->GetPath();
		const char* raster_name=pRequest->GetRasterName();

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

		RESULTCODE rc = AG_FAILURE;
		RasterDataset* pRasterDataset = pFolder->GetRasterDataset();
		Raster* pRaster = pRasterDataset->GetRaster(raster_name);
		if(pRaster==NULL)
		{
			pFolder->Release();

			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Raster [%s/%s] does not exist.", folder_path, raster_name);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		pFolder->Release();
		DescribeRasterResponse* pDescribeRasterResponse = new DescribeRasterResponse(pRequest);
		pDescribeRasterResponse->SetRaster(pRaster);

		return pDescribeRasterResponse;
	}
}