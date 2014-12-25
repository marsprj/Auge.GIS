#include "WSEngine.h"
#include "GetCapabilitiesRequest.h"
#include "GetCapabilitiesResponse.h"

#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WebManagerEngine g_webMapEngine;
		return &g_webMapEngine;
	}

	WebManagerEngine::WebManagerEngine():
	m_handler(NULL),
	m_requests("GetCapabilities;CreateService;RemoveService;UpdateService,GetService")
	{

	}

	WebManagerEngine::~WebManagerEngine()
	{

	}

	const char*	WebManagerEngine::GetID()
	{
		return "WSEngine";
	}

	const char*	WebManagerEngine::GetType()
	{
		return "ims";
	}

	const char*	WebManagerEngine::GetDescription()
	{
		return "Auge Web Service Manager";
	}

	void* WebManagerEngine::GetHandler()
	{
		return m_handler;
	}

	void WebManagerEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	const char*	WebManagerEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WebManagerEngine::ParseRequest(const char* url)
	{
		GPropertySet props;
		props.Parse(url,"&");

		WebRequest* pWebRequest = NULL;

		const char* request = props.GetValue("request");
		if(request==NULL)
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			return NULL;
		}

		
		if(g_stricmp(request, "GetCapabilities")==0)
		{
			GetCapabilitiesRequest* pRequest = new GetCapabilitiesRequest();
			if(!pRequest->Create(props))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
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


		return pWebRequest;
	}

	WebRequest*	WebManagerEngine::ParseRequest(XDocument* pxDoc)
	{
		GError* pError = augeGetErrorInstance();
		pError->SetError("WMS do not support xml request");
		return NULL;
	}

	WebResponse* WebManagerEngine::Execute(WebRequest* pWebRequest)
	{
		WebResponse		*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		if(!Support(request))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "WMS doesn't Request [%s].", request);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		if(g_stricmp(request, "GetCapabilities")==0)
		{
			GetCapabilitiesRequest* pGetCapaRequest = static_cast<GetCapabilitiesRequest*>(pWebRequest);
			pWebResponse = GetCapabilities(pGetCapaRequest);
		}
		//else if(g_stricmp(request, "GetMap")==0)
		//{
		//	GetMapRequest* pGetMapRequest = static_cast<GetMapRequest*>(pWebRequest);
		//	pWebResponse = GetMap(pGetMapRequest, pWebContext, pMap);
		//}
		//else if(g_stricmp(request, "GetFeatureInfo")==0)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "WMS doesn't Request [%s].", request);

		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);

		//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
		//	pExpResopnse->SetMessage(msg);
		//	pWebResponse = pExpResopnse;
		//}

		return pWebResponse;
	}

	WebResponse* WebManagerEngine::Execute(WebRequest* pWebRequest,WebContext* pWebContext, Map* pMap)
	{
		WebResponse		*pWebResponse = NULL;

		const char* request = pWebRequest->GetRequest();
		if(!Support(request))
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "WMS doesn't Request [%s].", request);

			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg);

			WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
			pExpResopnse->SetMessage(msg);
			return pExpResopnse;
		}

		if(g_stricmp(request, "GetCapabilities")==0)
		{
			GetCapabilitiesRequest* pGetCapaRequest = static_cast<GetCapabilitiesRequest*>(pWebRequest);
			pWebResponse = GetCapabilities(pGetCapaRequest);
		}
		//else if(g_stricmp(request, "GetMap")==0)
		//{
		//	GetMapRequest* pGetMapRequest = static_cast<GetMapRequest*>(pWebRequest);
		//	pWebResponse = GetMap(pGetMapRequest, pWebContext, pMap);
		//}
		//else if(g_stricmp(request, "GetFeatureInfo")==0)
		//{
		//	char msg[AUGE_MSG_MAX];
		//	g_sprintf(msg, "WMS doesn't Request [%s].", request);

		//	GLogger* pLogger = augeGetLoggerInstance();
		//	pLogger->Error(msg);

		//	WebExceptionResponse* pExpResopnse = augeCreateWebExceptionResponse();
		//	pExpResopnse->SetMessage(msg);
		//	pWebResponse = pExpResopnse;
		//}

		return pWebResponse;
	}

	bool WebManagerEngine::Support(const char* request)
	{
		if(request==NULL)
		{
			return false;
		}

		size_t pos = m_requests.find(request);
		return (pos!=std::string::npos);
	}

	WebResponse* WebManagerEngine::GetCapabilities(GetCapabilitiesRequest* pRequest)
	{	
		return new GetCapabilitiesResponse(pRequest);
	}
}