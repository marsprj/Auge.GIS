#include "AddStyleHandler.h"
#include "AddStyleRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"


namespace auge
{
	AddStyleHandler::AddStyleHandler()
	{

	}

	AddStyleHandler::~AddStyleHandler()
	{

	}

	const char*	AddStyleHandler::GetName()
	{
		return "AddStyle";
	}

	WebRequest*	AddStyleHandler::ParseRequest(rude::CGI& cgi)
	{
		AddStyleRequest* pRequest = new AddStyleRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		pRequest->Info();
		return pRequest;
	}

	WebRequest* AddStyleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	AddStyleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* AddStyleHandler::Execute(WebRequest* pWebRequest)
	{
		AddStyleRequest* pRequest = static_cast<AddStyleRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		const char* text = pRequest->GetStyle();
		const char* type = pRequest->GetType();
		GeometryFactory* pGeometryFactory = augeGetGeometryFactoryInstance();
		augeGeometryType gtype = pGeometryFactory->DecodeGeometryType(type);
		if(gtype==augeGTNull)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Ivalid Type [%s].", type);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg,__FILE__,__LINE__);

			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = pCartoManager->CreateStyle(name, text, gtype);
		if(rc>0)
		{
			WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
			pSusResponse->SetRequest(pRequest->GetRequest());
			pWebResponse = pSusResponse;
		}
		else
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;			
		}
		return pWebResponse;
	}

	WebResponse* AddStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		return Execute(pWebRequest);
	}
}