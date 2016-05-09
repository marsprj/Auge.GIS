#include "JobStatisticsRequest.h"
#include "JobStatisticsResponse.h"

namespace auge
{ 
	JobStatisticsResponse::JobStatisticsResponse(JobStatisticsRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
		m_pStatistics = NULL;
	}

	JobStatisticsResponse::~JobStatisticsResponse()
	{
		AUGE_SAFE_RELEASE(m_pStatistics);
	}

	void JobStatisticsResponse::SetWebContext(WebContext* pWebContext)
	{
		m_pWebContext = pWebContext;
	}

	void JobStatisticsResponse::SetStatistics(GStatistics* pStatistics)
	{
		m_pStatistics = pStatistics;
	}

	RESULTCODE JobStatisticsResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		XDocument* pxDoc = new XDocument();
		WriteStatistics(pxDoc);

		int len = 0;
		g_uchar* buffer = NULL;
		GLogger* pLogger = augeGetLoggerInstance();

		pxDoc->WriteToString(&buffer, len, m_pRequest->GetEncoding(),1);
		
		pWriter->WriteHead(m_pRequest->GetMimeType(),false);
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		return AG_SUCCESS;
	}

	void JobStatisticsResponse::WriteStatistics(XDocument* pxDoc)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XElement	*pxRoot = pxDoc->CreateRootNode("Statistics", NULL, NULL);
		XAttribute	*pxAttr = NULL;

		char value[AUGE_NAME_MAX];
		const char* item = NULL;
		g_uint item_count = 0;
		g_uint count = m_pStatistics->GetCount();
		for(g_uint i=0; i<count; i++)
		{ 
			item = m_pStatistics->GetKey(i);
			item_count = m_pStatistics->GetValue(i);

			g_snprintf(value, AUGE_NAME_MAX, "%d", item_count);
			pxNode = pxRoot->AddChild("Item");
			pxNode->SetAttribute("key",item,NULL);
			pxNode->SetAttribute("count",value,NULL);
		}
	}
}