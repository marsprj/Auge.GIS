#include "AddStyleHandler.h"
#include "AddStyleRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

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

	const char*	AddStyleHandler::GetDescription()
	{
		return GetName();
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

	WebResponse* AddStyleHandler::Execute(WebRequest* pWebRequest, User* pUser)
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
			g_sprintf(msg, "Invalid Type [%s].", type);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg,__FILE__,__LINE__);

			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		if(text==NULL)
		{
			const char* msg = "Parameter Style is NULL.";
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg,__FILE__,__LINE__);

			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		//size_t text_len = strlen(text);
		//if(text_len==0)
		//{
		//	const char* msg = "Invalid Style Text.";
		//	GError* pError = augeGetErrorInstance();
		//	pError->SetError(msg);

		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg,__FILE__,__LINE__);

		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}
		//size_t text_gbk_len = text_len<<1;
		//char *text_gbk = new char[text_gbk_len];
		//memset(text_gbk, 0, text_gbk_len);
		//auge_encoding_convert_2(AUGE_ENCODING_UTF8, AUGE_ENCODING_GBK, text, text_len, text_gbk, text_gbk_len);

		RESULTCODE rc = pCartoManager->CreateStyle(pUser->GetID(), name, text, gtype);
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

		//free(text_gbk);
		return pWebResponse;
	}

	WebResponse* AddStyleHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}