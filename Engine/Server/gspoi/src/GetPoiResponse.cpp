#include "GetPoiRequest.h"
#include "GetPoiResponse.h"
#include "AugeService.h"
#include "AugeData.h"

#include <math.h>
#include <time.h>

namespace auge
{
	GetPoiResponse::GetPoiResponse(GetPoiRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetPoiResponse::~GetPoiResponse()
	{
		if(m_pRequest!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRequest);
		}
		if(m_pResultset!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pResultset);
		}
	}

	void GetPoiResponse::SetResultset(GResultSet* pResultset)
	{
		m_pResultset = pResultset;
	}

	RESULTCODE GetPoiResponse::Write(WebWriter* pWriter)
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

	XDocument* GetPoiResponse::WriteDocument()
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement  *pxRoot = NULL;
		XElement  *pxNode = NULL;
		XElement  *pxPoi  = NULL;
		XDocument *pxDoc = new XDocument();
		// FeatureCollection
		pxRoot = pxDoc->CreateRootNode("Pois", NULL, NULL);

		char str[AUGE_NAME_MAX];
		g_uint count = m_pResultset->GetCount();
		for(g_uint i=0; i<count; i++)
		{
			pxPoi->AddChild("Pois", NULL); 
			//gid,name,lat,lon,address,cdate,adcode,type
			pxNode = pxPoi->AddChild("name", NULL);
			pxNode->SetChildText(m_pResultset->GetString(i,1));
			
			g_sprintf(str, "%f", m_pResultset->GetDouble(i,2));
			pxNode = pxPoi->AddChild("lat", NULL);			
			pxNode->SetChildText(str);

			g_sprintf(str, "%f", m_pResultset->GetDouble(i,3));
			pxNode = pxPoi->AddChild("lon", NULL);
			pxNode->SetChildText(str);

			pxNode = pxPoi->AddChild("address", NULL);
			pxNode->SetChildText(m_pResultset->GetString(i,4));

			pxNode = pxPoi->AddChild("type", NULL);
			pxNode->SetChildText(m_pResultset->GetString(i,7));
		}
		
		return pxDoc; 
	}

}