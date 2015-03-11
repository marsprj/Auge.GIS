#include "TransactionResponse.h"
#include "TransactionRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	TransactionResponse::TransactionResponse(TransactionRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_encoding = "GBK";
		m_num_insert = 0;
		m_num_update = 0;
		m_num_delete = 0;
	}

	TransactionResponse::~TransactionResponse()
	{
	}

	RESULTCODE TransactionResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		const char* typeName = m_pRequest->GetTypeName();
		char str[AUGE_BUFFER_MAX];
		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		XDocument *pxDoc = new XDocument();
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("TransactionResponse", "http://www.opengis.net/wfs", "wfs");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wfs",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ows","ows");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema","xsd");		
		pxRoot->SetAttribute("version", m_pRequest->GetVersion(), NULL);

		XElement* pxSummary = pxRoot->AddChild("TransactionSummary","wfs");

		char text[AUGE_BUFFER_MAX];
		g_sprintf(text,"%d", m_num_insert);
		pxNode = pxSummary->AddChild("totalInserted","wfs");
		pxNode->AddChildText(text);

		g_sprintf(text,"%d", m_num_update);
		pxNode = pxSummary->AddChild("totalUpdated","wfs");
		pxNode->AddChildText(text);

		g_sprintf(text,"%d", m_num_delete);
		pxNode = pxSummary->AddChild("totalDeleted","wfs");
		pxNode->AddChildText(text);

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len,m_encoding.c_str(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();
		pxDoc->Release();

		return AG_SUCCESS;
	}

	void TransactionResponse::SetInsertCount(g_uint count)
	{
		m_num_insert = count;
	}

	void TransactionResponse::SetUpdateCount(g_uint count)
	{
		m_num_update = count;
	}

	void TransactionResponse::SetDeleteCount(g_uint count)
	{
		m_num_delete = count;
	}
}