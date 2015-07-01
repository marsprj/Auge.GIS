#include "GetGmlObjectResponse.h"
#include "GetGmlObjectRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	GetGmlObjectResponse::GetGmlObjectResponse(GetGmlObjectRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetGmlObjectResponse::~GetGmlObjectResponse()
	{
		if(m_pCursor!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pCursor);
		}
	}

	RESULTCODE GetGmlObjectResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		//const char* typeName = m_pRequest->GetTypeName();
		//char str[AUGE_BUFFER_MAX];
		//GLogger* pLogger = augeGetLoggerInstance();

		//XElement  *pxNode = NULL;
		//XElement  *pxRoot = NULL;
		//XDocument *pxDoc = new XDocument();
		//// FeatureCollection
		//pxRoot = pxDoc->CreateRootNode("FeatureCollection", "http://www.opengis.net/wfs", "wfs");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		//pxRoot->SetAttribute("version", "1.1.0", NULL);

		//// FeatureCollection-->boundedBy
		//XElement* pxBoundedBy = pxRoot->AddChild("boundedBy","gml");
		//pxNode = pxBoundedBy->AddChild("null", "gml");
		//pxNode->SetAttribute("null", "unknown", NULL);

		//XElement	*pxMember = NULL;
		//XElement	*pxFeature= NULL;
		//XElement	*pxValue  = NULL;

		//Feature		*pFeature = NULL;
		//FeatureClass *pFeatureClass = m_pCursor->GetGmlObjectClass();
		//GFields		*pFields = pFeatureClass->GetFields();
		//GField		*pField  = NULL;
		//g_uint		fcount = pFields->Count();

		//while((pFeature=m_pCursor->NextFeature())!=NULL)
		//{
		//	// FeatureCollection-->featureMember
		//	pxMember = pxRoot->AddChild("featureMember","gml");
		//	// FeatureCollection-->feature
		//	pxFeature = pxMember->AddChild(typeName,"gml");
		//	pxFeature->AddChild(typeName,NULL);
		//	g_sprintf(str,"%s.%d", typeName, pFeature->GetFID());
		//	pxFeature->SetAttribute("fid",str, NULL);
		//	
		//	for(g_uint i=0; i<fcount; i++)
		//	{
		//		pField = pFields->GetField(i);
		//		// FeatureCollection-->feature->value
		//		pxValue = pxFeature->AddChild(pField->GetName(),NULL);

		//		switch(pField->GetType())
		//		{					 
		//		case augeFieldTypeShort:
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetShort(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeInt:
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeLong:
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetLong(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeInt64:
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetInt64(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeFloat:
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"%f",pFeature->GetFloat(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeDouble:
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"%d",pFeature->GetDouble(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeChar:			 
		//			{
		//				g_snprintf(str, AUGE_BUFFER_MAX,"'%c'",pFeature->GetChar(i));
		//				pxValue->AddChildText(str);
		//			}
		//			break;
		//		case augeFieldTypeString:
		//			{
		//				pxValue->AddChildText(pFeature->GetString(i));
		//			}
		//			break;
		//		case augeFieldTypeTime:	
		//			{

		//			}
		//			break;
		//		case augeFieldTypeBool:			 
		//			{

		//			}
		//			break;
		//		case augeFieldTypeBLOB:			 
		//			{

		//			}
		//			break;
		//		case augeFieldTypeGeometry:
		//			{
		//				GeometryDef* pGeometryDef = pField->GetGeometryDef();
		//				Geometry *pGeometry = pFeature->GetGeometry();
		//				if(pGeometry!=NULL)
		//				{
		//					const char* wkt = pGeometry->AsText(true);
		//					if(wkt!=NULL)
		//					{
		//						g_snprintf(str, AUGE_BUFFER_MAX,"%d",pGeometryDef->GetSRID());	
		//						pxValue->AddChildText(wkt);
		//					}
		//				}
		//			}
		//			break;
		//		}//switch
		//	}//for
		//}

		//int len = 0;
		//g_uchar* buffer = NULL;
		//pxDoc->WriteToString(&buffer, len,m_encoding.c_str(),1);

		//pWriter->WriteHead(m_pRequest->GetMimeType());
		//pWriter->Write(buffer, len);
		//pWriter->WriteTail();
		//pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetGmlObjectResponse::SetFeatureCursor(FeatureCursor* pCursor)
	{
		m_pCursor = pCursor;
	}

}