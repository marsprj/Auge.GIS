#include "GetFeatureResponse.h"
#include "GetFeatureRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	GetFeatureResponse::GetFeatureResponse(GetFeatureRequest* pRequest)
	{
		m_pCursor = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetFeatureResponse::~GetFeatureResponse()
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

	void GetFeatureResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE GetFeatureResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		XDocument* pxDoc = WriteDocument(); 

		const char* encoding = m_pRequest->GetEncoding();
		
		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, encoding ,1);
		pLogger->Info((char*)buffer, __FILE__, __LINE__);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();
		
		pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetFeatureResponse::SetFeatureCursor(FeatureCursor* pCursor)
	{
		m_pCursor = pCursor;
	}

	XDocument* GetFeatureResponse::WriteDocument()
	{
		const char* type_name = m_pRequest->GetTypeName(); 
		const char* service_name = m_pWebContext->GetService();
		const char* service_uri = m_pWebContext->GetURI();
		const char* output_format = m_pRequest->GetOutputFormat();
		char str[AUGE_BUFFER_MAX];

		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("FeatureCollection", "http://www.opengis.net/wfs", "wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");
		pxRoot->SetNamespaceDeclaration(service_uri, service_name);
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		// FeatureCollection-->boundedBy
		XElement* pxBoundedBy = pxRoot->AddChild("boundedBy","gml");
		pxNode = pxBoundedBy->AddChild("null", "gml");
		pxNode->SetAttribute("null", "unknown", NULL);

		XElement	*pxMember = NULL;
		XElement	*pxFeature= NULL;
		XElement	*pxValue  = NULL;

		Feature		*pFeature = NULL;
		if(m_pCursor!=NULL)
		{
			FeatureClass *pFeatureClass = m_pCursor->GetFeatureClass();
			GFields		*pFields = pFeatureClass->GetFields();
			GField		*pField  = NULL;
			g_uint		fcount = pFields->Count();
			const char	*fname = NULL;	

			while((pFeature=m_pCursor->NextFeature())!=NULL)
			{
				// FeatureCollection-->featureMember
				pxMember = pxRoot->AddChild("featureMember","gml"); 
				// FeatureCollection-->feature
				pxFeature = pxMember->AddChild(type_name,service_name);
				g_sprintf(str,"%s.%d", type_name, pFeature->GetFID());
				pxFeature->SetAttribute("fid",str, NULL, true);

				for(g_uint i=0; i<fcount; i++)
				{
					pField = pFields->GetField(i);
					fname = pField->GetName();
					// FeatureCollection-->feature->value
					pxValue = pxFeature->AddChild(pField->GetName(),service_name);

					switch(pField->GetType())
					{					 
					case augeFieldTypeShort:
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetShort(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeInt:
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeLong:
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%ld",pFeature->GetLong(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeInt64:
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%lld",pFeature->GetInt64(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeFloat:
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%f",pFeature->GetFloat(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeDouble:
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"%f",pFeature->GetDouble(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeChar:			 
						{
							g_snprintf(str, AUGE_BUFFER_MAX,"'%c'",pFeature->GetChar(i));
							pxValue->AddChildText(str);
						}
						break;
					case augeFieldTypeString:
						{
							const char* val = pFeature->GetString(i);
							pxValue->AddChildText(val,true);
						}
						break;
					case augeFieldTypeTime:	
						{
							TIME_STRU* pTimeStru = pFeature->GetTime(i);
							g_snprintf(str, AUGE_BUFFER_MAX,"%4d-%02d-%02d %02d:%02d:%02d",pTimeStru->usYear,pTimeStru->usMonth,pTimeStru->usDay,pTimeStru->usHour,pTimeStru->usMinute,pTimeStru->usSecond);
							pxValue->AddChildText(str,false);
							free(pTimeStru);
						}
						break;
					case augeFieldTypeBool:			 
						{

						}
						break;
					case augeFieldTypeBLOB:			 
						{

						}
						break;
					case augeFieldTypeGeometry:
						{
							GeometryDef* pGeometryDef = pField->GetGeometryDef();
							Geometry *pGeometry = pFeature->GetGeometry();
							if(pGeometry!=NULL)
							{
								if(!g_stricmp(output_format, AUGE_WFS_OUTPUT_FORMAT_WKT))
								{
									const char* wkt = pGeometry->AsText(true);
									if(wkt!=NULL)
									{
										g_snprintf(str, AUGE_BUFFER_MAX,"%d",pGeometryDef->GetSRID());	
										pxValue->AddChildText(wkt);
									}
								}
								else
								{
									//GML2
									GMLWriter* pGMLWriter = NULL;
									GeometryFactory* pFactory = augeGetGeometryFactoryInstance();

									pGMLWriter = pFactory->CreateGMLWriter();
									pGMLWriter->Write(pxValue, pGeometry, pGeometryDef->GetSRID(),"2");
									pGMLWriter->Release();
								}
							}
						}
						break;
					}//switch
				}//for
				pFeature->Release();
			}
		}
		return pxDoc; 
	}

	RESULTCODE GetFeatureResponse::Write()
	{		
		XDocument* pxDoc = WriteDocument();
		int len = 0; 
		g_uchar* buffer = NULL;

		pxDoc->WriteToString(&buffer, len,m_encoding.c_str(),1);
		pxDoc->Release();

		return AG_SUCCESS;
	}


}