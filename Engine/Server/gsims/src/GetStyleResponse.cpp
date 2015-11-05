#include "GetStyleRequest.h"
#include "GetStyleResponse.h"
#include "AugeService.h"
#include "AugeCarto.h"
#include "AugeXML.h"

namespace auge
{
	GetStyleResponse::GetStyleResponse(GetStyleRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pStyles = NULL;
		m_style_text = NULL;
	}

	GetStyleResponse::~GetStyleResponse()
	{
		if(m_pStyles!=NULL)
		{
			m_pStyles->Release();
			m_pStyles = NULL;
		}
		if(m_style_text!=NULL)
		{
			auge_free(m_style_text);
			m_style_text = NULL;
		}
	}

	void GetStyleResponse::SetStyles(EnumStyle* pStyles)
	{
		if(m_pStyles!=NULL)
		{
			m_pStyles->Release();
			m_pStyles = NULL;
		}
		m_pStyles = pStyles;
	}

	void GetStyleResponse::SetStyleText(char* text)
	{
		if(m_style_text!=NULL)
		{
			auge_free(m_style_text);
			m_style_text = NULL;
		}
		m_style_text = text;
	}

	RESULTCODE GetStyleResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		pWriter->WriteHead(m_pRequest->GetMimeType());
		if(m_style_text!=NULL)
		{
			size_t text_len = strlen(m_style_text);
			size_t text_utf8_len = text_len << 1;
			char* text_utf8 = (char*)malloc(text_utf8_len);
			memset(text_utf8, 0, text_utf8_len);
			auge_encoding_convert_2(AUGE_ENCODING_GBK, AUGE_ENCODING_UTF8, m_style_text, text_len, text_utf8, &text_utf8_len);
			pWriter->Write((g_uchar*)text_utf8, strlen(text_utf8 ));
			free(text_utf8);
		}
		else
		{
			XElement	*pxNode = NULL;
			XElement	*pxStyle = NULL;
			XDocument	*pxDoc = new XDocument();
			XElement	*pxRoot = pxDoc->CreateRootNode("Styles", NULL, NULL);

			GeometryFactory* pGeometryFactory = NULL;
			pGeometryFactory = augeGetGeometryFactoryInstance();

			char str[AUGE_MSG_MAX];
			Style* pStyle = NULL;
			GLogger* pLogger = augeGetLoggerInstance();
			if(m_pStyles!=NULL)
			{
				m_pStyles->Reset();
				while((pStyle = m_pStyles->Next())!=NULL) 
				{
					pxStyle = pxRoot->AddChild("Style");
					g_sprintf(str, "%d", pStyle->GetID());
					pxStyle->SetAttribute("id",str, NULL);
					pxStyle->SetAttribute("name",pStyle->GetName(), NULL);
					switch(pStyle->GetType())
					{
					case augeStyleFeature:
						{
							FeatureStyle* pFStyle = static_cast<FeatureStyle*>(pStyle);
							pxStyle->SetAttribute("type",pGeometryFactory->Encode(pFStyle->GetGeometryType()), NULL);
						}
						break;
					}
					

				}
			}			

			int len = 0;
			g_uchar* buffer = NULL;
			pxDoc->WriteToString(&buffer, len, "GBK",0);

			//pWriter->WriteHead(m_pRequest->GetMimeType());
			pWriter->Write(buffer, len);
			//pWriter->WriteTail();

			pLogger->Info((g_char*)buffer);

			pxDoc->Close();
			AUGE_SAFE_RELEASE(pxDoc);
		}
		pWriter->WriteTail();
			
		return AG_SUCCESS;
	}

	/*RESULTCODE GetStyleResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = m_pRequest->GetName();
		if(name==NULL)
		{
			XElement	*pxNode = NULL;
			XElement	*pxStyle = NULL;
			XDocument	*pxDoc = new XDocument();
			XElement	*pxRoot = pxDoc->CreateRootNode("Styles", NULL, NULL);
			
			char str[AUGE_MSG_MAX];
			Style* pStyle = NULL;
			EnumStyle* pStyles = pCartoManager->GetStyles();
			pStyles->Reset(); 
			while((pStyle = pStyles->Next())!=NULL) 
			{
				pxStyle = pxRoot->AddChild("Style");
				g_sprintf(str, "%d", pStyle->GetID());
				pxStyle->SetAttribute("id",str, NULL);
				pxStyle->SetAttribute("name",pStyle->GetName(), NULL);
				pxStyle->SetAttribute("type","Point", NULL);
				
			}
			pStyles->Release();

			int len = 0;
			g_uchar* buffer = NULL;
			pxDoc->WriteToString(&buffer, len, "GBK",0);

			pWriter->WriteHead(m_pRequest->GetMimeType());
			pWriter->Write(buffer, len);
			pWriter->WriteTail();

			pLogger->Info((g_char*)buffer);

			pxDoc->Close();
			AUGE_SAFE_RELEASE(pxDoc);
		}
		else
		{
			pWriter->WriteHead(m_pRequest->GetMimeType());
			char* text = pCartoManager->GetStyleText(name);
			if(text!=NULL)
			{	
				pWriter->Write((g_uchar*)text, strlen(text));
				auge_free(text);
			}
			pWriter->WriteTail();
		}

		
		return AG_SUCCESS;
	}*/
}