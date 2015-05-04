#include "GetRoleRequest.h"
#include "GetRoleResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetRoleResponse::GetRoleResponse(GetRoleRequest* pRequest)
	{
		m_pRole = NULL;
		m_pRoles = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetRoleResponse::~GetRoleResponse()
	{
		if(m_pRoles!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRoles);
		}
		if(m_pRole!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRole);
		}
	}

	RESULTCODE GetRoleResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		if(m_pRole!=NULL)
		{
			Write(pWriter, m_pRole);
		}
		else
		{
			Write(pWriter, m_pRoles);
		}
	}

	RESULTCODE GetRoleResponse::Write(WebWriter* pWriter, EnumRole* pRoles)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Roles", NULL, NULL);

		XElement	*pxService  = NULL;
		pRoles->Reset();
		Role* pRole = NULL;
		while((pRole=pRoles->Next())!=NULL)
		{ 
			XElement* pxRole = pxRoot->AddChild("Role");				
			
			XElement* pxElement = pxRole->AddChild("Name");
			pxElement->SetChildText(pRole->GetName());
			
			pxElement = pxRole->AddChild("Alias");
			pxElement->SetChildText(pRole->GetAlias());
		}

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

	RESULTCODE GetRoleResponse::Write(WebWriter* pWriter, Role* pRole)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRole = pxDoc->CreateRootNode("Role", NULL, NULL);							

		XElement* pxElement = pxRole->AddChild("Name");
		pxElement->SetChildText(pRole->GetName());

		pxElement = pxRole->AddChild("Alias");
		pxElement->SetChildText(pRole->GetAlias());

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

	void GetRoleResponse::SetRoles(EnumRole* pRoles)
	{
		if(m_pRoles!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRoles);
		}
		m_pRoles = pRoles;
		if(m_pRole!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRole);
		}
	}

	void GetRoleResponse::SetRole(Role* pRole)
	{
		if(m_pRole!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRole);
		}
		if(m_pRoles!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pRoles);
		}
		m_pRole = pRole;
	}
}