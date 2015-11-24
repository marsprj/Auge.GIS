#include "GetSubscriptionRequest.h"
#include "GetSubscriptionResponse.h"
#include "AugeWebCore.h"

namespace auge
{
	GetSubscriptionResponse::GetSubscriptionResponse(GetSubscriptionRequest* pRequest)
	{
		m_pTheme = NULL;
		m_pThemes = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();

		m_pWebContext = NULL;
	}

	GetSubscriptionResponse::~GetSubscriptionResponse()
	{
		if(m_pThemes!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pThemes);
		}
		if(m_pTheme!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTheme);
		}
	}

	RESULTCODE GetSubscriptionResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		if(m_pTheme!=NULL)
		{
			Write(pWriter, m_pTheme);
		}
		else
		{
			Write(pWriter, m_pThemes);
		}
	}

	RESULTCODE GetSubscriptionResponse::Write(WebWriter* pWriter, EnumSubscriptionTheme* pThemes)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Themes", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/2001/XMLSchema-instance","xsi");

		XElement	*pxTheme = NULL;		
		XElement	*pxKeyword = NULL;
		SubscriptionTheme* pTheme = NULL;

		pThemes->Reset();
		while((pTheme=pThemes->Next())!=NULL)
		{ 
			const char* name = pTheme->GetTheme();
			pxTheme = pxRoot->AddChild("Theme");		
			pxTheme->SetAttribute("name", name, NULL, true);

			const char* word = NULL;
			g_uint keywords = pTheme->GetKeywordCount();
			for(g_uint i=0; i<keywords; i++)
			{
				word = pTheme->GetKeyword(i);
				pxKeyword = pxTheme->AddChild("Keyword");
				pxKeyword->AddChildText(word);
			}
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

	
	RESULTCODE GetSubscriptionResponse::Write(WebWriter* pWriter, SubscriptionTheme* pTheme)
	{
		GLogger* pLogger = augeGetLoggerInstance(); 

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxTheme = pxDoc->CreateRootNode("Theme", NULL, NULL);
		XElement	*pxKeyword = NULL;
				
		const char* name = pTheme->GetTheme();
		pxTheme->SetAttribute("name", name, NULL, true);

		const char* word = NULL;
		g_uint keywords = pTheme->GetKeywordCount();
		for(g_uint i=0; i<keywords; i++)
		{
			word = pTheme->GetKeyword(i);
			pxKeyword = pxTheme->AddChild("Keyword");
			pxKeyword->AddChildText(word);
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

	void GetSubscriptionResponse::SetThemes(EnumSubscriptionTheme* pThemes)
	{
		if(m_pThemes!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pThemes);
		}
		if(m_pTheme!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTheme);
		}
		m_pThemes = pThemes;
	}

	void GetSubscriptionResponse::SetTheme(SubscriptionTheme* pTheme)
	{
		if(m_pTheme!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pTheme);
		}
		if(m_pThemes!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pThemes);
		}
		m_pTheme = pTheme;
	}

	void GetSubscriptionResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}
