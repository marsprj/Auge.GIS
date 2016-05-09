#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"
#include "AugeService.h"

#include "GProcessEngine.h"
#include "CapabilitiesHandler.h"

#include "GetAreaHandler.h"
#include "GetLengthHandler.h"

#include "CentroidHandler.h"
#include "BufferHandler.h"
#include "ConvexHullHandler.h"

#include "TileUpdateHandler.h"

#include "BuildPyramidHandler.h"
#include "FeatureImportHandler.h"
#include "FeatureProjectHandler.h"
#include "CsvImportHandler.h"

#include "MultiPointToPointsHandler.h"
#include "LineToPointsHandler.h"
#include "PolygonToPointsHandler.h"
#include "PolygonToLineHandler.h"
#include "GenerateRandomPointsHandler.h"
#include "GenerateRandomPointsInPolygonHandler.h"

#include "RasterEdgeDetectHandler.h"
#include "RasterExtractByRectangleHandler.h"
#include "RasterExtractByPolygonHandler.h"
#include "RasterReverseHandler.h"
#include "RasterGraylizeHandler.h"
#include "RasterSmoothHandler.h"
#include "RasterStretchHandler.h"
#include "RasterSubtractHandler.h"
#include "RasterPixelBlendHandler.h"
#include "RasterThresholdHandler.h"
#include "RasterHistogramEqualizationHandler.h"
#include "RasterMosiacHandler.h"
#include "RasterMaskHandler.h"
#include "RasterResampleHandler.h"

#include "RasterSepiaToneHandler.h"

#include "DemSlopeHandler.h"
#include "DemAspectHandler.h"
#include "DemStretchHandler.h"
#include "DemHillshadeHandler.h"
#include "DemInundationHandler.h"
#include "DelaunayHandler.h"

#include "ClusterKmeanHandler.h"

