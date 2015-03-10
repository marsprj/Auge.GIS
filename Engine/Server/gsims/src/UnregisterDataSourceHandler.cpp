#include "UnRegisterDataSourceHandler.h"
#include "UnRegisterDataSourceRequest.h"
#include "AugeService.h"
#include "AugeData.h"

namespace auge
{
	UnRegisterDataSourceHandler::UnRegisterDataSourceHandler()
	{

	}

	UnRegisterDataSourceHandler::~UnRegisterDataSourceHandler()
	{

	}

	const char*	UnRegisterDataSourceHandler::GetName()
	{
		return "UnRegisterDataSource";
	}

	WebRequest*	UnRegisterDataSourceHandler::ParseRequest(rude::CGI& cgi)
	{
		UnRegisterDataSourceRequest* pRequest = new UnRegisterDataSourceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	UnRegisterDataSourceHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* UnRegisterDataSourceHandler::Execute(WebRequest* pWebRequest)
	{
		const char* name = NULL;
		WebResponse* pWebResponse = NULL;
		UnRegisterDataSourceRequest* pRequest = static_cast<UnRegisterDataSourceRequest*>(pWebRequest);

		name = pRequest->GetName();
		if(name==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("DataSource Name not Defined");
			return pExpResponse;
		}

		ConnectionManager*	pConnManager = augeGetConnectionManagerInstance();
		RESULTCODE rc = pConnManager->Unregister(name);
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

	WebResponse* UnRegisterDataSourceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}