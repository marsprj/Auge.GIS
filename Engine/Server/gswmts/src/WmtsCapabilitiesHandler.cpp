#include "WmtsCapabilitiesHandler.h"
#include "WmtsCapabilitiesRequest.h"
#include "WmtsCapabilitiesResponse.h"
#include "WTileEngine.h"
#include "AugeService.h" 
#include "AugeXML.h"
#include "AugeTile.h"
#include "AugeData.h"
#include <math.h>

namespace auge
{
	CapabilitiesHandler::CapabilitiesHandler()
	{
		m_pRequest = NULL;
	}

	CapabilitiesHandler::~CapabilitiesHandler()
	{

	}

	const char*	CapabilitiesHandler::GetName()
	{
		return "GetCapabilities";
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi)
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

	WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		WebRequest* pWebRequest = ParseRequest(cgi);
		if(pWebRequest!=NULL)
		{
			CapabilitiesRequest* pWRequest = static_cast<CapabilitiesRequest*>(pWebRequest);
			pWRequest->SetMapName(mapName);
		}
		return pWebRequest;
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
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

	WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		CapabilitiesRequest* pRequest = static_cast<CapabilitiesRequest*>(pWebRequest);
		m_pRequest = pRequest;

		const char* sourceName = pRequest->GetMapName();
		if(sourceName==NULL)
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse(); 
			g_sprintf(msg, "No DataSource is attached");
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		const char* version = pRequest->GetVersion();
		//if(strcmp(version,"1.0.0")==0)
		{
			pWebResponse = WriteCapabilities_1_0_0(pRequest, pWebContext, sourceName);
		}
		//else if(strcmp(version,"1.3.0")==0)
		//{
		//	pWebResponse = WriteCapabilities_1_3_0(pRequest, pWebContext, sourceName); 
		//}
		//else
		//{
		//	char msg[AUGE_MSG_MAX];
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	g_sprintf(msg, "WMS does not support %s",version);
		//	pExpResponse->SetMessage(msg);
		//	pWebResponse = pExpResponse;
		//}
		//pMap->Release();

