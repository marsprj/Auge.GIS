#include "DescribeProcessHandler.h"
#include "DescribeProcessRequest.h"
#include "DescribeProcessResponse.h"
#include "WProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"

namespace auge
{
	DescribeProcessHandler::DescribeProcessHandler()
	{

	}

	DescribeProcessHandler::~DescribeProcessHandler()
	{

	}

	const char*	DescribeProcessHandler::GetName()
	{
		return "DescribeProcess";
	}

	WebRequest*	DescribeProcessHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeProcessRequest* pRequest = new DescribeProcessRequest();
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

	WebRequest*	DescribeProcessHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeProcessHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		DescribeProcessRequest* pRequest = new DescribeProcessRequest();
		//if(!pRequest->Create(cgi))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	pRequest = NULL;
		//}
		return pRequest;
	}

	WebResponse* DescribeProcessHandler::Execute(WebRequest* pWebRequest)
	{
		WebResponse* pWebResponse = NULL;
		DescribeProcessRequest* pRequest = static_cast<DescribeProcessRequest*>(pWebRequest);

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

	WebResponse* DescribeProcessHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		WebResponse* pWebResponse = NULL;
		DescribeProcessRequest* pRequest = static_cast<DescribeProcessRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{
			pWebResponse = WriteDescribeProcess_1_0_0(pRequest, pWebContext);
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

	DescribeProcessResponse* DescribeProcessHandler::WriteDescribeProcess_1_0_0(DescribeProcessRequest* pRequest, WebContext* pWebContext) 
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
		pxRoot = pxDoc->CreateRootNode("ProcessDescriptions", NULL,NULL);
		SetRooteNode_1_1_0(pxRoot, "1.1.0");
		
		std::vector<std::string>& identifiers = pRequest->m_identifiers;
		std::vector<std::string>::iterator iter;
		for(iter=identifiers.begin(); iter!=identifiers.end(); iter++)
		{
			std::string& identifier = *iter;
			// ProcessDescriptions-->ProcessDescription
			AddProcessDescriptionNode(pxRoot, identifier.c_str());
		}

		pxDoc->Save(capa_path, pWebContext->GetResponseEncoding(), 1);
		pxDoc->Release();

		DescribeProcessResponse* pResponse = new DescribeProcessResponse(pRequest);
		pResponse->SetPath(capa_path);

		return pResponse;
	}

	void DescribeProcessHandler::SetRooteNode_1_1_0(XElement* pxRoot, const char* version)
	{
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wps",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wps","wps");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetAttribute("version", version, NULL);
	}

	void DescribeProcessHandler::AddProcessDescriptionNode(XElement* pxParent, const char* identifier)
	{ 
		//wps:processVersion="2" storeSupported="true" statusSupported="false"
		XElement* pxDescription = pxParent->AddChild("ProcessDescription", NULL);
		pxDescription->SetAttribute("processVersion","2","wps");
		pxDescription->SetAttribute("storeSupported","true",NULL);
		pxDescription->SetAttribute("statusSupported","true",NULL);

		XElement* pxIdentifier = pxDescription->AddChild("Identifier", "ows");
		pxIdentifier->SetChildText(identifier);

		XElement* pxTitle = pxDescription->AddChild("Title", "ows");
		pxTitle->SetChildText(identifier);

		XElement* pxAbstract= pxDescription->AddChild("Title", "ows");
		pxAbstract->SetChildText(identifier);

		//<ows:Metadata xlink:title="spatial"/>
		//<ows:Metadata xlink:title="geometry"/>
		//<ows:Metadata xlink:title="buffer"/>
		//<ows:Metadata xlink:title="GML"/>
		//<wps:Profile>urn:ogc:wps:1.0.0:buffer</wps:Profile>
		//<wps:WSDL xlink:href="http://foo.bar/foo"/>

		XElement* pxMetadata= pxDescription->AddChild("Metadata","ows");
		pxMetadata->SetAttribute("title", "spatial", "xlink");

		pxMetadata= pxDescription->AddChild("Metadata","ows");
		pxMetadata->SetAttribute("title", "geometry", "xlink");
		
		pxMetadata= pxDescription->AddChild("Metadata","ows");
		pxMetadata->SetAttribute("title", "GML", "xlink");

		pxMetadata= pxDescription->AddChild("Metadata","ows");
		pxMetadata->SetAttribute("title", identifier, "xlink");

		char str[AUGE_NAME_MAX];
		g_sprintf(str, "urn:ogc:wps:1.0.0:%s", identifier);
		XElement* pxProfile= pxDescription->AddChild("Profile","wps");
		pxProfile->AddChildText(str);

		XElement* pxWSDL= pxDescription->AddChild("WSDL","wps");
		pxProfile->AddChildText("");

		AddDataInputsNode(pxDescription, identifier);

		AddProcessOutputsNode(pxDescription);
	}

	void DescribeProcessHandler::AddDataInputsNode(XElement* pxParent, const char* identifier)
	{
		XElement* pxDataInputes = pxParent->AddChild("DataInputs", "ows");

		XElement* pxInput = pxDataInputes->AddChild("Input",NULL);
		pxInput->SetAttribute("minOccurs","1",NULL);
		pxInput->SetAttribute("maxOccurs","1",NULL);

		XElement* pxIdentifier = pxInput->AddChild("Identifier", "ows");
		pxIdentifier->AddChildText("InputPolygon");
		XElement* pxTitle = pxInput->AddChild("Title", "ows");
		XElement* pxAbstract = pxInput->AddChild("Abstract", "ows");

		XElement* pxComplexData = pxInput->AddChild("ComplexData", NULL);
		pxComplexData->SetAttribute("maximumMegabytes", "5", NULL);

		//ComplexData-->Default
		XElement* pxDefault = pxComplexData->AddChild("Default", NULL);
		XElement* pxFormat = pxDefault->AddChild("Format", NULL);
		XElement* pxMimeType = pxFormat->AddChild("MimeType", NULL);
		pxMimeType->AddChildText("text/xml");
		XElement* pxEncoding = pxFormat->AddChild("Encoding", NULL);
		pxEncoding->AddChildText("base64");
		XElement* pxSchema = pxFormat->AddChild("Schema", NULL);
		pxSchema->AddChildText("http://127.0.0.1:8088/gml/3.1.0/polygon.xsd");

		//ComplexData-->Default
		XElement* pxSupported = pxComplexData->AddChild("Supported", NULL);
		pxFormat = pxSupported->AddChild("Format", NULL);
		pxMimeType = pxFormat->AddChild("MimeType", NULL);
		pxMimeType->AddChildText("text/xml");
		pxEncoding = pxFormat->AddChild("Encoding", NULL);
		pxEncoding->AddChildText("base64");
		pxSchema = pxFormat->AddChild("Schema", NULL);
		pxSchema->AddChildText("http://127.0.0.1:8088/gml/3.1.0/polygon.xsd");

	}

	void DescribeProcessHandler::AddProcessOutputsNode(XElement* pxParent)
	{
		XElement* pxOutputs = pxParent->AddChild("ProcessOutputs", "ows");

		XElement* pxOutput = pxOutputs->AddChild("Output",NULL);

		XElement* pxIdentifier = pxOutput->AddChild("Identifier", "ows");
		pxIdentifier->AddChildText("BufferedPolygon");
		XElement* pxTitle = pxOutput->AddChild("Title", "ows");
		XElement* pxAbstract = pxOutput->AddChild("Abstract", "ows");

		XElement* pxComplexOutput = pxOutput->AddChild("ComplexOutput", NULL);
		pxComplexOutput->SetAttribute("maximumMegabytes", "5", NULL);

		//ComplexData-->Default
		XElement* pxDefault = pxComplexOutput->AddChild("Default", NULL);
		XElement* pxFormat = pxDefault->AddChild("Format", NULL);
		XElement* pxMimeType = pxFormat->AddChild("MimeType", NULL);
		pxMimeType->AddChildText("text/xml");
		XElement* pxEncoding = pxFormat->AddChild("Encoding", NULL);
		pxEncoding->AddChildText("base64");
		XElement* pxSchema = pxFormat->AddChild("Schema", NULL);
		pxSchema->AddChildText("http://127.0.0.1:8088/gml/3.1.0/polygon.xsd");

		//ComplexData-->Default
		XElement* pxSupported = pxComplexOutput->AddChild("Supported", NULL);
		pxFormat = pxSupported->AddChild("Format", NULL);
		pxMimeType = pxFormat->AddChild("MimeType", NULL);
		pxMimeType->AddChildText("text/xml");
		pxEncoding = pxFormat->AddChild("Encoding", NULL);
		pxEncoding->AddChildText("base64");
		pxSchema = pxFormat->AddChild("Schema", NULL);
		pxSchema->AddChildText("http://127.0.0.1:8088/gml/3.1.0/polygon.xsd");

	}

}
