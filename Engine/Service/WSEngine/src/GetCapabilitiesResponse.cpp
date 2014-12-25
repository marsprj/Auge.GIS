#include "GetCapabilitiesResponse.h"
#include "GetCapabilitiesRequest.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeCore.h"
#include "AugeXML.h"

namespace auge
{
	GetCapabilitiesResponse::GetCapabilitiesResponse(GetCapabilitiesRequest* pRequest)
	{
		m_pRequest = pRequest;
	}

	GetCapabilitiesResponse::~GetCapabilitiesResponse()
	{

	}

	RESULTCODE GetCapabilitiesResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Debug(m_path.c_str(), __FILE__, __LINE__);
		
		Service* pService = NULL;
		EnumService *pServices = NULL;
		ServiceManager* pServiceManager = augeGetServiceManagerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("IMS_Capabilities", NULL, NULL);
		
		// IMS_Capabilities-->Capability
		XElement	*pxCapability = pxRoot->AddChild("Capability", NULL);
		// IMS_Capabilities-->Capability-->Request
		XElement	*pxRequest = pxCapability->AddChild("Request", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities
		XElement	*pxCapabilities = pxRequest->AddChild("GetCapabilities", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format
		XElement	*pxFormat = pxCapabilities->AddChild("Format", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType
		XElement	*pxDCPType = pxCapabilities->AddChild("DCPType", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP
		XElement	*pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get
		XElement	*pxGet = pxHttp->AddChild("Get", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Get-->OnlineResource
		XElement	*pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post
		XElement	*pxPost = pxHttp->AddChild("Post", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetCapabilities-->Format-->DCPType-->HTTP-->Post-->OnlineResource
		pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

		// IMS_Capabilities-->Capability-->Request-->GetService
		XElement	*pxGetService = pxRequest->AddChild("GetService", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format
		pxFormat = pxGetService->AddChild("Format", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format-->DCPType
		pxDCPType = pxGetService->AddChild("DCPType", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format-->DCPType-->HTTP
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format-->DCPType-->HTTP-->Get
		pxGet = pxHttp->AddChild("Get", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format-->DCPType-->HTTP-->Get-->OnlineResource
		pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format-->DCPType-->HTTP-->Post
		pxPost = pxHttp->AddChild("Post", NULL);
		// IMS_Capabilities-->Capability-->Request-->GetService-->Format-->DCPType-->HTTP-->Post-->OnlineResource
		pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

		// IMS_Capabilities-->Capability-->Request-->CreateService
		XElement	*pxCreateService= pxRequest->AddChild("CreateService", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format
		pxFormat = pxCreateService->AddChild("Format", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format-->DCPType
		pxDCPType = pxCreateService->AddChild("DCPType", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format-->DCPType-->HTTP
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format-->DCPType-->HTTP-->Get
		pxGet = pxHttp->AddChild("Get", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format-->DCPType-->HTTP-->Get-->OnlineResource
		pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format-->DCPType-->HTTP-->Post
		pxPost = pxHttp->AddChild("Post", NULL);
		// IMS_Capabilities-->Capability-->Request-->CreateService-->Format-->DCPType-->HTTP-->Post-->OnlineResource
		pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

		// IMS_Capabilities-->Capability-->Request-->RemoveService
		XElement	*pxRemoveService= pxRequest->AddChild("RemoveService", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format
		pxFormat = pxRemoveService->AddChild("Format", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format-->DCPType
		pxDCPType = pxRemoveService->AddChild("DCPType", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format-->DCPType-->HTTP
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format-->DCPType-->HTTP-->Get
		pxGet = pxHttp->AddChild("Get", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format-->DCPType-->HTTP-->Get-->OnlineResource
		pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format-->DCPType-->HTTP-->Post
		pxPost = pxHttp->AddChild("Post", NULL);
		// IMS_Capabilities-->Capability-->Request-->RemoveService-->Format-->DCPType-->HTTP-->Post-->OnlineResource
		pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

		// IMS_Capabilities-->Capability-->Request-->UpdateService
		XElement	*pxUpdateService= pxRequest->AddChild("UpdateService", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format
		pxFormat = pxUpdateService->AddChild("Format", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format-->DCPType
		pxDCPType = pxUpdateService->AddChild("DCPType", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format-->DCPType-->HTTP
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format-->DCPType-->HTTP-->Get
		pxGet = pxHttp->AddChild("Get", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format-->DCPType-->HTTP-->Get-->OnlineResource
		pxOnlineResource = pxGet->AddChild("OnlineResource", NULL);
		pxHttp = pxDCPType->AddChild("HTTP", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format-->DCPType-->HTTP-->Post
		pxPost = pxHttp->AddChild("Post", NULL);
		// IMS_Capabilities-->Capability-->Request-->UpdateService-->Format-->DCPType-->HTTP-->Post-->OnlineResource
		pxOnlineResource = pxPost->AddChild("OnlineResource", NULL);

		XElement	*pxServices = NULL;
		XElement	*pxService  = NULL;
		pxServices = pxRoot->AddChild("Services", NULL);
		 
		//pServices = pServiceManager->GetServices();
		//pServices->Reset();
		//pServices->Release();
		pServices = pServiceManager->GetServices();
		pServices->Reset();

		const char* name = NULL;
		while((pService=pServices->Next())!=NULL)
		{
			name = pService->GetName();
			pxService = pxServices->AddChild("Service", NULL);
			pxService->SetAttribute("name", name, NULL);
		}
		pServices->Release();
		
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

	void GetCapabilitiesResponse::SetPath(const char* path)
	{
		m_path = path;
	}

}