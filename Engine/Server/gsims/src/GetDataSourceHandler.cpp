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

	WebRequest*	GetDataSourceHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* GetDataSourceHandler::Execute(WebRequest* pWebRequest)
	{
		return new GetDataSourceResponse(static_cast<GetDataSourceRequest*>(pWebRequest));
	}

	WebResponse* GetDataSourceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}