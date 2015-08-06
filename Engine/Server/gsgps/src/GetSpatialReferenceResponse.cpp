#include "GetSpatialReferenceResponse.h"
#include "GetSpatialReferenceRequest.h"
#include "AugeWebCore.h"
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeXML.h"

namespace auge
{
	GetSpatialReferenceResponse::GetSpatialReferenceResponse(GetSpatialReferenceRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pstmt = NULL;
	}

	GetSpatialReferenceResponse::~GetSpatialReferenceResponse()
	{
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
		if(m_pstmt!=NULL)
		{
			sqlite3_finalize(m_pstmt);
			m_pstmt = NULL;
		}
	}

	void GetSpatialReferenceResponse::SetWebContenxt(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	RESULTCODE GetSpatialReferenceResponse::Write(WebWriter* pWriter)
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

	void GetSpatialReferenceResponse::SetSqlite(sqlite3_stmt* pstmt)
	{
		m_pstmt = pstmt;
	}

	XDocument* GetSpatialReferenceResponse::WriteDocument()
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxRoot = NULL;
		XElement  *pxNode = NULL;
		XElement  *pxSRS  = NULL;
		XDocument *pxDoc = new XDocument();
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("SpatialBase", NULL, NULL);
		pxRoot->SetAttribute("version", "1.1.0", NULL);

		while(sqlite3_step(m_pstmt)==SQLITE_ROW)
		{
			pxSRS = pxRoot->AddChild("SpatialReference", NULL); 
			const char* srid = (const char*)sqlite3_column_text(m_pstmt, 0);
			const char* text = (const char*)sqlite3_column_text(m_pstmt, 1);
			const char* proj = (const char*)sqlite3_column_text(m_pstmt, 2);

			pxNode = pxSRS->AddChild("srid", NULL);
			pxNode->SetChildText(srid);

			pxNode = pxSRS->AddChild("srtext", NULL);
			pxNode->SetChildText(text);

			pxNode = pxSRS->AddChild("proj4", NULL);
			pxNode->SetChildText(proj);
		}
		return pxDoc; 
	}

	RESULTCODE GetSpatialReferenceResponse::Write()
	{		
		XDocument* pxDoc = WriteDocument();
		int len = 0; 
		g_uchar* buffer = NULL;

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
		pxDoc->Release();

		return AG_SUCCESS;
	}


}