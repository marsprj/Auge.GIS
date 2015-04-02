#include "CapabilitiesHandler.h"
#include "CapabilitiesRequest.h"
#include "CapabilitiesResponse.h"
#include "WMapEngine.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"

namespace auge
{
	CapabilitiesHandler::CapabilitiesHandler()
	{

	}

	CapabilitiesHandler::~CapabilitiesHandler()
	{

	}

	const char*	CapabilitiesHandler::GetName()
	{
		return "GetCapabilities";
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		CapabilitiesRequest* pRequest = new CapabilitiesRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->SetHost(getenv("HTTP_HOST"));
		pRequest->SetRequestMethod(getenv("REQUEST_METHOD"));
		return pRequest;
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		CapabilitiesRequest* pRequest = new CapabilitiesRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest)
	{
		WebResponse* pWebResponse = NULL;
		CapabilitiesRequest* pRequest = static_cast<CapabilitiesRequest*>(pWebRequest);
		
		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{

		}
		else if(strcmp(version,"1.3.0")==0)
		{

		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		WebResponse* pWebResponse = NULL;
		CapabilitiesRequest* pRequest = static_cast<CapabilitiesRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{
			pWebResponse = WriteCapabilities_1_0_0(pRequest, pWebContext, pMap);
		}
		else if(strcmp(version,"1.3.0")==0)
		{
			pWebResponse = WriteCapabilities_1_3_0(pRequest, pWebContext, pMap);
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WMS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}
	
	CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap) 
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath(); 
		
		char wms_xlink[AUGE_MSG_MAX];
		g_sprintf(wms_xlink, "http://%s/%s/%s/%s/ims?service=wms",	pRequest->GetHost(),
																	AUGE_VIRTUAL_NAME,
																	pRequest->GetUser(),
																	pRequest->GetServiceName()); 

		char str[AUGE_MSG_MAX];
		char capa_path[AUGE_PATH_MAX];
		auge_make_path(capa_path, NULL, cache_path, "wms_capabilities_1_0_0","xml");

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("WMS_Capabilities", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", "1.0.0", NULL);

		// WMS_Capabilities-->Service
		XElement *pxService = pxRoot->AddChild("Service", NULL);
		pxNode = pxNode = pxService->AddChild("Name", NULL);
		pxNode->SetChildText("WMS");
		pxNode = pxNode = pxService->AddChild("Title", NULL);
		pxNode->SetChildText("Auge Web Map Service");
		pxNode = pxNode = pxService->AddChild("Abstract", NULL);
		// WMS_Capabilities-->Service-->KeywordList
		XElement *pxKeywords = pxService->AddChild("KeywordList", NULL);
		pxNode = pxNode = pxKeywords->AddChild("Keyword", NULL);
		pxNode->SetChildText("WMS");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", NULL);
		pxNode->SetChildText("AugeGIS");
		// WMS_Capabilities-->Service-->OnlineResource
		pxNode = pxNode = pxService->AddChild("OnlineResource", NULL);
		// WMS_Capabilities-->Service-->Fees
		XElement *pxFees = pxService->AddChild("Fees", NULL);
		pxFees->SetChildText("NONE");
		// WMS_Capabilities-->Service-->AccessConstraints
		XElement *pxConstraints = pxService->AddChild("AccessConstraints", NULL);
		pxConstraints->SetChildText("NONE");

		// WMS_Capabilities-->Capability
		XElement* pxCapability = pxRoot->AddChild("Capability", NULL);
		
		// Service Handlers
		WMapEngine* pMapEngine = (WMapEngine*)augeGetWebEngineInstance();
		std::vector<WebHandler*>& handlers = pMapEngine->m_handlers;
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			WebHandler* handler = *iter;

			XElement* pxRequest = pxCapability->AddChild("Request", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities
			XElement* pxCapabilities = pxRequest->AddChild(handler->GetName(), NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format
			XElement* pxFormat = pxCapabilities->AddChild("Format", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType
			XElement* pxDCPType = pxCapabilities->AddChild("DCPType", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP
			XElement* pxHttp = pxDCPType->AddChild("HTTP", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get
			XElement* pxGet = pxHttp->AddChild("Get", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get-->OnlineResource
			XElement* pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
			pxOnlineResource->SetAttribute("xlink:href", wms_xlink, NULL);
			pxHttp = pxDCPType->AddChild("HTTP", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post
			XElement* pxPost = pxHttp->AddChild("Post", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post-->OnlineResource
			pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);
			pxOnlineResource->SetAttribute("xlink:href", wms_xlink, NULL);
		}

		// WMS_Capabilities-->Capability-->Exception
		XElement* pxException = pxCapability->AddChild("Exception", NULL);
		// WMS_Capabilities-->Capability-->Exception-->Format
		XElement* pxFormat = pxException->AddChild("Exception", NULL);
		pxFormat->AddChildText("text/xml");

		// WMS_Capabilities-->Capability-->Layer
		XElement* pxLayer = pxCapability->AddChild("Layer", NULL);
		pxNode = pxLayer->AddChild("Title", NULL);
		pxNode->AddChildText("Auge GIS Map Service");
		pxNode = pxLayer->AddChild("Abstract", NULL);
		pxNode->AddChildText("Auge GIS Map Service");
		// WMS_Capabilities-->Capability-->Layer-->CRS
		pxNode = pxLayer->AddChild("CRS", NULL);
		pxNode->AddChildText("EPSG:4326");
		// WMS_Capabilities-->Capability-->Layer-->EX_GeographicBoundingBox
		GEnvelope extent = pMap->GetExtent();
		if(!extent.IsValid())
		{
			extent.Set(-180.f,-90.0f,180.0f,90.0f);
		}
		XElement* pxGeoBounding = pxLayer->AddChild("EX_GeographicBoundingBox", NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxNode = pxGeoBounding->AddChild("westBoundLongitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxNode = pxGeoBounding->AddChild("southBoundLatitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxNode = pxGeoBounding->AddChild("northBoundLatitude");
		pxNode->SetChildText(str);
		// WMS_Capabilities-->Capability-->Layer-->BoundingBox
		XElement* pxBounding = pxLayer->AddChild("BoundingBox", NULL);
		g_sprintf(str, "CRS:%d", pMap->GetSRID());
		pxBounding->SetAttribute("CRS", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxBounding->SetAttribute("minx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxBounding->SetAttribute("maxx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxBounding->SetAttribute("miny", str,NULL);		
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxBounding->SetAttribute("maxy", str,NULL);

		// WMS_Capabilities-->Capability-->Layer-->Layer
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				const char* lname = pLayer->GetName();
				XElement* pxLayer_2 = pxLayer->AddChild("Layer", NULL);
				pxLayer_2->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
				pxNode = pxLayer_2->AddChild("Name",NULL);
				pxNode->SetChildText(lname);
				pxNode = pxLayer_2->AddChild("Title",NULL);
				pxNode->SetChildText(lname);
				pxNode = pxLayer_2->AddChild("Abstract",NULL);
				g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
				pxNode = pxLayer_2->AddChild("CRS",NULL);

				extent = pLayer->GetExtent();
				if(!extent.IsValid())
				{
					extent.Set(-180.f,-90.0f,180.0f,90.0f);
				}
				pxGeoBounding = pxLayer_2->AddChild("EX_GeographicBoundingBox", NULL);
				g_sprintf(str, "%.6f", extent.m_xmin);
				pxNode = pxGeoBounding->AddChild("westBoundLongitude");
				pxNode->SetChildText(str);
				g_sprintf(str, "%.6f", extent.m_xmax);
				pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
				pxNode->SetChildText(str);
				g_sprintf(str, "%.6f", extent.m_ymin);
				pxNode = pxGeoBounding->AddChild("southBoundLatitude");
				pxNode->SetChildText(str);
				g_sprintf(str, "%.6f", extent.m_ymax);
				pxNode = pxGeoBounding->AddChild("northBoundLatitude");
				pxNode->SetChildText(str);
				// WMS_Capabilities-->Capability-->Layer-->Layer
				pxBounding = pxLayer->AddChild("BoundingBox", NULL);
				g_sprintf(str, "CRS:%d", pMap->GetSRID());
				pxBounding->SetAttribute("CRS", str,NULL);
				g_sprintf(str, "%.6f", extent.m_xmin);
				pxBounding->SetAttribute("minx", str,NULL);
				g_sprintf(str, "%.6f", extent.m_xmax);
				pxBounding->SetAttribute("maxx", str,NULL);
				g_sprintf(str, "%.6f", extent.m_ymin);
				pxBounding->SetAttribute("miny", str,NULL);		
				g_sprintf(str, "%.6f", extent.m_ymax);
				pxBounding->SetAttribute("maxy", str,NULL);
				// WMS_Capabilities-->Capability-->Layer-->Layer-->Style
				Style* pStyle = pLayer->GetStyle();
				XElement* pxStyle = pxLayer_2->AddChild("Style", NULL);
				if(pStyle!=NULL)
				{
					pxNode = pxStyle->AddChild("Name",NULL);
					pxNode->AddChildText(pStyle->GetName());
					pxNode = pxStyle->AddChild("Title",NULL);
					pxNode = pxStyle->AddChild("Abstract",NULL);
					XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
					pxLegendURL->SetAttribute("width","20", NULL);
					pxLegendURL->SetAttribute("height","20", NULL);
					pxNode = pxLegendURL->AddChild("Format",NULL);
					pxNode->SetChildText("image/png");
					pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
				}
				//else
				//{
				//	pxNode = pxStyle->AddChild("Name",NULL);
				//	pxNode->SetChildText(pStyle->GetName());
				//	pxNode = pxStyle->AddChild("Title",NULL);
				//	pxNode->SetChildText(pStyle->GetName());
				//	pxNode = pxStyle->AddChild("Abstract",NULL);
				//	XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
				//	pxLegendURL->SetAttribute("width","20", NULL);
				//	pxLegendURL->SetAttribute("height","20", NULL);
				//	pxNode = pxLegendURL->AddChild("Format",NULL);
				//	pxNode->SetChildText("image/png");
				//	pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
				//}
			}
		}

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		CapabilitiesResponse* pResponse = new CapabilitiesResponse(pRequest);
		pResponse->SetPath(capa_path);

		return pResponse;
	}

	CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_3_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap)
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath(); 

		char wms_xlink[AUGE_MSG_MAX];
		g_sprintf(wms_xlink, "http://%s/%s/%s/%s/ims?service=wms",	pRequest->GetHost(),
			AUGE_VIRTUAL_NAME,
			pRequest->GetUser(),
			pRequest->GetServiceName()); 

		char str[AUGE_MSG_MAX];
		char capa_path[AUGE_PATH_MAX];
		auge_make_path(capa_path, NULL, cache_path, "wms_capabilities_1_3_0","xml");

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("WMS_Capabilities", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", "1.0.0", NULL);

		// WMS_Capabilities-->Service
		XElement *pxService = pxRoot->AddChild("Service", NULL);
		pxNode = pxNode = pxService->AddChild("Name", NULL);
		pxNode->SetChildText("WMS");
		pxNode = pxNode = pxService->AddChild("Title", NULL);
		pxNode->SetChildText("Auge Web Map Service");
		pxNode = pxNode = pxService->AddChild("Abstract", NULL);
		// WMS_Capabilities-->Service-->KeywordList
		XElement *pxKeywords = pxService->AddChild("KeywordList", NULL);
		pxNode = pxNode = pxKeywords->AddChild("Keyword", NULL);
		pxNode->SetChildText("WMS");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", NULL);
		pxNode->SetChildText("AugeGIS");
		// WMS_Capabilities-->Service-->OnlineResource
		pxNode = pxNode = pxService->AddChild("OnlineResource", NULL);
		// WMS_Capabilities-->Service-->Fees
		XElement *pxFees = pxService->AddChild("Fees", NULL);
		pxFees->SetChildText("NONE");
		// WMS_Capabilities-->Service-->AccessConstraints
		XElement *pxConstraints = pxService->AddChild("AccessConstraints", NULL);
		pxConstraints->SetChildText("NONE");

		// WMS_Capabilities-->Capability
		XElement* pxCapability = pxRoot->AddChild("Capability", NULL);

		// Service Handlers
		WMapEngine* pMapEngine = (WMapEngine*)augeGetWebEngineInstance();
		std::vector<WebHandler*>& handlers = pMapEngine->m_handlers;
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			WebHandler* handler = *iter;

			XElement* pxRequest = pxCapability->AddChild("Request", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities
			XElement* pxCapabilities = pxRequest->AddChild(handler->GetName(), NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format
			XElement* pxFormat = pxCapabilities->AddChild("Format", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType
			XElement* pxDCPType = pxCapabilities->AddChild("DCPType", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP
			XElement* pxHttp = pxDCPType->AddChild("HTTP", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get
			XElement* pxGet = pxHttp->AddChild("Get", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get-->OnlineResource
			XElement* pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
			pxOnlineResource->SetAttribute("xlink:href", wms_xlink, NULL);
			pxHttp = pxDCPType->AddChild("HTTP", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post
			XElement* pxPost = pxHttp->AddChild("Post", NULL);
			// WMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post-->OnlineResource
			pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);
			pxOnlineResource->SetAttribute("xlink:href", wms_xlink, NULL);
		}

		// WMS_Capabilities-->Capability-->Exception
		XElement* pxException = pxCapability->AddChild("Exception", NULL);
		// WMS_Capabilities-->Capability-->Exception-->Format
		XElement* pxFormat = pxException->AddChild("Exception", NULL);
		pxFormat->AddChildText("text/xml");

		// WMS_Capabilities-->Capability-->Layer
		XElement* pxLayer = pxCapability->AddChild("Layer", NULL);
		pxNode = pxLayer->AddChild("Title", NULL);
		pxNode->AddChildText("Auge GIS Map Service");
		pxNode = pxLayer->AddChild("Abstract", NULL);
		pxNode->AddChildText("Auge GIS Map Service");
		// WMS_Capabilities-->Capability-->Layer-->CRS
		pxNode = pxLayer->AddChild("CRS", NULL);
		pxNode->AddChildText("EPSG:4326");
		// WMS_Capabilities-->Capability-->Layer-->EX_GeographicBoundingBox
		GEnvelope extent = pMap->GetExtent();
		if(!extent.IsValid())
		{
			extent.Set(-180.f,-90.0f,180.0f,90.0f);
		}
		XElement* pxGeoBounding = pxLayer->AddChild("EX_GeographicBoundingBox", NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxNode = pxGeoBounding->AddChild("westBoundLongitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxNode = pxGeoBounding->AddChild("southBoundLatitude");
		pxNode->SetChildText(str);
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxNode = pxGeoBounding->AddChild("northBoundLatitude");
		pxNode->SetChildText(str);
		// WMS_Capabilities-->Capability-->Layer-->BoundingBox
		XElement* pxBounding = pxLayer->AddChild("BoundingBox", NULL);
		g_sprintf(str, "CRS:%d", pMap->GetSRID());
		pxBounding->SetAttribute("CRS", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmin);
		pxBounding->SetAttribute("minx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_xmax);
		pxBounding->SetAttribute("maxx", str,NULL);
		g_sprintf(str, "%.6f", extent.m_ymin);
		pxBounding->SetAttribute("miny", str,NULL);		
		g_sprintf(str, "%.6f", extent.m_ymax);
		pxBounding->SetAttribute("maxy", str,NULL);

		// WMS_Capabilities-->Capability-->Layer-->Layer
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				const char* lname = pLayer->GetName();
				XElement* pxLayer_2 = pxLayer->AddChild("Layer", NULL);
				pxLayer_2->SetAttribute("queryable", pLayer->IsQueryable()?"1":"0", NULL);
				pxNode = pxLayer_2->AddChild("Name",NULL);
				pxNode->SetChildText(lname);
				pxNode = pxLayer_2->AddChild("Title",NULL);
				pxNode->SetChildText(lname);
				pxNode = pxLayer_2->AddChild("Abstract",NULL);
				g_sprintf(str, "EPSG:%d", pLayer->GetSRID());
				pxNode = pxLayer_2->AddChild("CRS",NULL);

				extent = pLayer->GetExtent();
				if(!extent.IsValid())
				{
					extent.Set(-180.f,-90.0f,180.0f,90.0f);
				}
				pxGeoBounding = pxLayer_2->AddChild("EX_GeographicBoundingBox", NULL);
				g_sprintf(str, "%.6f", extent.m_xmin);
				pxNode = pxGeoBounding->AddChild("westBoundLongitude");
				pxNode->SetChildText(str);
				g_sprintf(str, "%.6f", extent.m_xmax);
				pxNode = pxGeoBounding->AddChild("eastBoundLongitude");
				pxNode->SetChildText(str);
				g_sprintf(str, "%.6f", extent.m_ymin);
				pxNode = pxGeoBounding->AddChild("southBoundLatitude");
				pxNode->SetChildText(str);
				g_sprintf(str, "%.6f", extent.m_ymax);
				pxNode = pxGeoBounding->AddChild("northBoundLatitude");
				pxNode->SetChildText(str);
				// WMS_Capabilities-->Capability-->Layer-->Layer
				pxBounding = pxLayer->AddChild("BoundingBox", NULL);
				g_sprintf(str, "CRS:%d", pMap->GetSRID());
				pxBounding->SetAttribute("CRS", str,NULL);
				g_sprintf(str, "%.6f", extent.m_xmin);
				pxBounding->SetAttribute("minx", str,NULL);
				g_sprintf(str, "%.6f", extent.m_xmax);
				pxBounding->SetAttribute("maxx", str,NULL);
				g_sprintf(str, "%.6f", extent.m_ymin);
				pxBounding->SetAttribute("miny", str,NULL);		
				g_sprintf(str, "%.6f", extent.m_ymax);
				pxBounding->SetAttribute("maxy", str,NULL);
				// WMS_Capabilities-->Capability-->Layer-->Layer-->Style
				Style* pStyle = pLayer->GetStyle();
				XElement* pxStyle = pxLayer_2->AddChild("Style", NULL);
				if(pStyle!=NULL)
				{
					pxNode = pxStyle->AddChild("Name",NULL);
					pxNode = pxStyle->AddChild("Title",NULL);
					pxNode = pxStyle->AddChild("Abstract",NULL);
					XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
					pxLegendURL->SetAttribute("width","20", NULL);
					pxLegendURL->SetAttribute("height","20", NULL);
					pxNode = pxLegendURL->AddChild("Format",NULL);
					pxNode->SetChildText("image/png");
					pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
				}
				//else
				//{
				//	pxNode = pxStyle->AddChild("Name",NULL);
				//	pxNode->SetChildText(pStyle->GetName());
				//	pxNode = pxStyle->AddChild("Title",NULL);
				//	pxNode->SetChildText(pStyle->GetName());
				//	pxNode = pxStyle->AddChild("Abstract",NULL);
				//	XElement* pxLegendURL = pxStyle->AddChild("LegendURL",NULL);
				//	pxLegendURL->SetAttribute("width","20", NULL);
				//	pxLegendURL->SetAttribute("height","20", NULL);
				//	pxNode = pxLegendURL->AddChild("Format",NULL);
				//	pxNode->SetChildText("image/png");
				//	pxNode = pxLegendURL->AddChild("OnlineResource",NULL);
				//} 
			}
		}

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		CapabilitiesResponse* pResponse = new CapabilitiesResponse(pRequest);
		pResponse->SetPath(capa_path);

		return pResponse;
	}
}