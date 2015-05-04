#include "GetUserRequest.h"
#include "GetUserResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetUserResponse::GetUserResponse(GetUserRequest* pRequest)
	{
		m_pUser = NULL;
		m_pUsers = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetUserResponse::~GetUserResponse()
	{
		if(m_pUsers!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUsers);
		}
		if(m_pUser!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUser);
		}
	}

	RESULTCODE GetUserResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		if(m_pUser!=NULL)
		{
			Write(pWriter, m_pUser);
		}
		else
		{
			Write(pWriter, m_pUsers);
		}
		return AG_SUCCESS;
	}

	RESULTCODE GetUserResponse::Write(WebWriter* pWriter, EnumUser* pUsers)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxRoot = pxDoc->CreateRootNode("Users", NULL, NULL);

		XElement	*pxService  = NULL;
		pUsers->Reset();
		User* pUser = NULL;
		while((pUser=pUsers->Next())!=NULL)
		{ 
			XElement* pxUser = pxRoot->AddChild("User");				
			
			XElement* pxElement = pxUser->AddChild("Name");
			pxElement->SetChildText(pUser->GetName());
			
			pxElement = pxUser->AddChild("Alias");
			pxElement->SetChildText(pUser->GetAlias());

			pxElement = pxUser->AddChild("Email");
			pxElement->SetChildText(pUser->GetEmail());

			pxElement = pxUser->AddChild("Role");
			pxElement->SetChildText(pUser->GetRole()->GetName());
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

	RESULTCODE GetUserResponse::Write(WebWriter* pWriter, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();

		XElement	*pxNode = NULL;
		XDocument	*pxDoc = new XDocument();
		XElement	*pxUser = pxDoc->CreateRootNode("User", NULL, NULL);							

		XElement* pxElement = pxUser->AddChild("Name");
		pxElement->SetChildText(pUser->GetName());

		pxElement = pxUser->AddChild("Alias");
		pxElement->SetChildText(pUser->GetAlias());

		pxElement = pxUser->AddChild("Email");
		pxElement->SetChildText(pUser->GetEmail());

		pxElement = pxUser->AddChild("Role");
		pxElement->SetChildText(pUser->GetRole()->GetName());

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

	void GetUserResponse::SetUsers(EnumUser* pUsers)
	{
		if(m_pUsers!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUsers);
		}
		m_pUsers = pUsers;
		if(m_pUser!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUser);
		}
	}

	void GetUserResponse::SetUser(User* pUser)
	{
		if(m_pUser!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUser);
		}
		if(m_pUsers!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUsers);
		}
		m_pUser = pUser;
	}
}