#include "WPoiEngine.h"

#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"

#include "GetPoiHandler.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WPoiEngine g_webPoiEngine;
		return &g_webPoiEngine;
	}

	WPoiEngine::WPoiEngine()
	{
		m_handler = NULL;

		m_handlers.push_back(new GetPoiHandler());
	}

	WPoiEngine::~WPoiEngine()
	{
		CleanupHandlers();
	}

	void WPoiEngine::CleanupHandlers()
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

	WebHandler*	WPoiEngine::GetHandler(const char* name)
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

	const char*	WPoiEngine::GetID()
	{
		return "WPoiEngine";
	}

	const char*	WPoiEngine::GetType()
	{
		return "poi";
	}

	const char*	WPoiEngine::GetDescription()
	{
		return "POI Data Service";
	}

	void* WPoiEngine::GetHandler()
	{
		return m_handler;
	}

	void WPoiEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WPoiEngine::GetLibraryPath()
	{
		return NULL;
	}

	RESULTCODE WPoiEngine::Initialize(void *pParam)
	{
		return AG_SUCCESS;
	}

	WebRequest*	WPoiEngine::ParseRequest(const char* url)
	{
		return NULL;
	}

	WebRequest*	WPoiEngine::ParseRequest(rude::CGI& cgi)
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

	WebRequest* WPoiEngine::ParseRequest(rude::CGI& cgi, const char* mapName)
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

	WebRequest*	WPoiEngine::ParseRequest(XDocument* pxDoc, const char* mapName)
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

	WebResponse* WPoiEngine::Execute(WebRequest* pWebRequest, User* pUser)
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
		return handler->Execute(pWebRequest, pUser);
	}

	WebResponse* WPoiEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
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

		return handler->Execute(pWebRequest, pWebContext, pUser);
	}

	void rds_get_raster_repository(char* raster_repository, size_t size, const char* user_name, WebContext* pWebContext)
	{
		char user_root[AUGE_PATH_MAX];
		memset(user_root, 0, AUGE_PATH_MAX);
		pWebContext->GetUserPath(user_name, user_root, AUGE_PATH_MAX);
		memset(raster_repository, 0, size);
		auge_make_path(raster_repository, NULL, user_root, "rds", NULL);
		auge_mkdir(raster_repository);
	}
	
}