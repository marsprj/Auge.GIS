#include "TestConnectionHandler.h"
#include "TestConnectionRequest.h"
#include "AugeService.h"
#include "AugeData.h"

namespace auge
{
	TryConnectionHandler::TryConnectionHandler()
	{

	}

	TryConnectionHandler::~TryConnectionHandler()
	{ 

	}

	const char*	TryConnectionHandler::GetName()
	{
		return "TryConnection";
	}

	WebRequest*	TryConnectionHandler::ParseRequest(rude::CGI& cgi)
	{
		TryConnectionRequest* pRequest = new TryConnectionRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	TryConnectionHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	TryConnectionHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* TryConnectionHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		const char* name = NULL;
		const char* engine = NULL;
		const char* uri = NULL;
		WebResponse* pWebResponse = NULL;
		TryConnectionRequest* pRequest = static_cast<TryConnectionRequest*>(pWebRequest);

		engine = pRequest->GetDataEngine();
		uri = pRequest->GetURI();

		DataEngineManager* pDataEngineManager = augeGetDataEngineManagerInstance();
		DataEngine* pDataEngine = pDataEngineManager->GetEngine(engine);
		if(pDataEngine==NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Invalid DataEnging [%s]", engine);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		GConnection* pConnection = pDataEngine->CreateConnection();
		pConnection->SetConnectionString(uri);
		RESULTCODE rc = pConnection->Open();
		pConnection->Release(); 
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

	WebResponse* TryConnectionHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}