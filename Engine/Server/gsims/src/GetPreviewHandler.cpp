#include "GetPreviewHandler.h"
#include "GetPreviewRequest.h"
#include "GetPreviewResponse.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeStyle.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeCore.h"

namespace auge
{
	GetPreviewHandler::GetPreviewHandler()
	{

	}

	GetPreviewHandler::~GetPreviewHandler()
	{

	}

	const char*	GetPreviewHandler::GetName()
	{
		return "GetPreview";
	}

	WebRequest*	GetPreviewHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		GetPreviewRequest* pRequest = new GetPreviewRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetPreviewHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* GetPreviewHandler::Execute(WebRequest* pWebRequest)
	{
		GetPreviewRequest* pRequest = static_cast<GetPreviewRequest*>(pWebRequest);

		const char* sourceName = pRequest->GetSourceName();
		const char* dataSetName= pRequest->GetDataSetName();

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
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(sourceName);
		if( pWorkspace==NULL )
		{
			char msg[AUGE_PATH_MAX] = {0};
			g_sprintf(msg, "Cannot load datasource [%s].", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		DataSet* pDataSet = pWorkspace->OpenDataSet(dataSetName);
		if(pDataSet==NULL)
		{
			char msg[AUGE_PATH_MAX] = {0};
			g_sprintf(msg, "Cannot load data set [%s].", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		WebResponse* pWebResponse = NULL;
		switch(pDataSet->GetType())
		{
		case augeDataSetFeature:
			pWebResponse = DrawFeature(static_cast<FeatureClass*>(pDataSet),pRequest);
			break;
		case augeDataSetRaster:
			break;
		}

		return pWebResponse;
	}

	WebResponse* GetPreviewHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}

	WebResponse* GetPreviewHandler::DrawFeature(FeatureClass* pFeatureClass, GetPreviewRequest* pRequest)
	{
		g_uint width  = pRequest->GetWidth();
		g_uint height = pRequest->GetHeight();

		Canvas* pCanvas = NULL;
		CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();
		GLogger* pLogger = augeGetLoggerInstance();

		Style* pStyle = NULL;
		FeatureLayer* pFeatureLayer = NULL;
		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		StyleFactory* pStyleFactory = augeGetStyleFactoryInstance();

		pFeatureLayer = pCartoFactory->CreateFeatureLayer();
		pFeatureLayer->SetFeatureClass(pFeatureClass);
		pFeatureClass->AddRef();

		GField* pField = pFeatureClass->GetFields()->GetGeometryField();
		augeGeometryType type = pField->GetGeometryDef()->GeometryType();
		pStyle = pStyleFactory->CreateFeatureStyle(type);

		GColor bgColor(255,255,255,255);
		pCanvas = pCartoFactory->CreateCanvas2D(width, height);
		pCanvas->DrawBackground(bgColor);

		GEnvelope extent = pRequest->GetExtent();
		pCanvas->SetViewer(extent.IsValid() ? extent : pFeatureClass->GetExtent());
		
		pCanvas->DrawLayer(pFeatureLayer, pStyle);

		pStyle->Release();
		pFeatureLayer->Release();
		
		char img_sfix[AUGE_EXT_MAX] = {0};
		char img_name[AUGE_NAME_MAX] = {0};
		char img_path[AUGE_PATH_MAX] = {0};
		auge_get_image_suffix(pRequest->GetMimeType(), img_sfix, AUGE_EXT_MAX);
		auge_generate_uuid(img_name, AUGE_NAME_MAX);
		const char* cache_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\cache\\map";//pWebContext->GetCacheMapPath();
		auge_make_path(img_path, NULL, cache_path, img_name, img_sfix);
		pCanvas->Save(img_path);
		pCanvas->Release();
		GetPreviewResponse* pMapResponse = new GetPreviewResponse(pRequest);
		pMapResponse->SetPath(img_path);

		return pMapResponse;
	}
}