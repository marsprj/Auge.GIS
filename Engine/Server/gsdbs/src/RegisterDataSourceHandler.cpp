#include "RegisterDataSourceHandler.h"
#include "RegisterDataSourceRequest.h"
#include "AugeService.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	RegisterDataSourceHandler::RegisterDataSourceHandler()
	{

	}

	RegisterDataSourceHandler::~RegisterDataSourceHandler()
	{

	}

	const char*	RegisterDataSourceHandler::GetName()
	{
		return "RegisterDataSource";
	}

	WebRequest*	RegisterDataSourceHandler::ParseRequest(rude::CGI& cgi)
	{
		RegisterDataSourceRequest* pRequest = new RegisterDataSourceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RegisterDataSourceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RegisterDataSourceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* RegisterDataSourceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		const char* name = NULL;
		const char* engine = NULL;
		const char* uri = NULL;
		augeWorkspaceType type = augeWorkspaceUnknown;
		WebResponse* pWebResponse = NULL;
		RegisterDataSourceRequest* pRequest = static_cast<RegisterDataSourceRequest*>(pWebRequest);

		name = pRequest->GetName();
		engine = pRequest->GetDataEngine();
		uri = pRequest->GetURI();
		type = pRequest->GetSourceType();

		ConnectionManager*	pConnManager = augeGetConnectionManagerInstance();
		RESULTCODE rc = pConnManager->Register(pUser->GetID(), name, engine, uri, type);
		if(rc!=AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* RegisterDataSourceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}