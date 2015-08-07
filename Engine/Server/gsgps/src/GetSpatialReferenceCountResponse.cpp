#include "GetSpatialReferenceCountResponse.h"
#include "GetSpatialReferenceCountRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	GetSpatialReferenceCountResponse::GetSpatialReferenceCountResponse(GetSpatialReferenceCountRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetSpatialReferenceCountResponse::~GetSpatialReferenceCountResponse()
	{
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
	}

	void GetSpatialReferenceCountResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE GetSpatialReferenceCountResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		const char* encoding = m_pRequest->GetEncoding();

		XDocument* pxDoc = WriteDocument(); 
		
		int len = 0; 
		g_uchar* buffer = NULL; 
		pxDoc->WriteToString(&buffer, len, encoding ,1);
		pLogger->Info((char*)buffer, __FILE__, __LINE__);

		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		//pWriter->Write((g_uchar*)"<a>ddd</a>", 10);
		pWriter->WriteTail();
		
		pxDoc->Release();

		return AG_SUCCESS;
	}

	XDocument* GetSpatialReferenceCountResponse::WriteDocument()
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxRoot = NULL;
		XElement  *pxNode = NULL;
		XElement  *pxSRS  = NULL;
		XDocument *pxDoc = new XDocument();
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("Count", NULL, NULL);
		pxRoot->AddChildText("3902");

		return pxDoc; 
	}

	RESULTCODE GetSpatialReferenceCountResponse::Write()
	{		
		XDocument* pxDoc = WriteDocument();
		int len = 0; 
		g_uchar* buffer = NULL;

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
		pxDoc->Release();

		return AG_SUCCESS;
	}


}