		return pWebResponse;
	}
	
	CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, const char* sourceName) 
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath(); 

		char wmts_xlink[AUGE_MSG_MAX];
		g_sprintf(wmts_xlink, "http://%s/%s/%s/%s/mgr?",	pRequest->GetHost(),
																	AUGE_VIRTUAL_NAME,
																	pRequest->GetUser(),
																	sourceName); 

		char str[AUGE_MSG_MAX];
		char capa_path[AUGE_PATH_MAX];
		auge_make_path(capa_path, NULL, cache_path, "wmts_capabilities_1_0_0","xml");

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("Capabilities", NULL, NULL); 
		SetRooteNode_1_0_0(pxRoot, "1.0.0"); 

		// Capabilities-->ServiceIdentification
		AddServiceIdentificationNode_1_0_0(pxRoot);

		// Capabilities-->ServiceProviderNode
		//AddServiceProviderNode_1_0_0(pxRoot);

		// Capabilities-->OperationsMetadataNode
		AddOperationsMetadataNode_1_0_0(pxRoot,wmts_xlink);

		// Capabilities-->Contents
		XElement* pxContents = pxRoot->AddChild("Contents", NULL);

		// Capabilities-->Contents-->Layer
		AddTileLayersNode_1_0_0(pxContents, sourceName);
		//AddTileLayerNode_1_0_0(pxContents, sourceName);
		
		// Capabilities-->Contents-->TileMatrixSet
		AddTileMatrixSetNode_1_0_0(pxContents);


		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		CapabilitiesResponse* pResponse = new CapabilitiesResponse(pRequest);
		pResponse->SetPath(capa_path);
		 
		return pResponse;
	}

	CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_3_0(CapabilitiesRequest* pRequest, WebContext* pWebContext, const char* sourceName)
	{
		return WriteCapabilities_1_0_0(pRequest, pWebContext, sourceName);
	}

	void CapabilitiesHandler::SetRooteNode_1_0_0(XElement* pxRoot, const char* version)
	{
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wmts/1.0",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wmts/1.0","wmts");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows/1.1","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("schemaLocation","http://www.opengis.net/wmts/1.0 http://schemas.opengis.net/wmts/1.0/wmtsGetCapabilities_response.xsd","xsi");
		pxRoot->SetAttribute("version", version, NULL);
	}

	void CapabilitiesHandler::AddServiceIdentificationNode_1_0_0(XElement* pxParent)
	{	
		XElement *pxService = pxParent->AddChild("ServiceIdentification", "ows");
		//XElement *pxNode = pxNode = pxService->AddChild("Name", "ows");
		//pxNode->SetChildText("WMTS");
		XElement *pxNode = pxNode = pxService->AddChild("Title", "ows");
		pxNode->SetChildText("Auge Web Map Service");
		//pxNode = pxNode = pxService->AddChild("Abstract", "ows");
		//// WMTS_Capabilities-->ServiceIdentification-->KeywordList
		//XElement *pxKeywords = pxService->AddChild("KeywordList", "ows");
		//pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		//pxNode->SetChildText("WMTS");
		//pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows"); 
		//pxNode->SetChildText("AugeGIS");
		// WMTS_Capabilities-->ServiceIdentification-->ServiceType
		pxNode = pxService->AddChild("ServiceType", "ows");
		pxNode->SetChildText("WMTS");
		pxNode = pxService->AddChild("ServiceTypeVersion", "ows");
		pxNode->SetChildText("1.0.0");
		//// WMTS_Capabilities-->ServiceIdentification-->Fees
		//XElement *pxFees = pxService->AddChild("Fees", "ows");
		//pxFees->SetChildText("NONE");
		//// WMTS_Capabilities-->ServiceIdentification-->AccessConstraints
		//XElement *pxConstraints = pxService->AddChild("AccessConstraints", "ows");
		//pxConstraints->SetChildText("NONE");
	}

	void CapabilitiesHandler::AddServiceProviderNode_1_0_0(XElement* pxParent)
	{
		XElement* pxProvider = pxParent->AddChild("ServiceProvider", "ows");
		XElement* pxProviderName = pxProvider->AddChild("ProviderName","ows");
		pxProviderName->SetChildText("Auge GIS");
		XElement* pxServiceContact = pxProvider->AddChild("ServiceContact","ows");
		XElement* pxNode = pxServiceContact->AddChild("IndividualName","ows");
		//pxNode = pxServiceContact->AddChild("PositionName","ows");
		//XElement* pxContactInfo = pxServiceContact->AddChild("PositionName","ows");
		//XElement* pxPhone = pxContactInfo->AddChild("Phone","ows");
		//pxNode = pxPhone->AddChild("Voice","ows");
		//pxNode = pxPhone->AddChild("Facsimile","ows");
		//XElement* pxAddress = pxContactInfo->AddChild("Address","ows");
		//pxNode = pxAddress->AddChild("City","ows");
		//pxNode = pxAddress->AddChild("AdministrativeArea","ows");
		//pxNode = pxAddress->AddChild("Country","ows");
	}

	void CapabilitiesHandler::AddOperationsMetadataNode_1_0_0(XElement* pxParent, const char* wmts_xlink)
	{
		XElement* pxOperationsMetadata = pxParent->AddChild("OperationsMetadata", "ows");

		// Service Handlers
		WTileEngine* pFeatureEngine = (WTileEngine*)augeGetWebEngineInstance();
		std::vector<WebHandler*>& handlers = pFeatureEngine->m_handlers;
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			WebHandler* handler = *iter; 

			// Capabilities-->OperationsMetadata-->Operation (GetCapabilities)
			XElement* pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
			pxOperation->SetAttribute("name", handler->GetName(),NULL);
			// Capabilities-->OperationsMetadata-->Operation-->DCP
			XElement* pxDCP = pxOperation->AddChild("DCP", "ows");
			// Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
			XElement* pxHTTP = pxDCP->AddChild("HTTP", "ows");
			// Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
			XElement* pxNode = pxHTTP->AddChild("Get", "ows");
			pxNode->SetAttribute("href",wmts_xlink,"xlink");

			XElement* pxConstraint = pxNode->AddChild("Constraint","ows");
			pxConstraint->SetAttribute("name", "GetEncoding", NULL);
			XElement* pxAllowedValues = pxConstraint->AddChild("AllowedValues","ows");
			XElement* pxValue = pxAllowedValues->AddChild("Value","ows");
			pxValue->AddChildText("KVP");
			//pxNode = pxHTTP->AddChild("Post", "ows");
			//pxNode->SetAttribute("href",wmts_xlink,"xlink");
			// Capabilities-->OperationsMetadata-->Operation-->Parameter(AcceptVersion)
			//XElement* pxParameter = pxOperation->AddChild("Parameter", "ows");
			//pxNode = pxParameter->AddChild("Value", "ows");
			//pxNode->AddChildText("1.0.0");
			//pxNode = pxParameter->AddChild("Value", "ows");
			//pxNode->AddChildText("1.1.0"); 
			//pxParameter = pxOperation->AddChild("AcceptFormats", "ows");
			//pxNode = pxParameter->AddChild("Value", "ows");
			//pxNode->AddChildText("text/xml");
		}
	}

	void CapabilitiesHandler::AddTileLayersNode_1_0_0(XElement* pxParent, const char* sourceName)
	{	
		TileStore* pTileStore = NULL;
		TileWorkspace* pTileWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();

		pTileWorkspace = dynamic_cast<TileWorkspace*>(pConnectionManager->GetWorkspace(sourceName));
		if(pTileWorkspace==NULL)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_sprintf(msg, "Cannot connect datasource [%s].", sourceName);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);

			return;
		}
		
		EnumTileStore* pEnumStore = pTileWorkspace->GetTileStores();
		pEnumStore->Reset();
		while((pTileStore=pEnumStore->Next())!=NULL)
		{
			char str[AUGE_PATH_MAX];
			const char* name = pTileStore->GetName();

			XElement* pxLayer = pxParent->AddChild("Layer", NULL); 
			XElement* pxTitle = pxLayer->AddChild("Title","ows");
			pxTitle->AddChildText(name);
			XElement* pxIdentifier = pxLayer->AddChild("Identifier","ows");
			pxIdentifier->AddChildText(name);
			XElement* pxAbstract = pxLayer->AddChild("Abstract","ows");
			pxAbstract->AddChildText(name);

			GEnvelope& extent = pTileStore->GetExtent();
			XElement* pxWGS84BoundingBox = pxLayer->AddChild("WGS84BoundingBox", "ows");
			pxWGS84BoundingBox->SetAttribute("crs","urn:ogc:def:crs:OGC:2:84",NULL);
			g_snprintf(str, AUGE_PATH_MAX, "%f %f", extent.m_xmin, extent.m_ymin);
			XElement* pxLowerCorner = pxWGS84BoundingBox->AddChild("LowerCorner", "ows");
			pxLowerCorner->AddChildText(str);
			g_snprintf(str, AUGE_PATH_MAX, "%f %f", extent.m_xmax, extent.m_ymax);
			XElement* pxUpperCorner = pxWGS84BoundingBox->AddChild("UpperCorner", "ows");
			pxUpperCorner->AddChildText(str);

			XElement* pxStyle = pxLayer->AddChild("Style",NULL);
			pxStyle->SetAttribute("IsDefault", "true",NULL);
			pxTitle = pxStyle->AddChild("Title", "ows");
			pxTitle->SetChildText("Default Style");
			pxIdentifier = pxStyle->AddChild("Identifier", "ows");
			pxIdentifier->AddChildText("Default");

			XElement* pxFormat = pxLayer->AddChild("Format",NULL);
			pxFormat->AddChildText("image/png");

			XElement* pxTileMatrixSetLink = pxLayer->AddChild("TileMatrixSetLink",NULL); 
			XElement* pxTileMatrixSet = pxTileMatrixSetLink->AddChild("TileMatrixSet",NULL);
			//pxTileMatrixSet->AddChildText("GoogleCRS84Quad");
			pxTileMatrixSet->AddChildText(pTileStore->GetTileTypeAsString());

			XElement* pxResourceURL = pxLayer->AddChild("ResourceURL", NULL);
			pxResourceURL->SetAttribute("format", "image/png", NULL);
			pxResourceURL->SetAttribute("resourceType", "tile", NULL);

			char strs[AUGE_PATH_MAX];
			//g_snprintf(strs,AUGE_PATH_MAX,"http://localhost:8080/QuadServer/services/maps/wmts100/google/default/GOOGLE_TILE_STORE/{TileMatrix}/{TileRow}/{TileCol}.png");
			//g_snprintf(str, AUGE_PATH_MAX, )
			g_snprintf(strs,AUGE_PATH_MAX, "http://%s/%s/%s/%s/mgr/default/GOOGLE_TILE_STORE/{TileMatrix}/{TileRow}/{TileCol}.png",m_pRequest->GetHost(),
				AUGE_VIRTUAL_NAME,
				m_pRequest->GetUser(),
				sourceName); 
			pxResourceURL->SetAttribute("template", strs, NULL);
		}
		pEnumStore->Release();
	}

	void CapabilitiesHandler::AddTileLayerNode_1_0_0(XElement* pxParent, const char* sourceName)
	{	
		const char* name = "store1";
		char str[AUGE_PATH_MAX];
		TileStore* pTileStore = NULL;
		TileWorkspace* pTileWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();

		pTileWorkspace = dynamic_cast<TileWorkspace*>(pConnectionManager->GetWorkspace(sourceName));
		if(pTileWorkspace!=NULL)
		{
			pTileStore = pTileWorkspace->OpenTileStore("store1");

			XElement* pxLayer = pxParent->AddChild("Layer", NULL); 
			XElement* pxTitle = pxLayer->AddChild("Title","ows");
			pxTitle->AddChildText(name);
			XElement* pxIdentifier = pxLayer->AddChild("Identifier","ows");
			pxIdentifier->AddChildText(name);
			XElement* pxAbstract = pxLayer->AddChild("Abstract","ows");
			pxAbstract->AddChildText(name);

			GEnvelope& extent = pTileStore->GetExtent();
			XElement* pxWGS84BoundingBox = pxLayer->AddChild("WGS84BoundingBox", "ows");
			pxWGS84BoundingBox->SetAttribute("crs","urn:ogc:def:crs:OGC:2:84",NULL);
			g_snprintf(str, AUGE_PATH_MAX, "%f %f", extent.m_xmin, extent.m_ymin);
			XElement* pxLowerCorner = pxWGS84BoundingBox->AddChild("LowerCorner", "ows");
			pxLowerCorner->AddChildText(str);
			g_snprintf(str, AUGE_PATH_MAX, "%f %f", extent.m_xmax, extent.m_ymax);
			XElement* pxUpperCorner = pxWGS84BoundingBox->AddChild("UpperCorner", "ows");
			pxUpperCorner->AddChildText(str);

			XElement* pxStyle = pxLayer->AddChild("Style",NULL);
			pxStyle->SetAttribute("IsDefault", "true",NULL);
			pxTitle = pxStyle->AddChild("Title", "ows");
			pxTitle->SetChildText("Default Style");
			pxIdentifier = pxStyle->AddChild("Identifier", "ows");
			pxIdentifier->AddChildText("Default");
			
			XElement* pxFormat = pxLayer->AddChild("Format",NULL);
			pxFormat->AddChildText("image/png");

			XElement* pxTileMatrixSetLink = pxLayer->AddChild("TileMatrixSetLink",NULL); 
			XElement* pxTileMatrixSet = pxTileMatrixSetLink->AddChild("TileMatrixSet",NULL);
			pxTileMatrixSet->AddChildText("GoogleCRS84Quad");

			XElement* pxResourceURL = pxLayer->AddChild("ResourceURL", NULL);
			pxResourceURL->SetAttribute("format", "image/png", NULL);
			pxResourceURL->SetAttribute("resourceType", "tile", NULL);

			char strs[AUGE_PATH_MAX];
			//g_snprintf(strs,AUGE_PATH_MAX,"http://localhost:8080/QuadServer/services/maps/wmts100/google/default/GOOGLE_TILE_STORE/{TileMatrix}/{TileRow}/{TileCol}.png");
			//g_snprintf(str, AUGE_PATH_MAX, )
			g_snprintf(strs,AUGE_PATH_MAX, "http://%s/%s/%s/%s/mgr/default/GOOGLE_TILE_STORE/{TileMatrix}/{TileRow}/{TileCol}.png",m_pRequest->GetHost(),
									AUGE_VIRTUAL_NAME,
									m_pRequest->GetUser(),
									sourceName); 
			pxResourceURL->SetAttribute("template", strs, NULL);

		}
	}

	void CapabilitiesHandler::AddTileMatrixSetNode_1_0_0(XElement* pxParent)
	{
		AddTileMatrixSet_GoogleCRS84Quad_1_0_0(pxParent);
		AddTileMatrixSet_PGIS_1_0_0(pxParent);
	}

	void CapabilitiesHandler::AddTileMatrixSet_GoogleCRS84Quad_1_0_0(XElement* pxParent)
	{
		const char* tile_width = "256";
		const char* tile_height= "256";
		const char* identifier = "GoogleCRS84Quad";
		const char* top_left_corner = "90.0 -180.0";
		char* denomiators[]={"5.590822640287178E8",
							"2.795411320143589E8",
							"1.397705660071794E8",
							"6.988528300358972E7",
							"3.494264150179486E7",
					 		"1.747132075089743E7",
							"8735660.375448715",
							"4367830.187724357",
							"2183915.093862179",
							"1091957.546931089",
							"545978.7734655447",
							"272989.3867327723",
							"136494.6933663862",
							"68247.34668319309",
							"34123.67334159654",
							"17061.83667079827",
							"8530.918335399136",
							"4265.459167699568", 
							"2132.729583849784"};
				
		char str[AUGE_NAME_MAX] = {0};
		
		/*
		<ows:Title>GoogleCRS84Quad</ows:Title>
		<ows:Abstract>GoogleMap切片标准</ows:Abstract>
		<ows:Identifier>GoogleCRS84Quad</ows:Identifier>
		<ows:BoundingBox crs="urn:ogc:def:crs:EPSG:6.18:3:3857">
		<ows:LowerCorner>-20037508 -20037508</ows:LowerCorner>
		<ows:UpperCorner>20037508 20037508</ows:UpperCorner>
		</ows:BoundingBox>
		<ows:SupportedCRS>urn:ogc:def:crs:EPSG:6.18:3:3857</ows:SupportedCRS>
		<WellKnownScaleSet>urn:ogc:def:wkss:OGC:1.0:GoogleMapsCompatible</WellKnownScaleSet>
		*/
		XElement* pxTileMatrixSet = pxParent->AddChild("TileMatrixSet", NULL);

		//XElement* pxTitle = pxTileMatrixSet->AddChild("Title", "ows");
		//pxTitle->AddChildText(identifier); 

		//XElement* pxAbstract = pxTileMatrixSet->AddChild("Abstract", "ows");
		//pxAbstract->AddChildText(identifier);

		XElement* pxIdentifier = pxTileMatrixSet->AddChild("Identifier", "ows");
		pxIdentifier->AddChildText(identifier);

		//XElement* pxBoundingBox = pxTileMatrixSet->AddChild("BoundingBox", "ows");
		////pxBoundingBox->SetAttribute("crs","urn:ogc:def:crs:EPSG:6.18:3:3857", NULL);
		//XElement* pxLowerCorner = pxBoundingBox->AddChild("LowerCorner", "ows");
		//pxLowerCorner->AddChildText("-20037508 -20037508");
		//XElement* pxUpperCorner = pxBoundingBox->AddChild("UpperCorner", "ows");
		//pxUpperCorner->AddChildText("20037508 20037508");

		XElement* pxSupportedCRS = pxTileMatrixSet->AddChild("SupportedCRS","ows");
		//pxSupportedCRS->AddChildText("urn:ogc:def:crs:EPSG:6.18:3:3857");
		pxSupportedCRS->AddChildText("urn:ogc:def:crs:EPSG::4326");

		//XElement* pxWellKnownScaleSet = pxTileMatrixSet->AddChild("WellKnownScaleSet", NULL);
		//pxWellKnownScaleSet->AddChildText("urn:ogc:def:wkss:OGC:1.0:GoogleMapsCompatible");

		size_t count = sizeof(denomiators) / sizeof(char*);
		for(size_t i=0; i<count; i++)
		{
			XElement* pxTileMatrix = pxTileMatrixSet->AddChild("TileMatrix", NULL);
			g_sprintf(str, "GoogleCRS84Quad:%d",i);
			XElement* pxIdentifier = pxTileMatrix->AddChild("Identifier", "ows");
			pxIdentifier->AddChildText(str);

			XElement* pxScaleDenominator = pxTileMatrix->AddChild("ScaleDenominator", NULL);
			pxScaleDenominator->AddChildText(denomiators[i]);

			XElement* pxTopLeftCorner = pxTileMatrix->AddChild("TopLeftCorner", NULL);
			pxTopLeftCorner->AddChildText(top_left_corner);

			XElement* pxTileWidth = pxTileMatrix->AddChild("TileWidth", NULL);
			pxTileWidth->AddChildText(tile_width);
			 
			XElement* pxTileHeight= pxTileMatrix->AddChild("TileHeight", NULL);
			pxTileHeight->AddChildText(tile_height);

			g_uint cols = pow(2.0f, (float)i);
			g_sprintf(str, "%d", cols);
			XElement* pxMatrixWidth = pxTileMatrix->AddChild("MatrixWidth", NULL);
			pxMatrixWidth->AddChildText(str);

			g_uint rows = pow(2.0f, (float)(i-1));
			g_sprintf(str, "%d", rows < 1 ? 1 : rows);
			XElement* pxMatrixHeight= pxTileMatrix->AddChild("MatrixHeight", NULL);
			pxMatrixHeight->AddChildText(str);

		}
	}

	void CapabilitiesHandler::AddTileMatrixSet_PGIS_1_0_0(XElement* pxParent)
	{
		const char* tile_width = "256";
		const char* tile_height= "256";
		const char* identifier = "PGIS";
		const char* top_left_corner = "256.0 -256.0";
		char* denomiators[]={"198784804.9879885",
							"99392402.4939943",
							"49696201.2469971",
							"24848100.6234986",
							"12424050.3117493",
							"6212025.1558746",
							"3106012.5779373",
							"1553006.2889687",
							"776503.1444843",
							"388251.5722422",
							"194125.7861211",
							"97062.8930605",
							"48531.4465303",
							"24265.7232651",
							"12132.8616326",
							"6066.4308163",
							"3033.2154081",
							"1516.6077041",
							"758.303852",
							"379.151926"};
				
		char str[AUGE_NAME_MAX] = {0};
		
		/*
		<ows:Title>GoogleCRS84Quad</ows:Title>
		<ows:Abstract>GoogleMap切片标准</ows:Abstract>
		<ows:Identifier>GoogleCRS84Quad</ows:Identifier>
		<ows:BoundingBox crs="urn:ogc:def:crs:EPSG:6.18:3:3857">
		<ows:LowerCorner>-20037508 -20037508</ows:LowerCorner>
		<ows:UpperCorner>20037508 20037508</ows:UpperCorner>
		</ows:BoundingBox>
		<ows:SupportedCRS>urn:ogc:def:crs:EPSG:6.18:3:3857</ows:SupportedCRS>
		<WellKnownScaleSet>urn:ogc:def:wkss:OGC:1.0:GoogleMapsCompatible</WellKnownScaleSet>
		*/
		XElement* pxTileMatrixSet = pxParent->AddChild("TileMatrixSet", NULL);

		//XElement* pxTitle = pxTileMatrixSet->AddChild("Title", "ows");
		//pxTitle->AddChildText(identifier); 

		//XElement* pxAbstract = pxTileMatrixSet->AddChild("Abstract", "ows");
		//pxAbstract->AddChildText(identifier);

		XElement* pxIdentifier = pxTileMatrixSet->AddChild("Identifier", "ows");
		pxIdentifier->AddChildText(identifier);

		//XElement* pxBoundingBox = pxTileMatrixSet->AddChild("BoundingBox", "ows");
		////pxBoundingBox->SetAttribute("crs","urn:ogc:def:crs:EPSG:6.18:3:3857", NULL);
		//XElement* pxLowerCorner = pxBoundingBox->AddChild("LowerCorner", "ows");
		//pxLowerCorner->AddChildText("-20037508 -20037508");
		//XElement* pxUpperCorner = pxBoundingBox->AddChild("UpperCorner", "ows");
		//pxUpperCorner->AddChildText("20037508 20037508");

		XElement* pxSupportedCRS = pxTileMatrixSet->AddChild("SupportedCRS","ows");
		//pxSupportedCRS->AddChildText("urn:ogc:def:crs:EPSG:6.18:3:3857");
		pxSupportedCRS->AddChildText("urn:ogc:def:crs:EPSG::4326");

		//XElement* pxWellKnownScaleSet = pxTileMatrixSet->AddChild("WellKnownScaleSet", NULL);
		//pxWellKnownScaleSet->AddChildText("urn:ogc:def:wkss:OGC:1.0:GoogleMapsCompatible");

		size_t count = sizeof(denomiators) / sizeof(char*);
		for(size_t i=0; i<count; i++)
		{
			XElement* pxTileMatrix = pxTileMatrixSet->AddChild("TileMatrix", NULL);
			g_sprintf(str, "PGIS:%d",i+2);
			XElement* pxIdentifier = pxTileMatrix->AddChild("Identifier", "ows");
			pxIdentifier->AddChildText(str);

			XElement* pxScaleDenominator = pxTileMatrix->AddChild("ScaleDenominator", NULL);
			pxScaleDenominator->AddChildText(denomiators[i]);

			XElement* pxTopLeftCorner = pxTileMatrix->AddChild("TopLeftCorner", NULL);
			pxTopLeftCorner->AddChildText(top_left_corner);

			XElement* pxTileWidth = pxTileMatrix->AddChild("TileWidth", NULL);
			pxTileWidth->AddChildText(tile_width);
			 
			XElement* pxTileHeight= pxTileMatrix->AddChild("TileHeight", NULL);
			pxTileHeight->AddChildText(tile_height);

			g_uint cols = pow(2.0f, (float)i);
			g_sprintf(str, "%d", cols);
			XElement* pxMatrixWidth = pxTileMatrix->AddChild("MatrixWidth", NULL);
			pxMatrixWidth->AddChildText(str);

			g_uint rows = pow(2.0f, (float)(i-1));
			g_sprintf(str, "%d", rows < 1 ? 1 : rows);
			XElement* pxMatrixHeight= pxTileMatrix->AddChild("MatrixHeight", NULL);
			pxMatrixHeight->AddChildText(str);

		}
	}
}