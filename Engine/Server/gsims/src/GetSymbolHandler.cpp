#include "GetSymbolHandler.h"
#include "GetSymbolRequest.h"
#include "GetSymbolResponse.h"

#include "AugeWebCore.h"
#include "AugeStyle.h"

namespace auge
{
	GetSymbolHandler::GetSymbolHandler()
	{

	}

	GetSymbolHandler::~GetSymbolHandler()
	{

	}

	const char*	GetSymbolHandler::GetName()
	{
		return "GetSymbol";
	}

	WebRequest*	GetSymbolHandler::ParseRequest(rude::CGI& cgi)
	{
		GetSymbolRequest* pRequest = new GetSymbolRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* GetSymbolHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetSymbolHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetSymbolHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetSymbolHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		// Check validation of Request
		GetSymbolRequest* pRequest = static_cast<GetSymbolRequest*>(pWebRequest);
		const char* type = pRequest->GetType();
		if(type==NULL)
		{
			const char* msg = "Parameter [Type] is NULL";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	const char* msg = "Parameter [Name] is NULL";
		//	pError->SetError(msg);
		//	pLogger->Error(msg, __FILE__, __LINE__);
		//	WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
		//	pExpResponse->SetMessage(msg);
		//	return pExpResponse;
		//}

		// GetSymbol 
		Symbol		*pSymbol  = NULL;
		EnumSymbol	*pSymbols = NULL;
		SymbolManager *pSymbolManager = augeGetSymbolManagerInstance();
		if(g_stricmp(type, "Marker")==0)
		{
			if(name==NULL)
				pSymbols = pSymbolManager->GetMarkerSymbols();
			else
				pSymbol = pSymbolManager->CreateMarkerSymbol(name);
		}
		else if(g_stricmp(type, "Line")==0)
		{
			if(name==NULL)
				pSymbols = pSymbolManager->GetLineSymbols();
			else
				pSymbol = pSymbolManager->CreateLineSymbol(name);
		}
		else if(g_stricmp(type, "Region")==0)
		{
			if(name==NULL)
				pSymbols = pSymbolManager->GetRegionSymbols();
			else
				pSymbol = pSymbolManager->CreateMarkerSymbol(name);
		}
		else
		{
			char msg[AUGE_MSG_MAX];
			memset(msg, 0, AUGE_MSG_MAX);
			g_sprintf(msg, "Invalid Symbol Type [%ds]", type);
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		// check whether symbol is get correctly
		if(name!=NULL)
		{
			if(pSymbol==NULL)
			{
				char msg[AUGE_MSG_MAX];
				memset(msg, 0, AUGE_MSG_MAX);
				g_sprintf(msg, "Invalid Symbol Name [%ds]", name);
				pError->SetError(msg);
				pLogger->Error(msg, __FILE__, __LINE__);
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(msg);
				return pExpResponse;
			}
		}

		GetSymbolResponse* pResponse = new GetSymbolResponse(pRequest);
		pResponse->SetSymbol(pSymbol);
		pResponse->SetSymbols(pSymbols);
		pResponse->SetWebContext(pWebContext);

		return pResponse;
	}
}