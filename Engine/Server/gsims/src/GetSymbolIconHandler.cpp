#include "GetSymbolIconHandler.h"
#include "GetSymbolIconRequest.h"
#include "GetSymbolIconResponse.h"

#include "AugeWebCore.h"
#include "AugeStyle.h"
#include "AugeSymbol.h"

namespace auge
{
	GetSymbolIconHandler::GetSymbolIconHandler()
	{

	}

	GetSymbolIconHandler::~GetSymbolIconHandler()
	{

	}

	const char*	GetSymbolIconHandler::GetName()
	{
		return "GetSymbolIcon";
	}

	const char*	GetSymbolIconHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	GetSymbolIconHandler::ParseRequest(rude::CGI& cgi)
	{
		GetSymbolIconRequest* pRequest = new GetSymbolIconRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* GetSymbolIconHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetSymbolIconHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetSymbolIconHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetSymbolIconHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		// Check validation of Request
		GetSymbolIconRequest* pRequest = static_cast<GetSymbolIconRequest*>(pWebRequest);
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
		if(name==NULL)
		{
			const char* msg = "Parameter [Name] is NULL";
			pError->SetError(msg);
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		// GetSymbolIcon 
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
				pSymbols = pSymbolManager->GetFillSymbols();
			else
				pSymbol = pSymbolManager->CreateFillSymbol(name);
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

		GetSymbolIconResponse* pResponse = new GetSymbolIconResponse(pRequest);
		pResponse->SetSymbol(pSymbol);
		pResponse->SetWebContext(pWebContext);

		return pResponse;
	}
}