#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"
#include "AugeService.h"
#include "AugeData.h"

namespace auge
{
	GetDataSetResponse::GetDataSetResponse(GetDataSetRequest* pRequest)
	{
		m_pDataSets = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetDataSetResponse::~GetDataSetResponse()
	{
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
	}

	RESULTCODE GetDataSetResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Debug(m_path.c_str(), __FILE__, __LINE__);

		FILE* fp = fopen(m_path.c_str(), "rb");
		if(fp==NULL)
		{
			return AG_FAILURE;
		}

		pWriter->WriteHead(m_pRequest->GetMimeType());

		g_uint nBytes = 0;
		g_uchar buffer[AUGE_BUFFER_MAX];		
		memset(buffer, 0, AUGE_BUFFER_MAX);
		while((nBytes=fread(buffer, sizeof(g_uchar),AUGE_BUFFER_MAX, fp))>0)
		{
			pWriter->Write(buffer, nBytes);
		}

		pWriter->WriteTail();

		fclose(fp);

		return AG_SUCCESS;
	}

	void GetDataSetResponse::SetDataSets(EnumDataSet* pDataSets)
	{
		if(m_pDataSets!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pDataSets);
		}
		m_pDataSets = pDataSets;
	}
}