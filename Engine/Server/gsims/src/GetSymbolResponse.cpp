#include "GetSymbolRequest.h"
#include "GetSymbolResponse.h"
#include "AugeService.h"
#include "AugeData.h"

#include "AugeSymbol.h"

namespace auge
{
	GetSymbolResponse::GetSymbolResponse(GetSymbolRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pSymbol = NULL;
		m_pSymbols= NULL;

		m_pWebContext = NULL;
	}

	GetSymbolResponse::~GetSymbolResponse()
	{
		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
		//if(m_pSymbols!=NULL)
		//{
		//	AUGE_SAFE_RELEASE(m_pSymbols);
		//}
	}

	RESULTCODE GetSymbolResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}
		
		XDocument* pxDoc = new XDocument();
		if(m_pSymbol!=NULL)
		{
			WriteSymbol(pxDoc);
		}
		else if(m_pSymbols!=NULL)
		{
			WriteSymbols(pxDoc);
		}

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),0);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);
		
		return AG_SUCCESS;
	}

	void GetSymbolResponse::SetSymbol(Symbol* pSymbol)
	{
		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
		m_pSymbol = pSymbol;
	}

	void GetSymbolResponse::SetSymbols(EnumSymbol* pSymbols)
	{
		if(m_pSymbols!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbols); 
		}
		m_pSymbols = pSymbols;
	}

	void GetSymbolResponse::WriteSymbols(XDocument* pxDoc)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XElement	*pxRoot = pxDoc->CreateRootNode("Symbols", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");

		Symbol		*pSymbol = NULL;
		m_pSymbols->Reset();
		while((pSymbol=m_pSymbols->Next())!=NULL)
		{
			AddSymbolNode(pxRoot, pSymbol);
		}
	}

	void GetSymbolResponse::WriteSymbol(XDocument* pxDoc)
	{
		XElement  *pxNode = NULL;
		XElement  *pxRoot = NULL;
		char str[AUGE_MSG_MAX];

		pxRoot = pxDoc->CreateRootNode("Symbols", NULL, NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.opengis.net/wms",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");
		//pxRoot->SetAttribute("version", "1.0.0", NULL);

		Symbol* pSymbol = m_pSymbol;
		AddSymbolNode(pxRoot, pSymbol);
	}

	void GetSymbolResponse::AddSymbolNode(XElement* pxParent, Symbol* pSymbol)
	{
		XElement *pxSymbol  = pxParent->AddChild("Symbol", NULL);
		XElement *pxName = pxSymbol->AddChild("Name");
		pxName->AddChildText(pSymbol->GetName());
		
		const char* icon_name = pSymbol->GetIcon();
		//icon_name = pSymbol->GetIcon();
		char icon_xlink[AUGE_PATH_MAX];
		memset(icon_xlink, 0, AUGE_PATH_MAX);
		g_sprintf(icon_xlink,"http://%s:%s/%s/symbol/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), AUGE_VIRTUAL_NAME, icon_name);
		XElement *pxIcon = pxSymbol->AddChild("Icon", NULL);
		pxIcon->SetAttribute("xlink:href",icon_xlink, NULL);
	}

	void GetSymbolResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}