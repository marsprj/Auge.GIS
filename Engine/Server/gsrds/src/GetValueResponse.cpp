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
	}

	GetValueResponse::~GetValueResponse()
	{
		if(m_pRequest!=NULL)
		{
			m_pRequest->Release();
		}
	}

	void GetValueResponse::SetPath(const char* path)
	{
		m_path = path;
	}

	RESULTCODE GetValueResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

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

}