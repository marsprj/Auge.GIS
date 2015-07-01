#include "GetCountResponse.h"
#include "GetCountRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	GetCountResponse::GetCountResponse(GetCountRequest* pRequest)
	{
		m_count = 0;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetCountResponse::~GetCountResponse()
	{
		
	}

	void GetCountResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE GetCountResponse::Write(WebWriter* pWriter)
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

	void GetCountResponse::SetCount(g_uint count)
	{
		m_count = count;
	}

	XDocument* GetCountResponse::WriteDocument()
	{
		const char* type_name = m_pRequest->GetTypeName(); 
		char str[AUGE_BUFFER_MAX];
		g_sprintf(str, "%d",m_count);

		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();

		pxRoot = pxDoc->CreateRootNode("Count", NULL, NULL);
		pxRoot->SetAttribute("typeName", type_name, NULL);
		pxRoot->AddChildText(str);

		return pxDoc; 
	}

	RESULTCODE GetCountResponse::Write()
	{		
		XDocument* pxDoc = WriteDocument();
		int len = 0; 
		g_uchar* buffer = NULL;

		pxDoc->WriteToString(&buffer, len,m_encoding.c_str(),1);
		pxDoc->Release();

		return AG_SUCCESS;
	}


}