#include "GetAreaResponse.h"
#include "GetAreaRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "GProcessEngine.h"

namespace auge
{
	GetAreaResponse::GetAreaResponse(GetAreaRequest* pRequest)
	{
		m_pCursor = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetAreaResponse::~GetAreaResponse()
	{
		if(m_pCursor!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pCursor);
		}
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
	}

	void GetAreaResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	void GetAreaResponse::SetFeatureCursor(FeatureCursor* pCursor)
	{
		m_pCursor = pCursor;
	}


	RESULTCODE GetAreaResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = WriteDocument(); 

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len,m_encoding.c_str(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	XDocument* GetAreaResponse::WriteDocument()
	{
		const char* type_name = m_pRequest->GetInputTypeName(); 
		const char* service_name = m_pWebContext->GetService();
		const char* service_uri = m_pWebContext->GetURI();
		
		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		// GeometryCollection
		pxRoot = pxDoc->CreateRootNode("GeometryCollection", NULL,NULL);
		//pxRoot = pxDoc->CreateRootNode("GeometryCollection", "http://www.opengis.net/wfs", "wfs");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");
		pxRoot->SetNamespaceDeclaration(service_uri, service_name);
		pxRoot->SetAttribute("version", "1.1.0", NULL);
		
		if(m_pCursor!=NULL)
		{
			WriteGetArea(pxRoot);
		}
		return pxDoc; 
	}

	void GetAreaResponse::WriteGetArea(XElement* pxParent)
	{
		double area = 0.0f;
		char str[AUGE_BUFFER_MAX];
		g_uint srid = m_pCursor->GetFeatureClass()->GetSRID();
		const char* output_format = m_pRequest->GetOutputFormat();

		GeometryUtil* pGeometryUtil = augeGetGeometryUtilInstance();
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		XElement *pxArea = NULL;
		Feature	 *pFeature = NULL;
		while((pFeature=m_pCursor->NextFeature())!=NULL)
		{	
			pxArea = pxParent->AddChild("Area",NULL);
			g_sprintf(str,"%d", pFeature->GetFID());
			pxArea->SetAttribute("fid",str, NULL);

			Geometry *pGeometry = pFeature->GetGeometry();
			switch(pGeometry->GeometryType())
			{
			case augeGTPolygon:
				area = ((GPolygon*)pGeometry)->Area();
				break;
			case augeGTMultiPolygon:				
				area = ((GPolygon*)pGeometry)->Area();
				break;
			default:
				area = 0.0f;
			}
			g_sprintf(str, "%f", area);
			pxArea->SetChildText(str);
			pFeature->Release();
		}
	}

}