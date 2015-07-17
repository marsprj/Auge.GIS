#include "GetValueHandler.h"
#include "GetValueRequest.h"
#include "GetValueResponse.h"

#include "AugeUser.h"
#include "AugeRaster.h"
#include "AugeData.h"
#include "AugeCore.h"
#include "AugeWebCore.h"


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

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetValueRequest* pRequest = static_cast<GetValueRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* GetValueHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
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

		RasterDataset* pRasterDataset = NULL;
		pRasterDataset = pRasterWorkspace->OpenRasterDataset(raster_name);
		if(pRasterDataset==NULL)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Fail to get Raster [%s]", raster_name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		Raster* pRaster = pRasterDataset->GetRaster();

		g_int rx=0, ry=0;
		if((pRequest->GetIX()>=0)&&(pRequest->GetIX()>=0))
		{
			rx = pRequest->GetIX();
			ry = pRequest->GetIY();
		}
		else
		{
			pRaster->GetRasterPosition(pRequest->GetX(), pRequest->GetY(),rx, ry);
		}
		
		g_uint width = pRaster->GetWidth();
		g_uint height = pRaster->GetHeight();
		if((rx<0||rx>width)||((ry<0||ry>height)))
		{
			pRasterDataset->Release();

			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"[%d %d] is out of bounding box", rx,ry);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		GetValueResponse* pWebResponse = new GetValueResponse(pRequest);
		pWebResponse->Create(pRaster->GetBandCount());

		char str[AUGE_NAME_MAX];
		RasterBand* pBand = NULL;
		g_uint nband = pRaster->GetBandCount();
		for(g_uint i=0; i<nband; i++)
		{
			pBand = pRaster->GetBand(i);
			void* val = pBand->GetData(rx,ry);

			memset(str,0,AUGE_NAME_MAX);
			switch(pRaster->GetPixelType())
			{
			case augePixelByte:
				g_sprintf(str,"%d",*(g_uchar*)val);
				break;
			case augePixelUInt16:
				g_sprintf(str,"%d",*(unsigned short*)val);
				break;
			case augePixelInt16:
				g_sprintf(str,"%d",*(short*)val);
				break;
			case augePixelUInt32:
				g_sprintf(str,"%d",*(unsigned int*)val);
				break;
			case augePixelInt32:
				g_sprintf(str,"%d",*(int*)val);
				break;
			case augePixelFloat32:
				g_sprintf(str,"%f",*(float*)val);
				break;
			case augePixelDouble:
				g_sprintf(str,"%f",*(double*)val);
				break;
			}

			pWebResponse->SetValue(i,i,str);
		}

		pRasterDataset->Release();
		
		return pWebResponse;
	}
}