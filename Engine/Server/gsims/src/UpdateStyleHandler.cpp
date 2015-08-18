#include "UpdateStyleHandler.h"
#include "UpdateStyleRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	UpdateStyleHandler::UpdateStyleHandler()
	{

	}

	UpdateStyleHandler::~UpdateStyleHandler()
	{

	}

	const char*	UpdateStyleHandler::GetName()
	{
		return "UpdateStyle";
	}

	WebRequest*	UpdateStyleHandler::ParseRequest(rude::CGI& cgi)
	{
		UpdateStyleRequest* pRequest = new UpdateStyleRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* UpdateStyleHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	UpdateStyleHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* UpdateStyleHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* UpdateStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{

		UpdateStyleRequest* pRequest = static_cast<UpdateStyleRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		GLogger* pLogger = augeGetLoggerInstance();
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		const char* text = pRequest->GetStyle();

		size_t text_len = strlen(text);
		size_t buff_len = text_len << 1;
		char* text_gbk = (char*)malloc(buff_len);
		memset(text_gbk, 0, buff_len);
		auge_encoding_convert_2("UTF-8", "GBK", text, text_len, text_gbk, &buff_len);

		pLogger->Info(text_gbk,__FILE__,__LINE__);
		
		RESULTCODE rc = pCartoManager->UpdateStyle(pUser->GetID(), name, text_gbk);
		free(text_gbk);

		if(rc!=AG_SUCCESS) 
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError());
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pWebResponse = pExpResponse;
		}
		else
		{
			WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
			pSusResponse->SetRequest(pRequest->GetRequest());
			pWebResponse = pSusResponse;
		}
		return pWebResponse;
		//return Execute(pWebRequest, pUser);
	}
}
