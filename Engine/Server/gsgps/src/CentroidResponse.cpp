#include "CentroidResponse.h"
#include "CentroidRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "GProcessEngine.h"

namespace auge
{
	#define AUGE_GPS_OUTPUT_FORMAT_WKT		"WKT"
	#define AUGE_GPS_OUTPUT_FORMAT_GML2		"GML2"

	CentroidResponse::CentroidResponse(CentroidRequest* pRequest)
	{
		m_pCursor = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	CentroidResponse::~CentroidResponse()
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

	void CentroidResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	void CentroidResponse::SetFeatureCursor(FeatureCursor* pCursor)
	{
		m_pCursor = pCursor;
	}


	RESULTCODE CentroidResponse::Write(WebWriter* pWriter)
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
	}

	XDocument* CentroidResponse::WriteDocument()
	{
		const char* type_name = m_pRequest->GetInputTypeName(); 


		const char* service_name = m_pWebContext->GetService();
		const char* service_uri = m_pWebContext->GetURI();
		const char* output_format = m_pRequest->GetOutputFormat();
		char str[AUGE_BUFFER_MAX];

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

		// GeometryCollection-->boundedBy
		//XElement* pxBoundedBy = pxRoot->AddChild("boundedBy","gml");
		//pxNode = pxBoundedBy->AddChild("null", "gml");
		//pxNode->SetAttribute("null", "unknown", NULL);

		
		if(m_pCursor!=NULL)
		{
			double x, y;
			const char* format = "POINT(%f %f)";

			XElement *pxCentorid = NULL;
			Feature	 *pFeature = NULL;
			while((pFeature=m_pCursor->NextFeature())!=NULL)
			{	
				Geometry *pGeometry = pFeature->GetGeometry();
				if(pGeometry!=NULL)
				{
					pxCentorid = pxRoot->AddChild("Centroid",NULL);
					//g_sprintf(str,"%s.%d", type_name, pFeature->GetFID());
					g_sprintf(str,"%d", pFeature->GetFID());
					pxCentorid->SetAttribute("fid",str, NULL);

					pGeometry->Centroid(x,y);
					memset(str, 0, AUGE_BUFFER_MAX);
					g_snprintf(str, AUGE_BUFFER_MAX, format, x, y);
					pxCentorid->AddChildText(str);

					//if(!g_stricmp(output_format, AUGE_GPS_OUTPUT_FORMAT_WKT))
					//{
					//	const char* wkt = pGeometry->AsText(true);
					//	if(wkt!=NULL)
					//	{
					//		//g_snprintf(str, AUGE_BUFFER_MAX,"%d",pGeometryDef->GetSRID());	
					//		pxValue->AddChildText(wkt);
					//	}
					//}
					//else
					//{
					//	//GML2
					//	GMLWriter* pGMLWriter = NULL;
					//	GeometryFactory* pFactory = augeGetGeometryFactoryInstance();

					//	pGMLWriter = pFactory->CreateGMLWriter();
					//	pGMLWriter->Write(pxValue, pGeometry, 4326,"2");
					//	pGMLWriter->Release();
					//}
				}
				pFeature->Release();
			}
		}
		return pxDoc; 
	}

}