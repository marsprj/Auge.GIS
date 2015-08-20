#include "GetDataSourceHandler.h"
#include "GetDataSourceRequest.h"
#include "GetDataSourceResponse.h"


namespace auge
{
	GetDataSourceHandler::GetDataSourceHandler()
	{

	}

	GetDataSourceHandler::~GetDataSourceHandler()
	{

	}

	const char*	GetDataSourceHandler::GetName()
	{
		return "GetDataSource";
	}

	const char*	GetDataSourceHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetDataSourceHandler::ParseRequest(rude::CGI& cgi)
	{
		GetDataSourceRequest* pRequest = new GetDataSourceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetDataSourceHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetDataSourceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetDataSourceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		GetDataSourceResponse* pResponse = new GetDataSourceResponse(static_cast<GetDataSourceRequest*>(pWebRequest));
		pResponse->SetUser(pUser);

		return pResponse;
	}

	WebResponse* GetDataSourceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}