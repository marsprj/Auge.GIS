#include "GetRoleHandler.h"
#include "GetRoleRequest.h"
#include "GetRoleResponse.h"
#include "AugeWebCore.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	GetRoleHandler::GetRoleHandler()
	{

	}

	GetRoleHandler::~GetRoleHandler()
	{

	}

	const char*	GetRoleHandler::GetName()
	{
		return "GetRole";
	}

	WebRequest*	GetRoleHandler::ParseRequest(rude::CGI& cgi)
	{
		GetRoleRequest* pRequest = new GetRoleRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetRoleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetRoleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		//GetRoleRequest* pRequest = new GetRoleRequest();
		////if(!pRequest->Create(pxDoc, pMap))
		//if(!pRequest->Create(pxDoc))
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	pRequest->Release();
		//	return NULL;
		//}
		//return pRequest;
		return NULL;
	}

	WebResponse* GetRoleHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GetRoleRequest* pRequest = static_cast<GetRoleRequest*>(pWebRequest);
		const char* name = pRequest->GetName();

		UserManager* pUserManager = augeGetUserManagerInstance();

		WebResponse* pWebResponse = NULL;
		if((name==NULL)||strlen(name)==0)
		{
			EnumRole* pRoles = pUserManager->GetRoles();
			GetRoleResponse* pResponse = new GetRoleResponse(pRequest);
			pResponse->SetRoles(pRoles);
			pWebResponse = pResponse;
		}
		else
		{
			Role *pRole = pUserManager->GetRole(name);
			if(pRole==NULL)
			{
				char msg[AUGE_MSG_MAX];
				g_sprintf(msg, "Role [%s] does not exist.", name);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				pWebResponse = pExpResponse;
			}
			else
			{
				GetRoleResponse* pResponse = new GetRoleResponse(pRequest);
				pResponse->SetRole(pRole);
				pWebResponse = pResponse;
			}

		}

		return pWebResponse;
	}

	WebResponse* GetRoleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);	
	}
}