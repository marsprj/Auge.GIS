#include "CreateServiceRequest.h"
#include "CreateServiceResponse.h"
#include "AugeService.h"

namespace auge
{
	CreateServiceResponse::CreateServiceResponse(CreateServiceRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	CreateServiceResponse::~CreateServiceResponse()
	{
		m_pService->Release();
		m_pService = NULL;
	}

	RESULTCODE CreateServiceResponse::Write(WebWriter* pWriter)
	{
		return AG_SUCCESS;
	}

	void CreateServiceResponse::SetService(Service* pService)
	{
		m_pService = pService;
	}

}