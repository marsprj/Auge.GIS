#include "DescribeServiceRequest.h"
#include "DescribeServiceResponse.h"
#include "AugeService.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeUser.h"

namespace auge
{
	DescribeServiceResponse::DescribeServiceResponse(DescribeServiceRequest* pRequest)
	{
		m_pUser = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	DescribeServiceResponse::~DescribeServiceResponse()
	{
	}

	void DescribeServiceResponse::SetUser(User* pUser)
	{
		m_pUser = pUser;
	}

	RESULTCODE DescribeServiceResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		g_uint user_id = m_pUser->GetID();
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
			pServices = pServiceManager->GetServices(user_id);
			pServices->Reset();

			while((pService=pServices->Next())!=NULL)
			{
				/*name = pService->GetName();
				pxService = pxRoot->AddChild("Service", NULL);
				pxService->SetAttribute("name", name, NULL);*/

				AddServiceNode(pxRoot, pService);
			}
			pServices->Release();
		}
		else
		{
			pService = pServiceManager->GetService(user_id, name);
			if(pService!=NULL)
			{
				/*name = pService->GetName();
				pxService = pxRoot->AddChild("Service", NULL);
				pxService->SetAttribute("name", name, NULL);*/

				AddServiceNode(pxRoot, pService);
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

	bool DescribeServiceResponse::AddServiceNode(XElement* pxParent, Service* pService)
	{
		const char* name = pService->GetName();
		const char* uri  = pService->GetURI();
		XElement* pxService = pxParent->AddChild("Service", NULL);
		pxService->SetAttribute("name", name, NULL);
		pxService->SetAttribute("uri", uri, NULL);

		AddMapNode(pxService, pService->GetMap());

		return true;
	}

	bool DescribeServiceResponse::AddMapNode(XElement* pxService, Map* pMap)
	{
		XElement* pxMap = pxService->AddChild("Map", NULL);
		if(pMap!=NULL)
		{
			const char* name = pMap->GetName();
			pxMap->SetAttribute("name", name, NULL);
		}
		return true;
	}
}