#include "GetSpatialReferenceHandler.h"
#include "GetSpatialReferenceCountHandler.h"
//#include "GetJobHandler.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static GeoProcessingEngine g_webProcessEngine;
		return &g_webProcessEngine;
	} 

	GeoProcessingEngine::GeoProcessingEngine()
	{
		m_handler = NULL;

		m_pcapHandler = new GeoProcessingCapabilitiesHandler(this);

		m_geometry_handlers.push_back(new GetAreaHandler());
		m_geometry_handlers.push_back(new GetLengthHandler());

		m_geometry_handlers.push_back(new BufferHandler());
		m_geometry_handlers.push_back(new CentroidHandler());
		m_geometry_handlers.push_back(new ConvexHullHandler());
		m_geometry_handlers.push_back(new MultiPointToPointsHandler());
		m_geometry_handlers.push_back(new LineToPointsHandler());
		m_geometry_handlers.push_back(new PolygonToPointsHandler());
		m_geometry_handlers.push_back(new PolygonToLineHandler());
		m_geometry_handlers.push_back(new GenerateRandomPointsHandler());
		m_geometry_handlers.push_back(new GenerateRandomPointsInPolygonHandler());

		m_tile_handlers.push_back(new BuildPyramidHandler());
		m_tile_handlers.push_back(new UpdateTileHandler());

		m_feature_handlers.push_back(new FeatureImportHandler());
		m_feature_handlers.push_back(new FeatureProjectHandler());
		m_feature_handlers.push_back(new CsvImportHandler());

		m_raster_handlers.push_back(new RasterEdgeDetectHandler());
		m_raster_handlers.push_back(new RasterExtractByRectangleHandler());
		m_raster_handlers.push_back(new RasterExtractByPolygonHandler());
		m_raster_handlers.push_back(new RasterReverseHandler());
		m_raster_handlers.push_back(new RasterGraylizeHandler());
		m_raster_handlers.push_back(new RasterSmoothHandler());
		m_raster_handlers.push_back(new RasterStretchHandler());
		m_raster_handlers.push_back(new RasterSubtractHandler());
		m_raster_handlers.push_back(new RasterPixelBlendHandler());
		m_raster_handlers.push_back(new RasterThresholdHandler());
		m_raster_handlers.push_back(new RasterHistogramEqualizationHandler());
		m_raster_handlers.push_back(new RasterMosiacHandler());
		m_raster_handlers.push_back(new RasterMaskHandler());
		m_raster_handlers.push_back(new RasterResampleHandler());

		m_raster_handlers.push_back(new RasterSepiaToneHandler());
		
		m_dem_handlers.push_back(new DemSlopeHandler());
		m_dem_handlers.push_back(new DemAspectHandler());
		m_dem_handlers.push_back(new DemStretchHandler());
		m_dem_handlers.push_back(new DemHillshadeHandler());
		m_dem_handlers.push_back(new DelaunayHandler());
		m_dem_handlers.push_back(new DemInundationHandler());

		m_cluster_handlers.push_back(new KMeanHandler());

		m_misc_handlers.push_back(new GetSpatialReferenceHandler());
		m_misc_handlers.push_back(new GetSpatialReferenceCountHandler());
	}

	GeoProcessingEngine::~GeoProcessingEngine()
	{
		m_handler = NULL; 
		m_pcapHandler->Release();
		CleanupHandlers();
	}

	void* GeoProcessingEngine::GetHandler()
	{
		return m_handler;
	}

	void GeoProcessingEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	void GeoProcessingEngine::CleanupHandlers()
	{
		CleanupHandlers(m_feature_handlers);
		CleanupHandlers(m_raster_handlers);
		CleanupHandlers(m_dem_handlers);
		CleanupHandlers(m_geometry_handlers);
		CleanupHandlers(m_tile_handlers);
		CleanupHandlers(m_cluster_handlers);
		CleanupHandlers(m_misc_handlers);
	}

	void GeoProcessingEngine::CleanupHandlers(std::vector<WebHandler*>& handlers)
	{
		WebHandler* pHandler = NULL;
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			pHandler = *iter;
			pHandler->Release();
		}
		handlers.clear();
	}

	WebHandler*	GeoProcessingEngine::GetHandler(const char* name)
	{
		if(g_stricmp(m_pcapHandler->GetName(), name)==0)
		{
			return m_pcapHandler;
		}

		WebHandler* handler = NULL;
		handler = GetHandler(name, m_feature_handlers);
		if(handler!=NULL)
		{
			return handler;
		}
		handler = GetHandler(name, m_raster_handlers);
		if(handler!=NULL)
		{
			return handler;
		}
		handler = GetHandler(name, m_dem_handlers);
		if(handler!=NULL)
		{
			return handler;
		}
		handler = GetHandler(name, m_geometry_handlers);
		if(handler!=NULL)
		{
			return handler;
		}
		handler = GetHandler(name, m_tile_handlers);
		if(handler!=NULL)
		{
			return handler;
		}
		handler = GetHandler(name, m_cluster_handlers);
		if(handler!=NULL)
		{
			return handler;
		}

		handler = GetHandler(name, m_misc_handlers);
		if(handler!=NULL)
		{
			return handler;
		}
		return NULL;
	}

	WebHandler*	GeoProcessingEngine::GetHandler(const char* name, std::vector<WebHandler*>& handlers)
	{
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			if(g_stricmp((*iter)->GetName(), name)==0)
			{
				return *iter; 
			}
		}
		return NULL;
	}

	const char*	GeoProcessingEngine::GetID()
	{
		return "GeoProcessingEngine";
	}

	const char*	GeoProcessingEngine::GetType()
	{
		return "gps";
	}

	const char*	GeoProcessingEngine::GetDescription()
	{
		return "GeoProcessing Service";
	}

	const char*	GeoProcessingEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	GeoProcessingEngine::ParseRequest(const char* url)
	{
		GPropertySet props;
		props.Parse(url,"&");

		WebRequest* pWebRequest = NULL;

		const char* request = props.GetValue("request");
		if(request==NULL)
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			return NULL;
		}

		if(g_stricmp(request, "GetCapabilities")==0)
		{
			//CapabilitiesRequest* pRequest = new CapabilitiesRequest();
			//if(!pRequest->Create(props))
			//{
			//	GLogger* pLogger = augeGetLoggerInstance();
			//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			//	pRequest->Release();
			//	pRequest = NULL;
			//}
			//pWebRequest = pRequest;
		}

		return pWebRequest;
	}

	WebRequest*	GeoProcessingEngine::ParseRequest(rude::CGI& cgi)
	{
		const char* request = cgi["request"];
		if(request==NULL)
		{ 
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			return NULL;
		}
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		return handler->ParseRequest(cgi);
	}

	WebRequest*	GeoProcessingEngine::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	/*WebRequest*	GeoProcessingEngine::ParseRequest(rude::CGI& cgi)
	{
		WebRequest* pWebRequest = NULL;

		const char* request = cgi["request"];
		if(request==NULL)
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			return NULL;
		}


		if(g_stricmp(request, "GetCapabilities")==0)
		{ 
			CapabilitiesRequest* pRequest = new CapabilitiesRequest();
			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
		else if(g_stricmp(request, "CreateService")==0)
		{
			CreateServiceRequest* pRequest = new CreateServiceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		else if(g_stricmp(request, "RemoveService")==0)
		{
			RemoveServiceRequest* pRequest = new RemoveServiceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
		else if(g_stricmp(request, "GetService")==0)
		{
			GetServiceRequest* pRequest = new GetServiceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		else if(g_stricmp(request, "RegisterDataSource")==0)
		{
			RegisterDataSourceRequest* pRequest = new RegisterDataSourceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		return pWebRequest;
	}*/

	WebRequest*	GeoProcessingEngine::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		XElement	*pxRoot = pxDoc->GetRootNode();
		const char* request = pxRoot->GetName();

		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return NULL;
		}
		return handler->ParseRequest(pxDoc, mapName);
	}

	WebResponse* GeoProcessingEngine::Execute(WebRequest* pWebRequest, User* pUser)
	{
		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
		}
				
		return handler->Execute(pWebRequest, pUser);
	}

	/*WebResponse* GeoProcessingEngine::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse		*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		if(!Support(request))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "WMS doesn't Request [%s].", request);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		if(g_stricmp(request, "GetCapabilities")==0)
		{
			CapabilitiesRequest* pCapaRequest = static_cast<CapabilitiesRequest*>(pWebRequest);
			pWebResponse = GetCapabilities(pCapaRequest);
		}
		else if(g_stricmp(request, "CreateService")==0)
		{
			CreateServiceRequest* pRequest = static_cast<CreateServiceRequest*>(pWebRequest);
			pWebResponse = CreateService(pRequest);
		}
		else if(g_stricmp(request, "RemoveService")==0)
		{
			RemoveServiceRequest* pRequest = static_cast<RemoveServiceRequest*>(pWebRequest);
			pWebResponse = RemoveService(pRequest);
		}
		else if(g_stricmp(request, "GetService")==0)
		{
			GetServiceRequest* pRequest = static_cast<GetServiceRequest*>(pWebRequest);
			pWebResponse = GetService(pRequest);
		}
		else if(g_stricmp(request, "RegisterDataSource")==0)
		{
			RegisterDataSourceRequest* pRequest = static_cast<RegisterDataSourceRequest*>(pWebRequest);
			pWebResponse = RegisterDataSource(pRequest);
		}

		return pWebResponse;
	}*/

	WebResponse* GeoProcessingEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
		}

		return handler->Execute(pWebRequest, pWebContext, pUser);
	}

	//WebResponse* GeoProcessingEngine::GetCapabilities(CapabilitiesRequest* pRequest)
	//{	
	//	return new CapabilitiesResponse(pRequest);
	//}

	//WebResponse* GeoProcessingEngine::CreateService(CreateServiceRequest* pRequest)
	//{
	//	const char* name = NULL;
	//	ServiceManager* pServiceManager = NULL;
	//	Service* pService = NULL;

	//	name = pRequest->GetName();
	//	pServiceManager = augeGetServiceManagerInstance();
	//	pService = pServiceManager->GetService(name);
	//	if(pService!=NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s already exists.", name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	RESULTCODE rc = pServiceManager->Register(name);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
	//	pSusResponse->SetRequest("CreateService");
	//	return pSusResponse;
	//}

	//WebResponse* GeoProcessingEngine::RemoveService(RemoveServiceRequest* pRequest)
	//{
	//	const char* name = NULL;
	//	ServiceManager* pServiceManager = NULL;
	//	Service* pService = NULL;

	//	name = pRequest->GetName();
	//	pServiceManager = augeGetServiceManagerInstance();
	//	pService = pServiceManager->GetService(name);
	//	if(pService==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s does not exists.", name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	RESULTCODE rc = pServiceManager->Unregister(name);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
	//	pSusResponse->SetRequest("RemoveService");
	//	return pSusResponse;
	//}

	//WebResponse* GeoProcessingEngine::GetService(GetServiceRequest* pRequest)
	//{
	//	return new GetServiceResponse(pRequest);
	//}

	//WebResponse* GeoProcessingEngine::RegisterHandler(RegisterHandlerRequest* pRequest)
	//{
	//	const char* service_name = NULL;
	//	const char* handler = NULL;
	//	WebResponse* pResponse = NULL;

	//	service_name = pRequest->GetName();
	//	handler = pRequest->GetHandler();

	//	Service* pService = NULL;
	//	ServiceManager* pServiceManager = NULL;
	//	pServiceManager = augeGetServiceManagerInstance();
	//	pService = pServiceManager->GetService(service_name);
	//	if(pService!=NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s already exists.", service_name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}
	//	
	//	return pResponse;
	//}
	//
	//WebResponse* GeoProcessingEngine::RegisterDataSource(RegisterDataSourceRequest* pRequest)
	//{
	//	const char* name = NULL;
	//	const char* engine = NULL;
	//	const char* uri = NULL;
	//	WebResponse* pWebResponse = NULL;

	//	name = pRequest->GetName();
	//	engine = pRequest->GetDataEngine();
	//	uri = pRequest->GetURI();

	//	ConnectionManager*	pConnManager = augeGetConnectionManagerInstance();
	//	RESULTCODE rc = pConnManager->Register(name, engine, uri);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
	//	pSusResponse->SetRequest(pRequest->GetRequest());
	//	return pSusResponse;
	//}
}