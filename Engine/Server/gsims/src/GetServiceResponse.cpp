#include "GetServiceRequest.h"
#include "GetServiceResponse.h"
#include "AugeService.h"
#include "AugeXML.h"

namespace auge
{
	GetServiceResponse::GetServiceResponse(GetServiceRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetServiceResponse::~GetServiceResponse()
	{
	}

	RESULTCODE GetServiceResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();

		Service* pService = NULL;
		EnumService *pServices = NULL;
		ServiceManager* pServiceManager = augeGetServiceManagerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("IMS_Services", NULL, NULL);

		XElement	*pxServices = NULL;
		XElement	*pxService  = NULL;

		const char* name = m_pRequest->GetName();
		if(name==NULL)
		{
			pServices = pServiceManager->GetServices();
			pServices->Reset();

			while((pService=pServices->Next())!=NULL)
			{
				name = pService->GetName();
				pxService = pxRoot->AddChild("Service", NULL);
				pxService->SetAttribute("name", name, NULL);
			}
			pServices->Release();
		}
		else
		{
			pService = pServiceManager->GetService(name);
			if(pService!=NULL)
			{
				name = pService->GetName();
				pxService = pxRoot->AddChild("Service", NULL);
				pxService->SetAttribute("name", name, NULL);
			}
		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return AG_SUCCESS;
	}
}