#include "GetSubscriptionThemeRequest.h"
#include "GetSubscriptionThemeResponse.h"
#include "AugeWebCore.h"

namespace auge
{
	const char* g_themes[] = {"poi","aqi"};

	GetSubscriptionThemeResponse::GetSubscriptionThemeResponse(GetSubscriptionThemeRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pWebContext = NULL;
	}

	GetSubscriptionThemeResponse::~GetSubscriptionThemeResponse()
	{

	}

	RESULTCODE GetSubscriptionThemeResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Themes", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");

		XElement	*pxTheme = NULL;		
		XElement	*pxKeyword = NULL;
		SubscriptionTheme* pTheme = NULL;

		g_uint count = sizeof(g_themes) / sizeof(const char*);
		for(g_uint i=0; i<count; i++)
		{ 
			const char* theme = g_themes[i];
			pxTheme = pxRoot->AddChild("Theme");		
			pxTheme->SetChildText(theme, true);
		}

		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();

		pxDoc->Release();

		return AG_SUCCESS;
	}

	void GetSubscriptionThemeResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}
