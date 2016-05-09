#include "GProcessHandler.h"
#include "AugeWebCore.h"
#include "AugeUser.h"

namespace auge
{
	GProcessHandler::GProcessHandler()
	{
		m_pJob = NULL;
	}

	GProcessHandler::~GProcessHandler()
	{
		if(m_pJob!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJob);
		}
	}

	const char*	GProcessHandler::GetName()
	{
		return "";
	}

	const char*	GProcessHandler::GetDescription()
	{
		return "";
	}

	WebRequest*	GProcessHandler::ParseRequest(rude::CGI& cgi)
	{
		return NULL;
	}

	WebRequest*	GProcessHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return NULL;
	}

	WebRequest*	GProcessHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GProcessHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GProcessHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return NULL;
	}

	void GProcessHandler::Begin(User* pUser)
	{
		JobManager* pJobmanager = augeGetJobManagerInstance();
		WebContext* pWebContext = augeGetWebContextInstance();

		if(m_pJob!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pJob);
		}
		const char* client = "";
		const char* server = pWebContext->GetServer();
		const char* operation= GetName();
		const char* service = "gps";
		const char* params = "";
		m_pJob = pJobmanager->AddJob(pUser->GetID(), operation, operation, params, client, server);
	}

	void GProcessHandler::End()
	{
		JobManager* pJobmanager = augeGetJobManagerInstance();
		if(m_pJob!=NULL)
		{
			pJobmanager->SetEndTime(m_pJob->GetUUID());
		}
	}
}