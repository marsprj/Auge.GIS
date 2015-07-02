#include "GetValueRequest.h"
#include "GetValueResponse.h"
#include "AugeService.h"

#include <math.h>
#include <time.h>

namespace auge
{
	GetValueResponse::GetValueResponse(GetValueRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_values = NULL;
		m_bands  = 0;
	}

	GetValueResponse::~GetValueResponse()
	{
		if(m_pRequest!=NULL)
		{
			m_pRequest->Release();
		}
		if(m_values!=NULL)
		{
			free(m_values);
			m_values = NULL;
		}
	}

	RESULTCODE GetValueResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();

		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Raster",NULL,NULL);
		XElement	*pxNode = NULL;
		
		char str[AUGE_NAME_MAX];
		for(g_int i=0; i<m_bands; i++)
		{
			g_sprintf(str,"%d",m_values[i].band);
			pxNode = pxRoot->AddChild("Value", NULL);
			pxNode->AddChildText(m_values[i].value);
			pxNode->SetAttribute("band", str,NULL);
		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);

		return AG_SUCCESS;
	}

	void GetValueResponse::Create(int bands)
	{
		if(m_values!=NULL)
		{
			free(m_values);
			m_values = NULL;
		}
		if(bands>0)
		{
			m_bands = bands;
			m_values = (pixel_value_t *)malloc(sizeof(pixel_value_t)*m_bands);
			memset(m_values, 0, sizeof(pixel_value_t)*m_bands);
		}
	}

	void GetValueResponse::SetValue(int index, short band, const char* value)
	{
		m_values[index].band = band;
		strcpy(m_values[index].value,value);
	}

}