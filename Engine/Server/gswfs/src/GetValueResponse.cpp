#include "GetValueResponse.h"
#include "GetValueRequest.h"
#include "AugeWebCore.h"
#include "AugeCore.h"

namespace auge
{
	GetValueResponse::GetValueResponse(GetValueRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetValueResponse::~GetValueResponse()
	{
		if(m_pValues!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pValues);
		}
	}

	RESULTCODE GetValueResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		const char* typeName = m_pRequest->GetTypeName();

		
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		pxRoot = pxDoc->CreateRootNode(m_pRequest->GetRequest(), NULL, NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs","wfs");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		if(m_pValues!=NULL)
		{
			char temp[AUGE_MSG_MAX];
			GValue* pValue = NULL;
			m_pValues->Reset();
			while((pValue=m_pValues->Next())!=NULL)
			{
				XElement* pxValue = pxRoot->AddChild("Value", "wfs");
				switch(pValue->GetType())
				{
				case augeValueTypeInt:
					{
						memset(temp, AUGE_MSG_MAX, 0);
						g_snprintf(temp, AUGE_MSG_MAX, "%d", pValue->GetInt());
						pxValue->AddChildText(temp);
					}
					break;
				case augeValueTypeDouble:
					{
						memset(temp, AUGE_MSG_MAX, 0);
						g_snprintf(temp, AUGE_MSG_MAX, "%f", pValue->GetDouble());
						pxValue->AddChildText(temp);
					}
					break;
				case augeValueTypeString:
					{
						pxValue->AddChildText(pValue->GetString());
					}
					break;
				}
			}
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len,m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetValueResponse::SetValues(EnumValue* pValues)
	{
		m_pValues = pValues;
	}
}