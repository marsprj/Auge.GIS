#include "ConvexHullResponse.h"
#include "ConvexHullRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeFeature.h"
#include "AugeGeometry.h"
#include "GProcessEngine.h"

#include "geos_c.h"

namespace auge
{
#define AUGE_GPS_OUTPUT_FORMAT_WKT		"WKT"
#define AUGE_GPS_OUTPUT_FORMAT_GML2		"GML2"

	ConvexHullResponse::ConvexHullResponse(ConvexHullRequest* pRequest)
	{
		m_pCursor = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	ConvexHullResponse::~ConvexHullResponse()
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

	void ConvexHullResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	void ConvexHullResponse::SetFeatureCursor(FeatureCursor* pCursor)
	{
		m_pCursor = pCursor;
	}


	RESULTCODE ConvexHullResponse::Write(WebWriter* pWriter)
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

	XDocument* ConvexHullResponse::WriteDocument()
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

		// GeometryCollection-->boundedBy
		XElement* pxBoundedBy = pxRoot->AddChild("boundedBy","gml");
		pxNode = pxBoundedBy->AddChild("null", "gml");
		pxNode->SetAttribute("null", "unknown", NULL);

		if(m_pCursor!=NULL)
		{
			initGEOS(NULL, NULL);
			WriteConvexHull(pxRoot);
			finishGEOS();
		}
		return pxDoc; 
	}

	void ConvexHullResponse::WriteConvexHull(XElement* pxParent)
	{
		char str[AUGE_BUFFER_MAX];
		g_uint srid = m_pCursor->GetFeatureClass()->GetSRID();
		const char* output_format = m_pRequest->GetOutputFormat();

		GeometryUtil* pGeometryUtil = augeGetGeometryUtilInstance();
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();

		XElement *pxConvexHull = NULL;
		Feature	 *pFeature = NULL;
		while((pFeature=m_pCursor->NextFeature())!=NULL)
		{	
			Geometry *pGeometry = pFeature->GetGeometry();
			if(pGeometry!=NULL)
			{
				pxConvexHull = pxParent->AddChild("ConvexHull",NULL);
				g_sprintf(str,"%d", pFeature->GetFID());
				pxConvexHull->SetAttribute("fid",str, NULL);

				g_uchar* wkb = pGeometry->AsBinary();
				g_uint   wkb_size = pGeometryUtil->GetWKBSize(wkb);

				GEOSGeometry* pgeosGeomtrySrc = GEOSGeomFromWKB_buf(wkb, wkb_size);
				if(pgeosGeomtrySrc!=NULL)
				{
					GEOSGeometry* pgeosConvexHull = GEOSConvexHull(pgeosGeomtrySrc);
					if(pgeosConvexHull!=NULL)
					{	
						size_t wkbSize = 0;
						unsigned char* pwkbConvexHull = NULL;
						pwkbConvexHull = GEOSGeomToWKB_buf(pgeosConvexHull, &wkbSize);
						if(pwkbConvexHull!=NULL||wkbSize>0)
						{

							Geometry* pConvexHull = NULL;
							pConvexHull = pGeometryFactory->CreateGeometryFromWKB(pwkbConvexHull, true);

							if(pConvexHull!=NULL)
							{
								if(!g_stricmp(output_format, AUGE_GPS_OUTPUT_FORMAT_WKT))
								{
									const char* wkt = pGeometry->AsText(true);
									if(wkt!=NULL)
									{
										//g_snprintf(str, AUGE_BUFFER_MAX,"%d",pGeometryDef->GetSRID());	
										pxConvexHull->AddChildText(wkt);
									}
								}
								else
								{
									//GML2
									GMLWriter* pGMLWriter = NULL;
									GeometryFactory* pFactory = augeGetGeometryFactoryInstance();

									pGMLWriter = pFactory->CreateGMLWriter();
									pGMLWriter->Write(pxConvexHull, pConvexHull, srid,"2");
									pGMLWriter->Release();
								}
								pConvexHull->Release();
							}

							////////////////////////////////////////////////////////////////
							//这里有内存泄露，需要名曲pwkbConvexHull由谁释放
							//free(pwkbConvexHull);
							////////////////////////////////////////////////////////////////

						}
						GEOSGeom_destroy(pgeosConvexHull);
					}
					GEOSGeom_destroy(pgeosGeomtrySrc);
				}
			}
			pFeature->Release();
		}
	}

}