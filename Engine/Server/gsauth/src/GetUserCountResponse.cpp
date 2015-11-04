#include "GetUserCountRequest.h"
#include "GetUserCountResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetUserCountResponse::GetUserCountResponse(GetUserCountRequest* pRequest)
	{
		m_count = 0;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetUserCountResponse::~GetUserCountResponse()
	{
	}

	RESULTCODE GetUserCountResponse::Write(WebWriter* pWriter)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("UserCount", NULL, NULL);

		char str[AUGE_NAME_MAX];
		memset(str, 0, AUGE_NAME_MAX);
		g_snprintf(str, AUGE_NAME_MAX, "%d", m_count);
		pxRoot->SetChildText(str);

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

	void GetUserCountResponse::SetCount(g_uint count)
	{
		m_count = count;
	}
}