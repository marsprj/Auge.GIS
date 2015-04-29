#include "TransactionRequest.h"
#include "AugeXML.h"

namespace auge
{
	TransactionRequest::TransactionRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
		m_pxDoc = NULL;
	}

	TransactionRequest::~TransactionRequest()
	{
		if(m_pxDoc!=NULL)
		{
			m_pxDoc->Close();
			m_pxDoc->Release();
			m_pxDoc = NULL;
		}
	}

	//const char*	TransactionRequest::GetEngine()
	//{
	//	return "wfs";
	//}

	//const char*	TransactionRequest::GetVersion()
	//{
	//	return m_version.c_str();
	//}

	const char* TransactionRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	TransactionRequest::GetRequest()
	{
		return "Transaction";
	}

	//void TransactionRequest::SetVersion(const char* value)
	//{
	//	if(value==NULL)
	//	{
	//		return;
	//	}
	//	m_version = value;
	//}

	void TransactionRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}

	//const char*	TransactionRequest::GetMimeType()
	//{
	//	return m_mime_type.c_str();
	//}

	XDocument* TransactionRequest::GetXmlDoc()
	{
		return m_pxDoc;
	}

	bool TransactionRequest::Create(XDocument* pxDoc)
	{
		m_pxDoc = pxDoc;
		if(m_pxDoc!=NULL)
		{
			m_pxDoc->AddRef();
		}
		
		XElement	*pxRoot = NULL;
		XAttribute	*pxAttr = NULL;

		pxRoot = pxDoc->GetRootNode();
		pxAttr = pxRoot->GetAttribute("version");
		if(pxAttr!=NULL)
		{
			SetVersion(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("mapName");
		if(pxAttr!=NULL)
		{
			SetMapName(pxAttr->GetValue());
		}

		pxAttr = pxRoot->GetAttribute("sourceName");
		if(pxAttr!=NULL)
		{
			SetSourceName(pxAttr->GetValue());
		}

		return true;
	}

}