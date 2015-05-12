#include "CapabilitiesHandler.h"
#include "CapabilitiesRequest.h"
#include "CapabilitiesResponse.h"
#include "WProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"

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
		pRequest->Info();
		return pRequest;
	}

	WebRequest*	CapabilitiesHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
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
			g_sprintf(msg, "WPS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* CapabilitiesHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		CapabilitiesRequest* pRequest = static_cast<CapabilitiesRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{
			pWebResponse = WriteCapabilities_1_0_0(pRequest, pWebContext);
		}
		else if(strcmp(version,"1.3.0")==0)
		{
			pWebResponse = WriteCapabilities_1_3_0(pRequest, pWebContext); 
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WPS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_0_0(CapabilitiesRequest* pRequest, WebContext* pWebContext) 
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath();

		char wps_xlink[AUGE_MSG_MAX];
		g_sprintf(wps_xlink, "http://%s/%s/%s/mgr?service=wps",	pRequest->GetHost(), 
			AUGE_VIRTUAL_NAME,
			pRequest->GetUser()); 

		char capa_name[AUGE_PATH_MAX];
		char capa_path[AUGE_PATH_MAX];
		g_sprintf(capa_name, "wps_capabilities_1_1_0");
		auge_make_path(capa_path, NULL, cache_path, capa_name,"xml");

		XElement  *pxNode = NULL; 
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("WPS_Capabilities", NULL, NULL);
		SetRooteNode_1_1_0(pxRoot, "1.1.0");

		// WPS_Capabilities-->ServiceIdentification
		AddServiceIdentificationNode_1_1_0(pxRoot);

		// WPS_Capabilities-->ServiceProvider
		AddServiceProviderNode_1_1_0(pxRoot);

		// WPS_Capabilities-->OperationsMetadata
		AddOperationsMetadataNode_1_1_0(pxRoot, wps_xlink);

		// WPS_Capabilities-->ProcessOfferings
		AddProcessOfferingsNode_1_1_0(pxRoot, wps_xlink);

		// WPS_Capabilities-->Languages
		AddLanguagesNode_1_1_0(pxRoot, wps_xlink);

		

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		CapabilitiesResponse* pResponse = new CapabilitiesResponse(pRequest);
		pResponse->SetPath(capa_path);

		return pResponse;
	}

	CapabilitiesResponse* CapabilitiesHandler::WriteCapabilities_1_3_0(CapabilitiesRequest* pRequest, WebContext* pWebContext)
	{
		//return WriteCapabilities_1_0_0(pRequest, pWebContext);
		return NULL;
	}

	void CapabilitiesHandler::SetRooteNode_1_1_0(XElement* pxRoot, const char* version)
	{
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wps",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wps","wps");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", version, NULL); 
	}

	void CapabilitiesHandler::AddServiceIdentificationNode_1_1_0(XElement* pxParent)
	{	
		XElement *pxService = pxParent->AddChild("ServiceIdentification", "ows");
		XElement *pxNode = pxNode = pxService->AddChild("Name", "ows");
		pxNode->SetChildText("WPS");
		pxNode = pxNode = pxService->AddChild("Title", "ows");
		pxNode->SetChildText("Auge Web Processing Service");
		pxNode = pxNode = pxService->AddChild("Abstract", "ows");
		// WPS_Capabilities-->ServiceIdentification-->KeywordList
		XElement *pxKeywords = pxService->AddChild("KeywordList", "ows");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText("WPS");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText("AugeGIS");
		// WPS_Capabilities-->ServiceIdentification-->ServiceType
		pxNode = pxService->AddChild("ServiceType", "ows");
		pxNode->SetChildText("WPS");
		pxNode = pxService->AddChild("ServiceVersion", "ows");
		pxNode->SetChildText("1.1.0");
		// WPS_Capabilities-->ServiceIdentification-->Fees
		XElement *pxFees = pxService->AddChild("Fees", "ows");
		pxFees->SetChildText("NONE");
		// WPS_Capabilities-->ServiceIdentification-->AccessConstraints
		XElement *pxConstraints = pxService->AddChild("AccessConstraints", "ows");
		pxConstraints->SetChildText("NONE");
	}

	void CapabilitiesHandler::AddServiceProviderNode_1_1_0(XElement* pxParent)
	{
		XElement* pxProvider = pxParent->AddChild("ServiceProvider", "ows");
		XElement* pxProviderName = pxProvider->AddChild("ProviderName","ows");
		//pxConstraints->SetChildText("Auge GIS");
		XElement* pxServiceContact = pxProvider->AddChild("ServiceContact","ows");
		XElement* pxNode = pxServiceContact->AddChild("IndividualName","ows");
		pxNode = pxServiceContact->AddChild("PositionName","ows");
		XElement* pxContactInfo = pxServiceContact->AddChild("PositionName","ows");
		XElement* pxPhone = pxContactInfo->AddChild("Phone","ows");
		pxNode = pxPhone->AddChild("Voice","ows");
		pxNode = pxPhone->AddChild("Facsimile","ows");
		XElement* pxAddress = pxContactInfo->AddChild("Address","ows");
		pxNode = pxAddress->AddChild("City","ows");
		pxNode = pxAddress->AddChild("AdministrativeArea","ows");
		pxNode = pxAddress->AddChild("Country","ows");
	}

	void CapabilitiesHandler::AddOperationsMetadataNode_1_1_0(XElement* pxParent, const char* wps_xlink)
	{
		XElement* pxOperationsMetadata = pxParent->AddChild("OperationsMetadata", "ows");

		// Service Handlers
		WProcessEngine* pFeatureEngine = (WProcessEngine*)augeGetWebEngineInstance();
		std::vector<WebHandler*>& handlers = pFeatureEngine->m_handlers;
		std::vector<WebHandler*>::iterator iter;
		for(iter=handlers.begin(); iter!=handlers.end(); iter++)
		{
			WebHandler* handler = *iter;

			// WPS_Capabilities-->OperationsMetadata-->Operation (GetCapabilities)
			XElement* pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
			pxOperation->SetAttribute("name", handler->GetName(),NULL);
			// WPS_Capabilities-->OperationsMetadata-->Operation-->DCP
			XElement* pxDCP = pxOperation->AddChild("DCP", "ows");
			// WPS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
			XElement* pxHTTP = pxDCP->AddChild("HTTP", "ows");
			// WPS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
			XElement* pxNode = pxHTTP->AddChild("Get", "ows");
			pxNode->SetAttribute("href",wps_xlink,"xlink");
			pxNode = pxHTTP->AddChild("Post", "ows");
			pxNode->SetAttribute("href",wps_xlink,"xlink");
			// WPS_Capabilities-->OperationsMetadata-->Operation-->Parameter(AcceptVersion)
			XElement* pxParameter = pxOperation->AddChild("Parameter", "ows");
			pxNode = pxParameter->AddChild("Value", "ows");
			pxNode->AddChildText("1.0.0");
			pxNode = pxParameter->AddChild("Value", "ows");
			pxNode->AddChildText("1.1.0"); 
			pxParameter = pxOperation->AddChild("AcceptFormats", "ows");
			pxNode = pxParameter->AddChild("Value", "ows");
			pxNode->AddChildText("text/xml");
		}
	}

	void CapabilitiesHandler::AddProcessOfferingsNode_1_1_0(XElement* pxParent, const char* wps_xlink)
	{
		XElement* pxProcessOfferings = pxParent->AddChild("ProcessOfferings", "wps");
		XElement* pxProecess = pxProcessOfferings->AddChild("Process","wps");

		XElement* pxIdentifier = pxProecess->AddChild("Identifier", "ows");
		pxIdentifier->AddChildText("buffer");
		XElement* pxTitle = pxProecess->AddChild("Title", "ows");
		pxTitle->AddChildText("Buffer a polygon feature");
		XElement* pxAbstract = pxProecess->AddChild("Abstract", "ows");
		pxTitle->AddChildText("Buffer the polygon coordinates found in one GML stream by a given buffer distance, and output the results in GML.");

		XElement* pxMetadata = pxProecess->AddChild("Metadata", "ows");
		pxMetadata->SetAttribute("title","buffer","xlink"); 
		pxMetadata->SetAttribute("title","polygon","xlink");
	}

	void CapabilitiesHandler::AddLanguagesNode_1_1_0(XElement* pxParent, const char* wps_xlink)
	{
		XElement* pxDefault = pxParent->AddChild("Default", "ows");
		XElement* pxLanguage = pxDefault->AddChild("Language", "ows");
		pxLanguage->AddChildText("zh-CN");

		XElement* pxSupported = pxParent->AddChild("Supported", "ows");
		pxLanguage = pxSupported->AddChild("Language", "ows");
		pxLanguage->AddChildText("zh-CN");
	}
}
