#include "RemoveServiceRequest.h"
#include "RemoveServiceResponse.h"
#include "AugeService.h"

namespace auge
{
	RemoveServiceResponse::RemoveServiceResponse(RemoveServiceRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	RemoveServiceResponse::~RemoveServiceResponse()
	{
	}

	RESULTCODE RemoveServiceResponse::Write(WebWriter* pWriter)
	{
		return AG_SUCCESS;
	}
}