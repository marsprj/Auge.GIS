#include "GetMinMaxValueResponse.h"
#include "GetValueRequest.h"
#include "AugeWebCore.h"
#include "AugeCore.h"

namespace auge
{
	GetMinMaxValueResponse::GetMinMaxValueResponse(GetValueRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetMinMaxValueResponse::~GetMinMaxValueResponse()
	{
		if(m_pValues!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pValues);
		}
	}

	RESULTCODE GetMinMaxValueResponse::Write(WebWriter* pWriter)
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

		m_pValues->Reset();
		GValue* pminValue = m_pValues->Next();
		GValue* pmaxValue = m_pValues->Next();

		XElement* pxValue = pxRoot->AddChild("Min", "wfs");
		AddValue(pxValue, pminValue);
		pxValue = pxRoot->AddChild("Max", "wfs");
		AddValue(pxValue, pmaxValue);

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len,m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetMinMaxValueResponse::SetValues(EnumValue* pValues)
	{
		m_pValues = pValues;
	}

	void GetMinMaxValueResponse::AddValue(XElement* pxValue, GValue* pValue)
	{
		if(pValue==NULL)
		{
			return;
		}

		char temp[AUGE_MSG_MAX];
		//XElement* pxValue = pxRoot->AddChild("Value", "wfs");
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
				const char* value = pValue->GetString();
				pxValue->AddChildText(value,true);
			}
			break;
		case augeValueTypeTime:
			{
				TIME_STRU* tim = pValue->GetTime();
				if(tim!=NULL)
				{
					char str[AUGE_NAME_MAX];
					memset(str, 0, AUGE_NAME_MAX);
					g_snprintf(str,AUGE_NAME_MAX,"%d-%2d-%2d %2d:%2d:%2d",tim->usYear,tim->usMonth,tim->usDay,tim->usHour,tim->usMinute,tim->usSecond);
					pxValue->AddChildText(str);
				}
			}
			break;
		}
	}
}