#include "WTileEngine.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"

#include "WTileRequest.h"
#include "WmtsCapabilitiesHandler.h"
#include "GetTileHandler.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WTileEngine g_webTileEngine;
		return &g_webTileEngine;
	}

	WTileEngine::WTileEngine()
	{
		m_handler = NULL;
		WmtsCapabilitiesHandler* hander = new WmtsCapabilitiesHandler();
		hander->SetEngine(this);
		m_handlers.push_back(hander);
		m_handlers.push_back(new GetTileHandler());
		
	}

	WTileEngine::~WTileEngine()
	{
		CleanupHandlers();
	}

	void WTileEngine::CleanupHandlers()
	{
		WebHandler* pHandler = NULL;
		std::vector<WebHandler*>::iterator iter;
		for(iter=m_handlers.begin(); iter!=m_handlers.end(); iter++)
		{
			pHandler = *iter;
			pHandler->Release();
		}
		m_handlers.clear();
	}

	WebHandler*	WTileEngine::GetHandler(const char* name)
	{
		std::vector<WebHandler*>::iterator iter;
		for(iter=m_handlers.begin(); iter!=m_handlers.end(); iter++)
		{
			if(g_stricmp((*iter)->GetName(), name)==0)
			{
				return *iter;
			}
		}
		return NULL;
	}

	const char*	WTileEngine::GetID()
	{
		return "WTileEngine";
	}

	const char*	WTileEngine::GetType()
	{
		return "wmts";
	}

	const char*	WTileEngine::GetDescription()
	{
		return "OGC WMTS";
	}

	void* WTileEngine::GetHandler()
	{
		return m_handler;
		}

	void WTileEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WTileEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WTileEngine::ParseRequest(const char* url)
	{
		//GPropertySet props;
		//props.Parse(url,"&");

		//const char* request = props.GetValue("request");
		//if(request==NULL)
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	return NULL;
		//}

		//WebRequest* pWebRequest = NULL;
		//if(g_stricmp(request, "GetCapabilities")==0)
		//{
		//	GetCapabilitiesRequest* pRequest = new GetCapabilitiesRequest();
		//	if(!pRequest->Create(props))
		//	{
		//		GLogger* pLogger = augeGetLoggerInstance();
		//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//		pRequest->Release();
		//		pRequest = NULL;
		//	}
		//	pWebRequest = pRequest;
		//}
		//else if(g_stricmp(request, "GetMap")==0)
		//{
		//	GetMapRequest* pRequest = new GetMapRequest();

		//	if(!pRequest->Create(props))
		//	{
		//		GLogger* pLogger = augeGetLoggerInstance();
		//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//		pRequest->Release();
		//		pRequest = NULL;
		//	}
		//	pWebRequest = pRequest;

		//}
		//else if(g_stricmp(request, "GetFeatureInfo")==0)
		//{

		//}
		//return pWebRequest;
		
		return NULL;
	}

	WebRequest* WTileEngine::ParseRequest(rude::CGI& cgi)
	{
		const char* request = cgi["request"];
		WebHandler* handler = NULL;
		if((request==NULL)||(strlen(request)==0))
		{
			handler = GetHandler("GetCapabilities");
		}
		else
		{
			handler = GetHandler(request);			
		}
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

	WebRequest* WTileEngine::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		WebRequest* pWebRequest = ParseRequest(cgi);
		if(pWebRequest!=NULL)
		{
			WTileRequest* pWRequest = static_cast<WTileRequest*>(pWebRequest);
			pWRequest->SetMapName(mapName);
		}
		return pWebRequest;
	}

	WebRequest*	WTileEngine::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		GError* pError = augeGetErrorInstance();
		pError->SetError("WMS do not support xml request");
		return NULL;
	}

	WebResponse* WTileEngine::Execute(WebRequest* pWebRequest)
	{
		WebResponse	*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;

		}
		return handler->Execute(pWebRequest);
	}

	WebResponse* WTileEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}
		return handler->Execute(pWebRequest, pWebContext);
	}

	//WebResponse* WTileEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	WebResponse	*pWebResponse = NULL;
	//	WTileRequest *pWTileRequest = static_cast<WTileRequest*>(pWebRequest);

	//	const char* request = pWTileRequest->GetRequest();
	//	WebHandler* handler = GetHandler(request);
	//	if(handler == NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Request %s is not supported", request);
	//	}

	//	if(pMap==NULL)
	//	{
	//		const char* mapName = pWTileRequest->GetMapName();
	//		if(mapName!=NULL)
	//		{
	//			CartoManager* pCartoManager = augeGetCartoManagerInstance();
	//			pMap = pCartoManager->LoadMap(mapName);
	//		}
	//	}
	//	else
	//	{
	//		pMap->AddRef();
	//	}

	//	if(pMap==NULL)
	//	{
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "Empty Service");
	//		pLogger->Error(msg, __FILE__, __LINE__);

	//		WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
	//		pExpResopnse->SetMessage(msg);
	//		return pExpResopnse;
	//	}
	//	
	//	pWebResponse = handler->Execute(pWebRequest, pWebContext, pMap);
	//	pMap->Release();

	//	return pWebResponse;
	//}
	
	//WebResponse* WebMapEngine::GetCapabilities(GetCapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	const char* cache_path = pWebContext->GetCacheProtocolPath();

	//	char capa_path[AUGE_PATH_MAX];
	//	auge_make_path(capa_path, NULL, cache_path, "wms_capabilities_1_3_0","xml");

	//	//if(access(capa_path,4))
	//	{
	//		WriteCapabilities(pRequest->GetVersion(), pWebContext, pMap);
	//	}

	//	GetCapabilitiesResponse* pResponse = new GetCapabilitiesResponse(pRequest);		
	//	pResponse->SetPath(capa_path);
	//	return pResponse;
	//}

	//WebResponse* WebMapEngine::GetMap(GetMapRequest* pRequest, WebContext* pWebContext, Map* pMap)
	//{
	//	g_uint width  = pRequest->GetWidth();
	//	g_uint height = pRequest->GetHeight();

	//	Canvas* pCanvas = NULL;
	//	CartoFactory* pCartoFactory = augeGetCartoFactoryInstance();

	//	pCanvas = pCartoFactory->CreateCanvas2D(width, height);

	//	GColor& bgColor = pRequest->GetBgColor(); 
	//	//GColor bgColor(255,0,0,255);
	//	char temp[AUGE_NAME_MAX];
	//	g_sprintf(temp, "[Alpha]:%d", bgColor.GetAlpha());
	//	GLogger* pLogger = augeGetLoggerInstance();
	//	pLogger->Info(temp, __FILE__, __LINE__);
	//	pCanvas->DrawBackground(bgColor);

	//	GEnvelope& extent = pRequest->GetExtent();
	//	pCanvas->SetViewer(extent);

	//	const char* lname = NULL;
	//	const char* sname = NULL;

	//	Layer* pLayer = NULL;
	//	Style* pStyle = NULL;
	//	CartoManager* pCartoManager = augeGetCartoManagerInstance();

	//	g_uint lc = pRequest->GetLayerCount();
	//	for(g_int i=lc-1; i>=0; i--)
	//	{
	//		lname = pRequest->GetLayer(i);
	//		sname = pRequest->GetStyle(i);

	//		pLayer = pMap->GetLayer(lname);
	//		if(pLayer!=NULL)
	//		{
	//			if(strlen(sname)>0)
	//			{	// User defined Style
	//				pStyle = pCartoManager->GetStyle(sname);
	//				if(pStyle!=NULL)
	//				{
	//					pCanvas->DrawLayer(pLayer, pStyle);
	//				}
	//				else
	//				{
	//					char msg[AUGE_MSG_MAX];
	//					g_sprintf(msg, "Style [%s] Not Defined", sname);
	//					GLogger* pLogger = augeGetLoggerInstance();
	//					pLogger->Info(msg, __FILE__, __LINE__);
	//				}
	//			}
	//			else
	//			{	// Default Style
	//				pCanvas->DrawLayer(pLayer);
	//			}
	//		}
	//	}

	//	char img_sfix[AUGE_EXT_MAX] = {0};
	//	char img_name[AUGE_NAME_MAX] = {0};
	//	char img_path[AUGE_PATH_MAX] = {0};
	//	auge_get_image_suffix(pRequest->GetMimeType(), img_sfix, AUGE_EXT_MAX);
	//	auge_generate_uuid(img_name, AUGE_NAME_MAX);
	//	const char* cache_path = pWebContext->GetCacheMapPath();
	//	auge_make_path(img_path, NULL, cache_path, img_name, img_sfix);
	//	pCanvas->Save(img_path);
	//	pCanvas->Release();
	//	GetMapResponse* pMapResponse = new GetMapResponse(pRequest);
	//	pMapResponse->SetPath(img_path);
	//	return pMapResponse;
	//}

	//bool  WebMapEngine::WriteCapabilities(const char* version, WebContext* pWebContext, Map* pMap)
	//{
	//	if(strcmp(version, "1.1.0")==0)
	//	{
	//		return false;
	//	}
	//	else if(strcmp(version, "1.3.0")==0)
	//	{
	//		return WriteCapabilities_1_3_0(pWebContext, pMap);
	//	}
	//	return false;
	//}

	//bool WebMapEngine::WriteCapabilities_1_3_0(WebContext* pWebContext, Map* pMap)
	//{
	//	const char* cache_path = pWebContext->GetCacheProtocolPath();

	//	char str[AUGE_MSG_MAX];
	//	char capa_path[AUGE_PATH_MAX];
	//	auge_make_path(capa_path, NULL, cache_path, "wms_capabilities_1_3_0","xml");

	//	XElement  *pxNode = NULL;
	//	XElement  *pxRoot = NULL;
	//	XDocument *pxDoc = new XDocument();
	//	pxRoot = pxDoc->CreateRootNode("WMS_Capabilities", NULL, NULL);
	//	pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
	//	pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
	//	pxRoot->SetAttribute("version", "1.3.0", NULL);

	//	// WMS_Capabilities-->Service
	//	XElement *pxService = pxRoot->AddChild("Service", NULL);
	//	pxNode = pxNode = pxService->AddChild("Name", NULL);
	//	pxNode->SetChildText("WMS");
	//	pxNode = pxNode = pxService->AddChild("Title", NULL);
	//	pxNode->SetChildText("Auge Web Map Service");
	//	pxNode = pxNode = pxService->AddChild("Abstract", NULL);
	//	// WMS_Capabilities-->Service-->KeywordList
	//	XElement *pxKeywords = pxService->AddChild("KeywordList", NULL);
	//	pxNode = pxNode = pxKeywords->AddChild("Keyword", NULL);
	//	pxNode->SetChildText("WMS");
	//	pxNode = pxNode = pxKeywords->AddChild("Keyword", NULL);
	//	pxNode->SetChildText("AugeGIS");
	//	// WMS_Capabilities-->Service-->OnlineResource
	//	pxNode = pxNode = pxService->AddChild("OnlineResource", NULL);
	//	// WMS_Capabilities-->Service-->Fees
	//	XElement *pxFees = pxService->AddChild("Fees", NULL);
	//	pxFees->SetChildText("NONE");
	//	// WMS_Capabilities-->Service-->AccessConstraints
	//	XElement *pxConstraints = pxService->AddChild("AccessConstraints", NULL);
	//	pxConstraints->SetChildText("NONE");

	//	// WMS_Capabilities-->Capability
	//	XElement* pxCapability = pxRoot->AddChild("Capability", NULL);

	//	// WMS_Capabilities-->Capability-->Request
	//	XElement* pxRequest = pxCapability->AddChild("Request", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities
	//	XElement* pxCapabilities = pxRequest->AddChild("GetCapabilities", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format
	//	XElement* pxFormat = pxCapabilities->AddChild("Format", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType
	//	XElement* pxDCPType = pxCapabilities->AddChild("DCPType", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP
	//	XElement* pxHttp = pxDCPType->AddChild("HTTP", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get
	//	XElement* pxGet = pxHttp->AddChild("Get", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get-->OnlineResource
	//	XElement* pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
	//	pxHttp = pxDCPType->AddChild("HTTP", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post
	//	XElement* pxPost = pxHttp->AddChild("Post", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post-->OnlineResource
	//	pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

	//	// WMS_Capabilities-->Capability-->Request-->GetMap
	//	XElement* pxGeMap = pxRequest->AddChild("GeMap", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format
	//	pxFormat = pxGeMap->AddChild("Format", NULL);
	//	pxFormat->AddChildText("image/png");
	//	pxFormat = pxGeMap->AddChild("Format", NULL);
	//	pxFormat->AddChildText("image/jpg");
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format-->DCPType
	//	pxDCPType = pxGeMap->AddChild("DCPType", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format-->DCPType-->HTTP
	//	pxHttp = pxDCPType->AddChild("HTTP", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format-->DCPType-->HTTP-->Get
	//	pxGet = pxHttp->AddChild("Get", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format-->DCPType-->HTTP-->Get-->OnlineResource
	//	pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
	//	pxHttp = pxDCPType->AddChild("HTTP", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format-->DCPType-->HTTP-->Post
	//	pxPost = pxHttp->AddChild("Post", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetMap-->Format-->DCPType-->HTTP-->Post-->OnlineResource
	//	pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo
	//	XElement* pxGetFeatureInfo = pxRequest->AddChild("GetFeatureInfo", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format
	//	pxFormat = pxGetFeatureInfo->AddChild("Format", NULL);
	//	pxFormat->AddChildText("text/plain");
	//	pxFormat = pxGetFeatureInfo->AddChild("Format", NULL);
	//	pxFormat->AddChildText("text.xml");
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format-->DCPType
	//	pxDCPType = pxGetFeatureInfo->AddChild("DCPType", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format-->DCPType-->HTTP
	//	pxHttp = pxDCPType->AddChild("HTTP", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format-->DCPType-->HTTP-->Get
	//	pxGet = pxHttp->AddChild("Get", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format-->DCPType-->HTTP-->Get-->OnlineResource
	//	pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
	//	//pxHttp = pxDCPType->AddChild("HTTP", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format-->DCPType-->HTTP-->Post
	//	//pxPost = pxHttp->AddChild("Post", NULL);
	//	// WMS_Capabilities-->Capability-->Request-->GetFeatureInfo-->Format-->DCPType-->HTTP-->Post-->OnlineResource
	//	//pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

	//	// WMS_Capabilities-->Capability-->Exception
	//	XElement* pxException = pxCapability->AddChild("Exception", NULL);
	//	// WMS_Capabilities-->Capability-->Exception-->Format
	//	pxFormat = pxException->AddChild("Exception", NULL);
	//	pxFormat->AddChildText("text/xml");

	//	// WMS_Capabilities-->Capability-->Layer
	//	XElement* pxLayer = pxCapability->AddChild("Layer", NULL);
	//	pxNode = pxLayer->AddChild("Title", NULL);
	//	pxNode->AddChildText("Auge GIS Map Service");
	//	pxNode = pxLayer->AddChild("Abstract", NULL);
	//	pxNode->AddChildText("Auge GIS Map Service");
	//	// WMS_Capabilities-->Capability-->Layer-->CRS
	//	pxNode = pxLayer->AddChild("CRS", NULL);
	//	pxNode->AddChildText("EPSG:4326");
	//	// WMS_Capabilities-->Capability-->Layer-->EX_GeographicBoundingBox
	//	GEnvelope extent = pMap->GetExtent();
	//	if(!extent.IsValid())
	//	{
	//		extent.Set(-180.f,-90.0f,180.0f,90.0f);
	//	}
	//	XElement* pxGeoBounding = pxLayer->AddChild("EX_GeographicBoundingBox", NULL);
	//	g_sprintf(str, "%.6f", extent.m_xmin);
	//	pxNode = pxGeoBounding->AddChild("westBoundLongitude");
	//	pxNode->SetChildText(str);
	//	g_sprintf(str, "%.6f", extent.m_xmax);
	//	pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
	//	pxNode->SetChildText(str);
	//	g_sprintf(str, "%.6f", extent.m_ymin);
	//	pxNode = pxGeoBounding->AddChild("southBoundLatitude");
	//	pxNode->SetChildText(str);
	//	g_sprintf(str, "%.6f", extent.m_ymax);
	//	pxNode = pxGeoBounding->AddChild("northBoundLatitude");
	//	pxNode->SetChildText(str);
	//	// WMS_Capabilities-->Capability-->Layer-->BoundingBox
	//	XElement* pxBounding = pxLayer->AddChild("BoundingBox", NULL);
	//	g_sprintf(str, "CRS:%d", pMap->GetSRID());
	//	pxBounding->SetAttribute("CRS", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_xmin);
	//	pxBounding->SetAttribute("minx", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_xmax);
	//	pxBounding->SetAttribute("maxx", str,NULL);
	//	g_sprintf(str, "%.6f", extent.m_ymin);
	//	pxBounding->SetAttribute("miny", str,NULL);		
	//	g_sprintf(str, "%.6f", extent.m_ymax);
	//	pxBounding->SetAttribute("maxy", str,NULL);

	//	// WMS_Capabilities-->Capability-->Layer-->Layer
	//	Layer* pLayer = NULL;
	//	g_uint lc = pMap->GetLayerCount();
	//	for(g_uint i=0; i<lc; i++)
	//	{
	//		pLayer = pMap->GetLayer(i);
	//		if(pLayer!=NULL)
	//		{
	//			const char* lname = pLayer->GetName();
	//			XElement* pxLayer_2 = pxLayer->AddChild("Layer", NULL);
	//			pxLayer_2->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
	//			pxNode = pxLayer_2->AddChild("Name",NULL);
	//			pxNode->SetChildText(lname);
	//			pxNode = pxLayer_2->AddChild("Title",NULL);
	//			pxNode->SetChildText(lname);
	//			pxNode = pxLayer_2->AddChild("Abstract",NULL);
	//			g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
	//			pxNode = pxLayer_2->AddChild("CRS",NULL);

	//			extent = pLayer->GetExtent();
	//			if(!extent.IsValid())
	//			{
	//				extent.Set(-180.f,-90.0f,180.0f,90.0f);
	//			}
	//			pxGeoBounding = pxLayer_2->AddChild("EX_GeographicBoundingBox", NULL);
	//			g_sprintf(str, "%.6f", extent.m_xmin);
	//			pxNode = pxGeoBounding->AddChild("westBoundLongitude");
	//			pxNode->SetChildText(str);
	//			g_sprintf(str, "%.6f", extent.m_xmax);
	//			pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
	//			pxNode->SetChildText(str);
	//			g_sprintf(str, "%.6f", extent.m_ymin);
	//			pxNode = pxGeoBounding->AddChild("southBoundLatitude");
	//			pxNode->SetChildText(str);
	//			g_sprintf(str, "%.6f", extent.m_ymax);
	//			pxNode = pxGeoBounding->AddChild("northBoundLatitude");
	//			pxNode->SetChildText(str);
	//			// WMS_Capabilities-->Capability-->Layer-->Layer
	//			pxBounding = pxLayer->AddChild("BoundingBox", NULL);
	//			g_sprintf(str, "CRS:%d", pMap->GetSRID());
	//			pxBounding->SetAttribute("CRS", str,NULL);
	//			g_sprintf(str, "%.6f", extent.m_xmin);
	//			pxBounding->SetAttribute("minx", str,NULL);
	//			g_sprintf(str, "%.6f", extent.m_xmax);
	//			pxBounding->SetAttribute("maxx", str,NULL);
	//			g_sprintf(str, "%.6f", extent.m_ymin);
	//			pxBounding->SetAttribute("miny", str,NULL);		
	//			g_sprintf(str, "%.6f", extent.m_ymax);
	//			pxBounding->SetAttribute("maxy", str,NULL);
	//			// WMS_Capabilities-->Capability-->Layer-->Layer-->Style
	//			Style* pStyle = pLayer->GetStyle();
	//			XElement* pxStyle = pxLayer_2->AddChild("Style", NULL);
	//			if(pStyle!=NULL)
	//			{
	//				pxNode = pxStyle->AddChild("Name",NULL);
	//				pxNode = pxStyle->AddChild("Title",NULL);
	//				pxNode = pxStyle->AddChild("Abstract",NULL);
	//				XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
	//				pxLegendURL->SetAttribute("width","20", NULL);
	//				pxLegendURL->SetAttribute("height","20", NULL);
	//				pxNode = pxLegendURL->AddChild("Format",NULL);
	//				pxNode->SetChildText("image/png");
	//				pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
	//			}
	//			else
	//			{
	//				pxNode = pxStyle->AddChild("Name",NULL);
	//				pxNode->SetChildText(pStyle->GetName());
	//				pxNode = pxStyle->AddChild("Title",NULL);
	//				pxNode->SetChildText(pStyle->GetName());
	//				pxNode = pxStyle->AddChild("Abstract",NULL);
	//				XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
	//				pxLegendURL->SetAttribute("width","20", NULL);
	//				pxLegendURL->SetAttribute("height","20", NULL);
	//				pxNode = pxLegendURL->AddChild("Format",NULL);
	//				pxNode->SetChildText("image/png");
	//				pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
	//			}

	//		}
	//	}


	//	pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
	//	pxDoc->Release();

	//	return true;
	//}
}