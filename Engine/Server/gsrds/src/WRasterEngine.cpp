#include "WRasterEngine.h"

#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"

#include "ListHandler.h"
#include "CreateFolderHandler.h"
#include "RemoveFolderHandler.h"
#include "RemoveFileHandler.h"
#include "MoveHandler.h"

#include "GetRasterHandler.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WRasterEngine g_webMapEngine;
		return &g_webMapEngine;
	}

	WRasterEngine::WRasterEngine()
	{
		m_handler = NULL;

		m_handlers.push_back(new ListHandler());
		m_handlers.push_back(new CreateFolderHandler());
		m_handlers.push_back(new RemoveFolderHandler());
		m_handlers.push_back(new RemoveFileHandler());
		m_handlers.push_back(new MoveHandler());

		m_handlers.push_back(new GetRasterHandler());
	}

	WRasterEngine::~WRasterEngine()
	{
		CleanupHandlers();
	}

	void WRasterEngine::CleanupHandlers()
	{
		WebHandler* pHandler = NULL;
		std::vector<WebHandler*>::iterator iter;
		for(iter=m_handlers.begin(); iter!=m_handlers.end(); iter++)
		{
			pHandler = *iter;
			pHandler->Release();
		}
		m_handlers.clear();
	}

	WebHandler*	WRasterEngine::GetHandler(const char* name)
	{
		std::vector<WebHandler*>::iterator iter;
		for(iter=m_handlers.begin(); iter!=m_handlers.end(); iter++)
		{
			if(g_stricmp((*iter)->GetName(), name)==0)
			{
				return *iter;
			}
		}
		return NULL;
	}

	const char*	WRasterEngine::GetID()
	{
		return "WRasterEngine";
	}

	const char*	WRasterEngine::GetType()
	{
		return "rds";
	}

	const char*	WRasterEngine::GetDescription()
	{
		return "Raster Data Service";
	}

	void* WRasterEngine::GetHandler()
	{
		return m_handler;
	}

	void WRasterEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WRasterEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WRasterEngine::ParseRequest(const char* url)
	{
		//GPropertySet props;
		//props.Parse(url,"&");

		//const char* request = props.GetValue("request");
		//if(request==NULL)
		//{
		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//	return NULL;
		//}

		//WebRequest* pWebRequest = NULL;
		//if(g_stricmp(request, "GetCapabilities")==0)
		//{
		//	GetCapabilitiesRequest* pRequest = new GetCapabilitiesRequest();
		//	if(!pRequest->Create(props))
		//	{
		//		GLogger* pLogger = augeGetLoggerInstance();
		//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//		pRequest->Release();
		//		pRequest = NULL;
		//	}
		//	pWebRequest = pRequest;
		//}
		//else if(g_stricmp(request, "GetMap")==0)
		//{
		//	GetMapRequest* pRequest = new GetMapRequest();

		//	if(!pRequest->Create(props))
		//	{
		//		GLogger* pLogger = augeGetLoggerInstance();
		//		pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
		//		pRequest->Release();
		//		pRequest = NULL;
		//	}
		//	pWebRequest = pRequest;

		//}
		//else if(g_stricmp(request, "GetFeatureInfo")==0)
		//{

		//}
		//return pWebRequest;

		return NULL;
	}

	WebRequest*	WRasterEngine::ParseRequest(rude::CGI& cgi)
	{
		const char* request = cgi["request"];
		if(request==NULL)
		{
			const char* msg = "[Request] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return NULL;
		}
		return handler->ParseRequest(cgi);
	}

	WebRequest* WRasterEngine::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		const char* request = cgi["request"];
		if(request==NULL)
		{
			const char* msg = "[Request] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return NULL;
		}
		return handler->ParseRequest(cgi,mapName);
	}

	//WebRequest* WRasterEngine::ParseRequest(rude::CGI& cgi, WebContext* pWebContext, Map* pMap)
	//{
	//	const char* request = cgi["request"];
	//	if(request==NULL)
	//	{
	//		const char* msg = "[Request] is NULL";
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error(msg, __FILE__, __LINE__);
	//		GError* pError = augeGetErrorInstance();
	//		pError->SetError(msg);
	//		return NULL;
	//	}
	//	WebHandler* handler = GetHandler(request);
	//	if(handler == NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error(msg, __FILE__, __LINE__);
	//		GError* pError = augeGetErrorInstance();
	//		pError->SetError(msg);

	//		return NULL;
	//	}
	//	return handler->ParseRequest(cgi, pWebContext, pMap);
	//}

	WebRequest*	WRasterEngine::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		XElement	*pxRoot = pxDoc->GetRootNode();
		const char* request = pxRoot->GetName();

		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);

			return NULL;
		}
		return handler->ParseRequest(pxDoc, mapName);
	}

	//WebRequest*	WRasterEngine::ParseRequest(XDocument* pxDoc,WebContext* pWebContext, Map* pMap)
	//{
	//	XElement	*pxRoot = pxDoc->GetRootNode();
	//	const char* request = pxRoot->GetName();

	//	WebHandler* handler = GetHandler(request);
	//	if(handler == NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s doesn't support request [%s]", GetType(), request);
	//		GLogger* pLogger = augeGetLoggerInstance();
	//		pLogger->Error(msg, __FILE__, __LINE__);
	//		GError* pError = augeGetErrorInstance();
	//		pError->SetError(msg);

	//		return NULL;
	//	}
	//	return handler->ParseRequest(pxDoc, pWebContext, pMap);
	//}

	WebResponse* WRasterEngine::Execute(WebRequest* pWebRequest)
	{
		WebResponse	*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}
		return handler->Execute(pWebRequest);
	}

	WebResponse* WRasterEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		return handler->Execute(pWebRequest, pWebContext);
	}
}