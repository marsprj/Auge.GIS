#include "WFEngine.h"
#include "GetCapabilitiesRequest.h"
#include "GetCapabilitiesResponse.h"
#include "DescribeFeatureTypeRequest.h"
#include "DescribeFeatureTypeResponse.h"
#include "GetFeatureRequest.h"
#include "GetFeatureResponse.h"

#include "AugeCore.h"
#include "AugeField.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"
#include "AugeGeometry.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WebFeatureEngine g_webFeatureEngine;
		return &g_webFeatureEngine;
	}

	WebFeatureEngine::WebFeatureEngine():
	m_handler(NULL),
		m_requests("GetCapabilities;DescribeFeatureType;GetFeature")
	{

	}

	WebFeatureEngine::~WebFeatureEngine()
	{

	}

	const char*	WebFeatureEngine::GetID()
	{
		return "WFEngine";
	}

	const char*	WebFeatureEngine::GetType()
	{
		return "wfs";
	}

	const char*	WebFeatureEngine::GetDescription()
	{
		return "OGC WFS";
	}

	void* WebFeatureEngine::GetHandler()
	{
		return m_handler;
	}

	void WebFeatureEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WebFeatureEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WebFeatureEngine::ParseRequest(const char* url)
	{
		GPropertySet props;
		props.Parse(url,"&");

		const char* request = props.GetValue("request");
		if(request==NULL)
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			return NULL;
		}

		WebRequest* pWebRequest = NULL;
		if(g_stricmp(request, "GetCapabilities")==0)
		{
			GetCapabilitiesRequest* pRequest = new GetCapabilitiesRequest();
			if(!pRequest->Create(props))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
		else if(g_stricmp(request, "DescribeFeatureType")==0)
		{
			DescribeFeatureTypeRequest* pRequest = new DescribeFeatureTypeRequest();
			if(!pRequest->Create(props))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
		else if(g_stricmp(request, "GetFeature")==0)
		{
			GetFeatureRequest* pRequest = new GetFeatureRequest();
			if(!pRequest->Create(props))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		else if(g_stricmp(request, "GetFeatureInfo")==0)
		{

		}
		else if(g_stricmp(request, "Transaction")==0)
		{

		}


		return pWebRequest;
	}

	WebRequest*	WebFeatureEngine::ParseRequest(XDocument* pxDoc)
	{
		GError* pError = augeGetErrorInstance();
		pError->SetError("WFS do not support xml request");
		return NULL;
	}

	WebResponse* WebFeatureEngine::Execute(WebRequest* pWebRequest)
	{
		return NULL;
	}

	WebResponse* WebFeatureEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		WebResponse		*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		if(!Support(request))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "WFS doesn't Request [%s].", request);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		if(g_stricmp(request, "GetCapabilities")==0)
		{
			GetCapabilitiesRequest* pRequest = static_cast<GetCapabilitiesRequest*>(pWebRequest);
			pWebResponse = GetCapabilities(pRequest, pWebContext, pMap);
		}
		else if(g_stricmp(request, "DescribeFeatureType")==0)
		{
			DescribeFeatureTypeRequest* pRequest = static_cast<DescribeFeatureTypeRequest*>(pWebRequest);
			pWebResponse = DescribeFeatureType(pRequest, pWebContext, pMap);
		}
		else if(g_stricmp(request, "GetFeature")==0)
		{
			GetFeatureRequest* pRequest = static_cast<GetFeatureRequest*>(pWebRequest);
			pWebResponse = GetFeature(pRequest, pWebContext, pMap);
		}

		return pWebResponse;
	}

	bool WebFeatureEngine::Support(const char* request)
	{
		if(request==NULL)
		{
			return false;
		}

		size_t pos = m_requests.find(request);
		return (pos!=std::string::npos);
	}

	WebResponse* WebFeatureEngine::GetCapabilities(GetCapabilitiesRequest* pRequest, WebContext* pWebContext, Map* pMap)
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath();

		char capa_path[AUGE_PATH_MAX];
		auge_make_path(capa_path, NULL, cache_path, "wfs_capabilities_1_1_0","xml");

		//if(access(capa_path,4))
		{
			WriteCapabilities(pRequest->GetVersion(), pWebContext, pMap);
		}

		GetCapabilitiesResponse* pResponse = new GetCapabilitiesResponse(pRequest);		
		pResponse->SetPath(capa_path);
		return pResponse;
	}

	bool  WebFeatureEngine::WriteCapabilities(const char* version, WebContext* pWebContext, Map* pMap)
	{
		if(strcmp(version, "1.0.0")==0)
		{
			return false;
		}
		else if(strcmp(version, "1.1.0")==0)
		{
			return WriteCapabilities_1_1_0(pWebContext, pMap);
		}
		return false;
	}

	bool WebFeatureEngine::WriteCapabilities_1_1_0(WebContext* pWebContext, Map* pMap)
	{
		const char* cache_path = pWebContext->GetCacheProtocolPath();

		char str[AUGE_MSG_MAX];
		char capa_path[AUGE_PATH_MAX];
		auge_make_path(capa_path, NULL, cache_path, "wfs_capabilities_1_1_0","xml");

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("WFS_Capabilities", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		// WFS_Capabilities-->ServiceIdentification
		XElement *pxService = pxRoot->AddChild("ServiceIdentification", "ows");
		pxNode = pxNode = pxService->AddChild("Name", "ows");
		pxNode->SetChildText("WFS");
		pxNode = pxNode = pxService->AddChild("Title", "ows");
		pxNode->SetChildText("Auge Web Map Service");
		pxNode = pxNode = pxService->AddChild("Abstract", "ows");
		// WFS_Capabilities-->ServiceIdentification-->KeywordList
		XElement *pxKeywords = pxService->AddChild("KeywordList", "ows");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText("WFS");
		pxNode = pxNode = pxKeywords->AddChild("Keyword", "ows");
		pxNode->SetChildText("AugeGIS");
		// WFS_Capabilities-->ServiceIdentification-->ServiceType
		pxNode = pxService->AddChild("ServiceType", "ows");
		pxNode->SetChildText("WFS");
		pxNode = pxService->AddChild("ServiceVersion", "ows");
		pxNode->SetChildText("1.1.0");
		// WFS_Capabilities-->ServiceIdentification-->Fees
		XElement *pxFees = pxService->AddChild("Fees", "ows");
		pxFees->SetChildText("NONE");
		// WFS_Capabilities-->ServiceIdentification-->AccessConstraints
		XElement *pxConstraints = pxService->AddChild("AccessConstraints", "ows");
		pxConstraints->SetChildText("NONE");

		// WFS_Capabilities-->ServiceProvider
		XElement* pxProvider = pxRoot->AddChild("ServiceProvider", "ows");
		XElement* pxProviderName = pxProvider->AddChild("ProviderName","ows");
		pxConstraints->SetChildText("Auge GIS");
		XElement* pxServiceContact = pxProvider->AddChild("ServiceContact","ows");
		pxNode = pxServiceContact->AddChild("IndividualName","ows");
		pxNode = pxServiceContact->AddChild("PositionName","ows");
		XElement* pxContactInfo = pxServiceContact->AddChild("PositionName","ows");
		XElement* pxPhone = pxContactInfo->AddChild("Phone","ows");
		pxNode = pxPhone->AddChild("Voice","ows");
		pxNode = pxPhone->AddChild("Facsimile","ows");
		XElement* pxAddress = pxContactInfo->AddChild("Address","ows");
		pxNode = pxAddress->AddChild("City","ows");
		pxNode = pxAddress->AddChild("AdministrativeArea","ows");
		pxNode = pxAddress->AddChild("Country","ows");

		// WFS_Capabilities-->OperationsMetadata
		XElement* pxOperationsMetadata = pxRoot->AddChild("OperationsMetadata", "ows");

		// WFS_Capabilities-->OperationsMetadata-->Operation (GetCapabilities)
		XElement* pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "GetCapabilities",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		XElement* pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		XElement* pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(AcceptVersion)
		XElement* pxParameter = pxOperation->AddChild("Parameter", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("1.0.0");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("1.1.0");
		pxParameter = pxOperation->AddChild("AcceptFormats", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("text/xml");

		// WFS_Capabilities-->OperationsMetadata-->Operation (DescribeFeatureType)
		pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "DescribeFeatureType",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(outpurFormat)
		pxParameter = pxOperation->AddChild("outpurFormat", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("text/xml;subtype=gml/3.1.1");

		// WFS_Capabilities-->OperationsMetadata-->Operation (GetFeature)
		pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "GetFeature",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(resultType)
		pxParameter = pxOperation->AddChild("outpurFormat", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("results");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("hits");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(outpurFormat)
		pxParameter = pxOperation->AddChild("outpurFormat", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("text/xml;subtype=gml/3.1.1");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("GML2");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("KML");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("WKT");

		// WFS_Capabilities-->OperationsMetadata-->Operation (GetGmlObject)
		pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "GetGmlObject",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");

		// WFS_Capabilities-->OperationsMetadata-->Operation (LockFeature)
		pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "GetGmlObject",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(outpurFormat)
		pxParameter = pxOperation->AddChild("releaseAction", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("ALL");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("SOME");
		
		// WFS_Capabilities-->OperationsMetadata-->Operation (GetFeatureWithLock)
		pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "GetFeatureWithLock",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(resultType)
		pxParameter = pxOperation->AddChild("outpurFormat", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("results");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("hits");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(outpurFormat)
		pxParameter = pxOperation->AddChild("outpurFormat", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("text/xml;subtype=gml/3.1.1");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("GML2");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("KML");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("WKT");

		// WFS_Capabilities-->OperationsMetadata-->Operation (Transaction)
		pxOperation = pxOperationsMetadata->AddChild("Operation", "ows");
		pxOperation->SetAttribute("name", "Transaction",NULL);
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP
		pxDCP = pxOperation->AddChild("DCP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->HTTP
		pxHTTP = pxDCP->AddChild("HTTP", "ows");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->DCP-->Get/Post
		pxNode = pxHTTP->AddChild("Get", "ows");
		pxNode->SetAttribute("href","","xlink");
		pxNode = pxHTTP->AddChild("Post", "ows");
		pxNode->SetAttribute("href","","xlink");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(inpurFormat)
		pxParameter = pxOperation->AddChild("inpurFormat", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("text/xml;subtype=gml/3.1.1");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(idgen)
		pxParameter = pxOperation->AddChild("idgen", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("GenerateNew");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("UseExisting");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("ReplaceDuplicate");
		// WFS_Capabilities-->OperationsMetadata-->Operation-->Parameter(releaseAction)
		pxParameter = pxOperation->AddChild("releaseAction", "ows");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("ALL");
		pxNode = pxParameter->AddChild("Value", "ows");
		pxNode->AddChildText("SOME");

		// WFS_Capabilities-->FeatureTypeList
		XElement* pxTypeList = pxRoot->AddChild("FeatureTypeList", "ows");
		XElement* pxOperations = pxTypeList->AddChild("Operations", "ows");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Query");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Insert");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Update");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Delete");
		pxOperation = pxOperations->AddChild("Operation", "ows");
		pxOperation->AddChildText("Lock");

		// WFS_Capabilities-->FeatureTypeList->FeatureType
		Layer* pLayer = NULL;
		g_uint lc = pMap->GetLayerCount();
		for(g_uint i=0; i<lc; i++)
		{
			pLayer = pMap->GetLayer(i);
			if(pLayer!=NULL)
			{
				if(pLayer->GetType()==augeLayerFeature)
				{
					FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
					FeatureClass* pFeatureClass = pFLayer->GetFeatureClass();
					if(pFeatureClass!=NULL)
					{
						XElement* pxType = pxTypeList->AddChild("FeatureType", NULL);
						pxNode = pxType->AddChild("Name",NULL);
						pxNode->SetChildText(pFeatureClass->GetName());
						pxNode = pxType->AddChild("Name",NULL);
						pxNode->SetChildText(pFeatureClass->GetName());
						pxNode = pxType->AddChild("Abstract",NULL);

						//Keywords
						pxKeywords = pxType->AddChild("KeywordList", "ows");
						pxNode = pxKeywords->AddChild("Keyword", "ows");
						pxNode->SetChildText(pFeatureClass->GetName());

						//DefaultSRS
						g_snprintf(str, AUGE_MSG_MAX, "EPSG:%d", pFeatureClass->GetSRID());
						pxNode = pxType->AddChild("DefaultSRS", "ows");
						pxNode->SetChildText(str);

						//WGS84BoundingBox
						GEnvelope& extent = pFeatureClass->GetExtent();
						XElement* px84Bounding = pxType->AddChild("WGS84BoundingBox", "ows");
						pxNode = px84Bounding->AddChild("LowerCorner","ows");
						if(extent.IsValid())
						{
							g_snprintf(str, AUGE_MSG_MAX, "%.7f %.7f", extent.m_xmin, extent.m_ymin);
							pxNode->AddChildText(str);
						}
						pxNode = px84Bounding->AddChild("LowerCorner","ows");
						if(extent.IsValid())
						{
							g_snprintf(str, AUGE_MSG_MAX, "%.7f %.7f", extent.m_xmax, extent.m_ymax);
							pxNode->AddChildText(str);
						}
					}
				}
			}
		}

		// WFS_Capabilities-->OGC_FilterCapabilities
		XElement* pxFilter = pxRoot->AddChild("OGC_FilterCapabilities","ogc");

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		return true;
	}

	WebResponse* WebFeatureEngine::DescribeFeatureType(DescribeFeatureTypeRequest* pRequest, WebContext* pWebContext, Map* pMap)
	{	
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();
		
		typeName = pRequest->GetTypeName();
		pLayer = pMap->GetLayer(typeName);
		if(pLayer==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		if(pLayer->GetType()!=augeLayerFeature)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s is not a Feature Layer",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		FeatureClass* pFeatureClass = NULL;
		FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
		pFeatureClass = pFLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		char cache_name[AUGE_NAME_MAX];
		char cache_file[AUGE_PATH_MAX];
		const char* cache_path = pWebContext->GetCacheProtocolPath();
		g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
		auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

		//if(access(capa_path,4))
		{
			WriteDescribeFeatureType(pRequest->GetVersion(), pWebContext, typeName, pFeatureClass);
		}

		DescribeFeatureTypeResponse* pResponse = new DescribeFeatureTypeResponse(pRequest);		
		pResponse->SetPath(cache_file);
		return pResponse;
	}

	bool WebFeatureEngine::WriteDescribeFeatureType(const char* version,WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	{
		if(strcmp(version, "1.0.0")==0)
		{
			return false;
		}
		else if(strcmp(version, "1.1.0")==0)
		{
			return WriteDescribeFeatureType_1_1_0(pWebContext, typeName, pFeatureClass);
		}
		return false;
	}

	bool WebFeatureEngine::WriteDescribeFeatureType_1_1_0(WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	{
		char str[AUGE_NAME_MAX];
		char cache_name[AUGE_NAME_MAX];
		char cache_file[AUGE_PATH_MAX];
		const char* cache_path = pWebContext->GetCacheProtocolPath();
		g_snprintf(cache_name, AUGE_NAME_MAX,"%s_wfs_%s_describe_feauretype_1_1_0", pWebContext->GetService(), typeName);
		auge_make_path(cache_file, NULL, cache_path, cache_name,"xml");

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode("schema", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		// schema-->import
		XElement* pxImport = pxRoot->AddChild("import", "xsd");
		pxImport->SetAttribute("namespace","http://www.opengis.net/gml", NULL);
		// schema-->complexType
		g_sprintf(str,"%sType", typeName);
		XElement* pxComplexType = pxRoot->AddChild("complexType", "xsd");
		pxComplexType->SetAttribute("name", str, NULL);
		// schema-->complexType-->complexContent
		XElement* pxComplexContent = pxComplexType->AddChild("complexContent","xsd");
		// schema-->complexType-->complexContent-->extentsion
		XElement* pxExtentsion = pxComplexContent->AddChild("extentsion", "xsd");
		// schema-->complexType-->complexContent-->extentsion-->sequence
		XElement* pxSequence = pxExtentsion->AddChild("extentsion", "xsd");
		pxSequence->SetAttribute("base","gml:AbstractFeatureType", NULL);

		GField	*pField  = NULL;
		GFields	*pFields = pFeatureClass->GetFields();
		g_uint count = pFields->Count();

		// schema-->complexType-->complexContent-->extentsion-->sequence-->element
		XElement* pxElement = NULL;
		for(g_uint i=0; i<count; i++)
		{
			pField = pFields->GetField(i);
			pxElement = pxSequence->AddChild("element", "xsd");		
			pxElement->SetAttribute("name",pField->GetName(), NULL);
			pxElement->SetAttribute("nillable",pField->IsNullable()?"true":"false", NULL);
			pxElement->SetAttribute("minOccurs","0", NULL);
			pxElement->SetAttribute("maxOccurs","1", NULL);

			if(pField->GetType()==augeFieldTypeGeometry)
			{
				augeGeometryType gtype = pField->GetGeometryDef()->GeometryType();
				const char* sss = GetOgcGeometryType(gtype);
				g_sprintf(str,"gml:%sPropertyType",sss);
				pxElement->SetAttribute("type",str, NULL);
			}
			else
			{
				g_sprintf(str, "xsd:%s", GetOgcFieldType(pField->GetType()));
				pxElement->SetAttribute("type",str, NULL);
			}
			
		}

		// schema-->element
		pxElement = pxRoot->AddChild("element", "xsd");
		g_sprintf(str, "%sType", typeName);
		pxElement->SetAttribute("type",str, NULL);
		pxElement->SetAttribute("name",typeName, NULL);
		pxElement->SetAttribute("substitutionGroup","gml:_Feature", NULL);

		pxDoc->Save(cache_file, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		return true;
	}

	WebResponse* WebFeatureEngine::GetFeature(GetFeatureRequest* pRequest, WebContext* pWebContext, Map* pMap)
	{
		const char* typeName = NULL;
		Layer* pLayer = NULL;
		GLogger *pLogger = augeGetLoggerInstance();

		typeName = pRequest->GetTypeName();
		pLayer = pMap->GetLayer(typeName);
		if(pLayer==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Service %s has not FeatureType %s,",pWebContext->GetService(), typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		if(pLayer->GetType()!=augeLayerFeature)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s is not a Feature Layer",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		FeatureClass* pFeatureClass = NULL;
		FeatureLayer* pFLayer = static_cast<FeatureLayer*>(pLayer);
		pFeatureClass = pFLayer->GetFeatureClass();
		if(pFeatureClass==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Cannot Get FeatureType %s",typeName);
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		const char* version = pRequest->GetVersion();
		WebResponse* pWebResponse = NULL;
		if(strcmp(version, "1.0.0")==0)
		{
			pWebResponse = GetFeature_1_1_0(pRequest, pWebContext, typeName, pFeatureClass);
		}
		else if(strcmp(version, "1.1.0")==0)
		{
			pWebResponse = GetFeature_1_1_0(pRequest, pWebContext, typeName, pFeatureClass);
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Unsupported Version %s", version);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			pWebResponse = pExpResopnse;
		}
		return pWebResponse;
	}
	
	WebResponse* WebFeatureEngine::GetFeature_1_1_0(GetFeatureRequest* pRequest,WebContext* pWebContext, const char* typeName, FeatureClass* pFeatureClass)
	{
		FeatureCursor* pCursor = NULL;
		pCursor = pFeatureClass->Query();

		GetFeatureResponse *pResponse = new GetFeatureResponse(pRequest);
		pResponse->Create(pCursor);
		return pResponse;
	}

	const char* WebFeatureEngine::GetOgcFieldType(augeFieldType type)
	{
		switch(type)
		{
		case augeFieldTypeBool:
			return "boolean";
		case augeFieldTypeChar:
			return "character";
		case augeFieldTypeShort:
			return "short";
		case augeFieldTypeInt:
			return "int";
		case augeFieldTypeLong:
			return "long";
		case augeFieldTypeInt64:
			return "int64";
		case augeFieldTypeFloat:
			return "float";
		case augeFieldTypeDouble:
			return "double";
		case augeFieldTypeString:
			return "string";
		case augeFieldTypeTime:
			return "timestamp";
		case augeFieldTypeBLOB:
			return "blob";
		case augeFieldTypeGeometry:
			return "geometry";
		case augeFieldTypeCLOB:
			return "clob";
		case augeFieldTypeNCLOB:
			return "nclob";
		case augeFieldTypeSerial:
			return "serial";
		}
		return "";
	}

	const char*	WebFeatureEngine::GetOgcGeometryType(augeGeometryType type)
	{
		switch(type)
		{
		case augeGTPoint:
			return "Point";
		case augeGTLineString:
			return "LineString";
		case augeGTPolygon:
			return "Polygon";
		case augeGTMultiPoint:
			return "MultiPoint";
		case augeGTMultiLineString:
			return "MultiLineString";
		case augeGTMultiPolygon:
			return "MultiPolygon";
		}
		return "";
	}
}