#include "GetColorMapResponse.h"
#include "GetColorMapRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "WEngine.h"
#include "AugeCarto.h"

namespace auge
{
	GetColorMapResponse::GetColorMapResponse(GetColorMapRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pColorMaps = NULL;
		m_pColorMap = NULL;
		m_pWebContext = NULL;
	}

	GetColorMapResponse::~GetColorMapResponse()
	{
		if(m_pColorMaps!=NULL)
		{
			m_pColorMaps->Release();
			m_pColorMaps = NULL;
		}
		if(m_pColorMap!=NULL)
		{
			m_pColorMap->Release();
			m_pColorMap = NULL;
		}
	}

	RESULTCODE GetColorMapResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		RESULTCODE rc = AG_FAILURE;
		if(m_pColorMap)
		{
			rc = WriteColorMap(pWriter);
		}
		else
		{
			rc = WriteColorMaps(pWriter);
		}

		//XDocument	*pxDoc = new XDocument();
		//XElement	*pxRoot = pxDoc->CreateRootNode("ColorMaps", NULL, NULL);
		//pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");

		//if(m_pColorMaps!=NULL)
		//{
		//	char str[AUGE_NAME_MAX]; 
		//	ColorMap* pColorMap = NULL; 
		//	m_pColorMaps->Reset();
		//	while((pColorMap=m_pColorMaps->Next())!=NULL)
		//	{
		//		sprintf(str,"%d",pColorMap->GetID());
		//		XElement* pxColorMap = pxRoot->AddChild("ColorMap",NULL);				
		//		pxColorMap->SetAttribute("id",str,NULL);

		//		XElement* pxColorWrapper = pxColorMap->AddChild("Color");
		//		XElement* pxColor = pxColorWrapper->AddChild("Start");
		//		GColor color = pColorMap->GetStartColor(); 
		//		const char* format = "#%02x%02x%02x%02x";
		//		g_sprintf(str,format, color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
		//		pxColor->AddChildText(str);

		//		color = pColorMap->GetEndColor();
		//		g_sprintf(str,format, color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
		//		pxColor = pxColorWrapper->AddChild("End");
		//		pxColor->AddChildText(str);
		//		
		//		XElement *pxOnlineResource = pxColorMap->AddChild("OnlineResource", NULL);
		//		pxOnlineResource->SetAttribute("xlink:href",pColorMap->GetImagePath(), NULL);
		//	}
		//}

		//int len = 0;
		//g_uchar* buffer = NULL;
		//GLogger* pLogger = augeGetLoggerInstance();

		//pxDoc->WriteToString(&buffer, len, "GBK",0);

		//pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		//pWriter->Write(buffer, len);
		//pWriter->WriteTail();

		return rc;
	}

	void GetColorMapResponse::SetColorMaps(EnumColorMap* pColorMaps)
	{		
		if(m_pColorMap!=NULL)
		{
			m_pColorMap->Release();
			m_pColorMap = NULL;
		}
		m_pColorMaps = pColorMaps;
	}

	void GetColorMapResponse::SetColorMap(ColorMap* pColorMap)
	{
		if(m_pColorMaps!=NULL)
		{
			m_pColorMaps->Release();
			m_pColorMaps = NULL;
		}
		m_pColorMap = pColorMap;
	}

	RESULTCODE GetColorMapResponse::WriteColorMaps(WebWriter* pWriter)
	{
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("ColorMaps", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");

		if(m_pColorMaps!=NULL)
		{
			char str[AUGE_NAME_MAX]; 
			ColorMap* pColorMap = NULL; 
			m_pColorMaps->Reset();
			while((pColorMap=m_pColorMaps->Next())!=NULL)
			{
				sprintf(str,"%d",pColorMap->GetID());
				XElement* pxColorMap = pxRoot->AddChild("ColorMap",NULL);				
				pxColorMap->SetAttribute("id",str,NULL); 

				XElement* pxColorWrapper = pxColorMap->AddChild("Color");
				XElement* pxColor = pxColorWrapper->AddChild("Start");
				GColor color = pColorMap->GetStartColor(); 
				//const char* format = "#%02x%02x%02x%02x";
				//g_sprintf(str,format, color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
				const char* format = "#%02x%02x%02x%";
				g_sprintf(str,format, color.GetRed(),color.GetGreen(),color.GetBlue());
				pxColor->AddChildText(str);

				color = pColorMap->GetEndColor();
				g_sprintf(str,format, color.GetRed(),color.GetGreen(),color.GetBlue(),color.GetAlpha());
				pxColor = pxColorWrapper->AddChild("End");
				pxColor->AddChildText(str);

				g_sprintf(str,"http://%s:%s/colormap/%s", m_pWebContext->GetServer(), m_pWebContext->GetPort(), pColorMap->GetImagePath());
				XElement *pxOnlineResource = pxColorMap->AddChild("OnlineResource", NULL);
				//pxOnlineResource->SetAttribute("xlink:href",pColorMap->GetImagePath(), NULL);
				pxOnlineResource->SetAttribute("xlink:href",str, NULL);
			}
		}

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		return AG_SUCCESS;
	}

	RESULTCODE GetColorMapResponse::WriteColorMap(WebWriter* pWriter)
	{
		g_uint count = m_pRequest->GetCount();
		m_pColorMap->SetCount(count);

		char str[AUGE_NAME_MAX];

		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("ColorMap", NULL, NULL);
		pxRoot->SetNamespaceDeclaration("http://www.w3.org/1999/xlink","xlink");

		sprintf(str,"%d", count);
		pxRoot->SetAttribute("count",str, NULL);
		
		for(g_uint i=0; i<count; i++)
		{
			GColor* c = m_pColorMap->GetColor(i);
			//sprintf(str, "#%02x%02x%02x%02x",c->GetRed(),c->GetGreen(),c->GetBlue(), c->GetAlpha());
			sprintf(str, "#%02x%02x%02x",c->GetRed(),c->GetGreen(),c->GetBlue());

			XElement* pxNode = pxRoot->AddChild("Color");
			pxNode->AddChildText(str);
		}

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		return AG_SUCCESS;
	}

	void GetColorMapResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}
}