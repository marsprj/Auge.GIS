#include "WFeatureEngine.h"

#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"

#include "CapabilitiesHandler.h"
#include "DescribeFeatureTypeHandler.h"
#include "GetFeatureHandler.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WFeatureEngine g_webMapEngine;
		return &g_webMapEngine;
	}

	WFeatureEngine::WFeatureEngine()
	{
		m_handler = NULL;

		m_handlers.push_back(new CapabilitiesHandler());
		m_handlers.push_back(new DescribeFeatureTypeHandler());
		m_handlers.push_back(new GetFeatureHandler());
	}

	WFeatureEngine::~WFeatureEngine()
	{
		CleanupHandlers();
	}

	void WFeatureEngine::CleanupHandlers()
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

	WebHandler*	WFeatureEngine::GetHandler(const char* name)
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

	const char*	WFeatureEngine::GetID()
	{
		return "WFEngine";
	}

	const char*	WFeatureEngine::GetType()
	{
		return "wfs";
	}

	const char*	WFeatureEngine::GetDescription()
	{
		return "OGC WFS";
	}

	void* WFeatureEngine::GetHandler()
	{
		return m_handler;
		}

	void WFeatureEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WFeatureEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WFeatureEngine::ParseRequest(const char* url)
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

	WebRequest* WFeatureEngine::ParseRequest(rude::CGI& cgi)
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

	WebRequest*	WFeatureEngine::ParseRequest(XDocument* pxDoc)
	{
		GError* pError = augeGetErrorInstance();
		pError->SetError("WMS do not support xml request");
		return NULL;
	}

	WebResponse* WFeatureEngine::Execute(WebRequest* pWebRequest)
	{
		WebResponse	*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
		}
		return handler->Execute(pWebRequest);
	}

	WebResponse* WFeatureEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		WebResponse	*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
		}
		return handler->Execute(pWebRequest, pWebContext, pMap);
	}
}