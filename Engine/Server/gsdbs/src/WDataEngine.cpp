#include "WDataEngine.h"

#include "RegisterDataSourceHandler.h"
#include "UnRegisterDataSourceHandler.h"
#include "GetDataSourceHandler.h"
#include "GetDataSetHandler.h"
#include "GetPreviewHandler.h"

#include "CreateDataSetHandler.h"
#include "RemoveDataSetHandler.h"
#include "RefreshDataSetHandler.h"

#include "TestConnectionHandler.h"

#include "DescribeTileStoreHandler.h"
#include "CreateTileStoreHandler.h"
#include "RemoveTileStoreHandler.h"
#include "GetTileHandler.h"


#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeWebCore.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	WebEngine* augeGetWebEngineInstance()
	{
		static WDataEngine g_webMapEngine;
		return &g_webMapEngine;
	} 

	WDataEngine::WDataEngine()
	{ 
		m_handler = NULL;

		m_handlers.push_back(new RegisterDataSourceHandler());
		m_handlers.push_back(new UnRegisterDataSourceHandler()); 
		m_handlers.push_back(new GetDataSourceHandler());
		m_handlers.push_back(new GetDataSetHandler());
		m_handlers.push_back(new GetPreviewHandler());
		m_handlers.push_back(new CreateDataSetHandler());
		m_handlers.push_back(new RemoveDataSetHandler());
		m_handlers.push_back(new TryConnectionHandler());
		m_handlers.push_back(new RefreshDatasetHandler());

		m_handlers.push_back(new CreateTileStoreHandler());
		m_handlers.push_back(new DescribeTileStoreHandler());
		m_handlers.push_back(new RemoveTileStoreHandler());
		m_handlers.push_back(new GetTileHandler());
	}

	WDataEngine::~WDataEngine()
	{
		m_handler = NULL;
		CleanupHandlers();
	}

	void* WDataEngine::GetHandler()
	{
		return m_handler;
	}

	void WDataEngine::SetHandler(void* handler)
	{
		m_handler = handler;
	}

	void WDataEngine::CleanupHandlers()
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

	WebHandler*	WDataEngine::GetHandler(const char* name)
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

	const char*	WDataEngine::GetID()
	{
		return "WDataEngine";
	}

	const char*	WDataEngine::GetType()
	{
		return "dbs";
	}

	const char*	WDataEngine::GetDescription()
	{
		return "Auge Web Service Manager";
	}

	const char*	WDataEngine::GetLibraryPath()
	{
		return NULL;
	}

	WebRequest*	WDataEngine::ParseRequest(const char* url)
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
			//CapabilitiesRequest* pRequest = new CapabilitiesRequest();
			//if(!pRequest->Create(props))
			//{
			//	GLogger* pLogger = augeGetLoggerInstance();
			//	pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			//	pRequest->Release();
			//	pRequest = NULL;
			//}
			//pWebRequest = pRequest;
		}

		return pWebRequest;
	}

	WebRequest*	WDataEngine::ParseRequest(rude::CGI& cgi)
	{
		const char* request = cgi["request"];
		if(request==NULL)
		{ 
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
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

	WebRequest*	WDataEngine::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	/*WebRequest*	WDataEngine::ParseRequest(rude::CGI& cgi)
	{
		WebRequest* pWebRequest = NULL;

		const char* request = cgi["request"];
		if(request==NULL)
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			return NULL;
		}


		if(g_stricmp(request, "GetCapabilities")==0)
		{ 
			CapabilitiesRequest* pRequest = new CapabilitiesRequest();
			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
		else if(g_stricmp(request, "CreateService")==0)
		{
			CreateServiceRequest* pRequest = new CreateServiceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		else if(g_stricmp(request, "RemoveService")==0)
		{
			RemoveServiceRequest* pRequest = new RemoveServiceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;
		}
		else if(g_stricmp(request, "GetService")==0)
		{
			GetServiceRequest* pRequest = new GetServiceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		else if(g_stricmp(request, "RegisterDataSource")==0)
		{
			RegisterDataSourceRequest* pRequest = new RegisterDataSourceRequest();

			if(!pRequest->Create(cgi))
			{
				GLogger* pLogger = augeGetLoggerInstance();
				pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
				pRequest->Release();
				pRequest = NULL;
			}
			pWebRequest = pRequest;

		}
		return pWebRequest;
	}*/

	WebRequest*	WDataEngine::ParseRequest(XDocument* pxDoc, const char* mapName)
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

	WebResponse* WDataEngine::Execute(WebRequest* pWebRequest, User* pUser)
	{
		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
		}

		g_ulong ts = auge_get_time();

		WebResponse* pWebResponse = handler->Execute(pWebRequest, pUser);

		g_ulong te = auge_get_time();
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg, "[%s Execute]:%ld ms", pWebRequest->GetRequest(), te-ts);
		augeGetLoggerInstance()->Debug(msg, __FILE__, __LINE__);

		return pWebResponse;
	}

	/*WebResponse* WDataEngine::Execute(WebRequest* pWebRequest, User* pUser)
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
			CapabilitiesRequest* pCapaRequest = static_cast<CapabilitiesRequest*>(pWebRequest);
			pWebResponse = GetCapabilities(pCapaRequest);
		}
		else if(g_stricmp(request, "CreateService")==0)
		{
			CreateServiceRequest* pRequest = static_cast<CreateServiceRequest*>(pWebRequest);
			pWebResponse = CreateService(pRequest);
		}
		else if(g_stricmp(request, "RemoveService")==0)
		{
			RemoveServiceRequest* pRequest = static_cast<RemoveServiceRequest*>(pWebRequest);
			pWebResponse = RemoveService(pRequest);
		}
		else if(g_stricmp(request, "GetService")==0)
		{
			GetServiceRequest* pRequest = static_cast<GetServiceRequest*>(pWebRequest);
			pWebResponse = GetService(pRequest);
		}
		else if(g_stricmp(request, "RegisterDataSource")==0)
		{
			RegisterDataSourceRequest* pRequest = static_cast<RegisterDataSourceRequest*>(pWebRequest);
			pWebResponse = RegisterDataSource(pRequest);
		}

		return pWebResponse;
	}*/

	WebResponse* WDataEngine::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		const char* request = pWebRequest->GetRequest();
		WebHandler* handler = GetHandler(request);
		if(handler == NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Request %s is not supported", request);
		}

		Begin(pWebRequest, pWebContext, pUser);
		WebResponse* pWebResponse = handler->Execute(pWebRequest, pWebContext, pUser);
		End();
		return pWebResponse;
	}

	void WDataEngine::Begin(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{ 
		JobManager* pJobmanager = augeGetJobManagerInstance(); 

		const char* client = "";
		const char* server = pWebContext->GetServer();
		const char* operation= pWebRequest->GetRequest();
		const char* service = GetType();
		const char* params = "";
		m_pJob = pJobmanager->AddJob(pUser->GetID(), service, operation, params, client, server);
	}

	void WDataEngine::End()
	{
		JobManager* pJobmanager = augeGetJobManagerInstance();
		if(m_pJob!=NULL)
		{
			pJobmanager->SetEndTime(m_pJob->GetUUID());
			AUGE_SAFE_RELEASE(m_pJob);
		}
	}

	//WebResponse* WDataEngine::GetCapabilities(CapabilitiesRequest* pRequest)
	//{	
	//	return new CapabilitiesResponse(pRequest);
	//}

	//WebResponse* WDataEngine::CreateService(CreateServiceRequest* pRequest)
	//{
	//	const char* name = NULL;
	//	ServiceManager* pServiceManager = NULL;
	//	Service* pService = NULL;

	//	name = pRequest->GetName();
	//	pServiceManager = augeGetServiceManagerInstance();
	//	pService = pServiceManager->GetService(name);
	//	if(pService!=NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s already exists.", name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	RESULTCODE rc = pServiceManager->Register(name);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
	//	pSusResponse->SetRequest("CreateService");
	//	return pSusResponse;
	//}

	//WebResponse* WDataEngine::RemoveService(RemoveServiceRequest* pRequest)
	//{
	//	const char* name = NULL;
	//	ServiceManager* pServiceManager = NULL;
	//	Service* pService = NULL;

	//	name = pRequest->GetName();
	//	pServiceManager = augeGetServiceManagerInstance();
	//	pService = pServiceManager->GetService(name);
	//	if(pService==NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s does not exists.", name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}

	//	RESULTCODE rc = pServiceManager->Unregister(name);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
	//	pSusResponse->SetRequest("RemoveService");
	//	return pSusResponse;
	//}

	//WebResponse* WDataEngine::GetService(GetServiceRequest* pRequest)
	//{
	//	return new GetServiceResponse(pRequest);
	//}

	//WebResponse* WDataEngine::RegisterHandler(RegisterHandlerRequest* pRequest)
	//{
	//	const char* service_name = NULL;
	//	const char* handler = NULL;
	//	WebResponse* pResponse = NULL;

	//	service_name = pRequest->GetName();
	//	handler = pRequest->GetHandler();

	//	Service* pService = NULL;
	//	ServiceManager* pServiceManager = NULL;
	//	pServiceManager = augeGetServiceManagerInstance();
	//	pService = pServiceManager->GetService(service_name);
	//	if(pService!=NULL)
	//	{
	//		char msg[AUGE_MSG_MAX];
	//		g_sprintf(msg, "%s already exists.", service_name);
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(msg);
	//		return pExpResponse;
	//	}
	//	
	//	return pResponse;
	//}
	//
	//WebResponse* WDataEngine::RegisterDataSource(RegisterDataSourceRequest* pRequest)
	//{
	//	const char* name = NULL;
	//	const char* engine = NULL;
	//	const char* uri = NULL;
	//	WebResponse* pWebResponse = NULL;

	//	name = pRequest->GetName();
	//	engine = pRequest->GetDataEngine();
	//	uri = pRequest->GetURI();

	//	ConnectionManager*	pConnManager = augeGetConnectionManagerInstance();
	//	RESULTCODE rc = pConnManager->Register(name, engine, uri);
	//	if(rc!=AG_SUCCESS)
	//	{
	//		GError* pError = augeGetErrorInstance();
	//		WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
	//		pExpResponse->SetMessage(pError->GetLastError());
	//		return pExpResponse;
	//	}

	//	WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
	//	pSusResponse->SetRequest(pRequest->GetRequest());
	//	return pSusResponse;
	//}
}