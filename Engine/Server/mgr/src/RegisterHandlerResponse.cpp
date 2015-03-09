#include "RegisterHandlerRequest.h"
#include "RegisterHandlerResponse.h"
#include "AugeService.h"

namespace auge
{
	RegisterHandlerResponse::RegisterHandlerResponse(RegisterHandlerRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	RegisterHandlerResponse::~RegisterHandlerResponse()
	{
		m_pService->Release();
		m_pService = NULL;
	}

	RESULTCODE RegisterHandlerResponse::Write(WebWriter* pWriter)
	{
		return AG_SUCCESS;
	}

	void RegisterHandlerResponse::SetService(Service* pService)
	{
		m_pService = pService;
	}

}