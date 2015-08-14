#include "GetSymbolIconRequest.h"
#include "GetSymbolIconResponse.h"
#include "AugeService.h"
#include "AugeData.h"

#include "AugeSymbol.h"

namespace auge
{
	GetSymbolIconResponse::GetSymbolIconResponse(GetSymbolIconRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pSymbol = NULL;

		m_pWebContext = NULL;
	}

	GetSymbolIconResponse::~GetSymbolIconResponse()
	{
		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
	}

	RESULTCODE GetSymbolIconResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}
		
		const char* icon_name = m_pSymbol->GetIcon();
		const char* base_path = augeGetSymbolManagerInstance()->GetPath();
		char icon_path[AUGE_PATH_MAX];
		memset(icon_path, 0, AUGE_PATH_MAX);
		auge_make_path(icon_path, NULL, base_path, icon_name, NULL);

		FILE* fp = fopen(icon_path, "rb");
		if(fp==NULL)
		{
			return AG_FAILURE;
		}

		pWriter->WriteHead("image/png");

		g_uint nBytes = 0;
		g_uchar buffer[AUGE_BUFFER_MAX];		
		memset(buffer, 0, AUGE_BUFFER_MAX);
		while((nBytes=fread(buffer, sizeof(g_uchar),AUGE_BUFFER_MAX, fp))>0)
		{
			pWriter->Write(buffer, nBytes);
		}
		pWriter->WriteTail();

		fclose(fp);		
		return AG_SUCCESS;
	}

	void GetSymbolIconResponse::SetSymbol(Symbol* pSymbol)
	{
		if(m_pSymbol!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pSymbol);
		}
		m_pSymbol = pSymbol;
	}
	
	void GetSymbolIconResponse::WriteSymbol(XDocument* pxDoc)
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

	void GetSymbolIconResponse::AddSymbolNode(XElement* pxParent, Symbol* pSymbol)
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

	void GetSymbolIconResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}