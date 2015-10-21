#include "WFileSystemEngine.h"

#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"

#include "ListHandler.h"
#include "CreateFolderHandler.h"
#include "RemoveFolderHandler.h"
#include "RemoveFileHandler.h"
#include "MoveHandler.h"
#include "DescribeCsvHandler.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WFileSystemEngine g_webFileEngine;
		return &g_webFileEngine;
	}

	WFileSystemEngine::WFileSystemEngine()
	{
		m_handler = NULL;

		m_handlers.push_back(new ListHandler());
		m_handlers.push_back(new CreateFolderHandler());
		m_handlers.push_back(new RemoveFolderHandler());
		m_handlers.push_back(new RemoveFileHandler());
		m_handlers.push_back(new MoveHandler());
		m_handlers.push_back(new DescribeCsvHandler());
	}

	WFileSystemEngine::~WFileSystemEngine()
	{
		CleanupHandlers();
	}

	void WFileSystemEngine::CleanupHandlers()
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

	WebHandler*	WFileSystemEngine::GetHandler(const char* name)
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

	const char*	WFileSystemEngine::GetID()
	{
		return "WFEngine";
	}

	const char*	WFileSystemEngine::GetType()
	{
		return "ufs";
	}

	const char*	WFileSystemEngine::GetDescription()
	{
		return "OGC WFS";
	}

	void* WFileSystemEngine::GetHandler()
	{
		return m_handler;
	}

	void WFileSystemEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WFileSystemEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WFileSystemEngine::ParseRequest(const char* url)
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

	WebRequest*	WFileSystemEngine::ParseRequest(rude::CGI& cgi)
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

	WebRequest* WFileSystemEngine::ParseRequest(rude::CGI& cgi, const char* mapName)
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

	//WebRequest* WFileSystemEngine::ParseRequest(rude::CGI& cgi, WebContext* pWebContext, Map* pMap)
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

	WebRequest*	WFileSystemEngine::ParseRequest(XDocument* pxDoc, const char* mapName)
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

	//WebRequest*	WFileSystemEngine::ParseRequest(XDocument* pxDoc,WebContext* pWebContext, Map* pMap)
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

	WebResponse* WFileSystemEngine::Execute(WebRequest* pWebRequest, User* pUser)
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

	WebResponse* WFileSystemEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
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

	bool make_user_path(char* user_path, size_t size, const char* root_path, const char* user_name)
	{
		//strcpy(user_path, root_path);
		//return true;
		auge_make_path(user_path, NULL, root_path, user_name, NULL);
		if(g_access(user_name, 4))
		{
			return auge_mkdir(user_path);
		}
		return true;
	}
}