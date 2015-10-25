#include "GetOnlineUserRequest.h"
#include "GetOnlineUserResponse.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	GetOnlineUserResponse::GetOnlineUserResponse(GetOnlineUserRequest* pRequest)
	{
		m_pUsers = NULL;
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetOnlineUserResponse::~GetOnlineUserResponse()
	{
		if(m_pUsers!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUsers);
		}
	}

	RESULTCODE GetOnlineUserResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		Write(pWriter, m_pUsers);
		return AG_SUCCESS;
	}

	RESULTCODE GetOnlineUserResponse::Write(WebWriter* pWriter, EnumUser* pUsers)
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
			
			//pxElement = pxUser->AddChild("Alias");
			//pxElement->SetChildText(pUser->GetAlias(), true);

			//pxElement = pxUser->AddChild("Email");
			//pxElement->SetChildText(pUser->GetEmail());

			//pxElement = pxUser->AddChild("Role");
			//pxElement->SetChildText(pUser->GetRole()->GetName());
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


	void GetOnlineUserResponse::SetUsers(EnumUser* pUsers)
	{
		if(m_pUsers!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pUsers);
		}
		m_pUsers = pUsers;
	}

}