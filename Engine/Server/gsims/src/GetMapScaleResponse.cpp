#include "GetMapScaleResponse.h"
#include "GetMapScaleRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "WEngine.h"
#include "AugeCarto.h"

namespace auge
{
	GetMapScaleResponse::GetMapScaleResponse(GetMapScaleRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();		
		m_pWebContext = NULL;
		m_map_scale = 1.0f;
	}

	GetMapScaleResponse::~GetMapScaleResponse()
	{
		
	}

	RESULTCODE GetMapScaleResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		char str[AUGE_NAME_MAX];
		g_snprintf(str, AUGE_NAME_MAX, "%f", m_map_scale);

		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("MapScale", NULL, NULL); 
		pxRoot->SetChildText(str);

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return AG_SUCCESS;
	}

	void GetMapScaleResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	void GetMapScaleResponse::SetMapScale(float mapScale)
	{
		m_map_scale = mapScale;
	}
}