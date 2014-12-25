#include <fcgi_stdio.h>
#include <fcgi_config.h>
#include <rude/cgi.h>
#include <time.h>

#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeData.h"
#include "AugeService.h"
#include "AugeWebCore.h"

bool Startup();
void Shutdown();
auge::WebRequest* CreateWebRequest(auge::WebEngine* pEngine, rude::CGI& cgi);
auge::WebRequest* CreateWebRequest_Get(auge::WebEngine* pEngine, rude::CGI& cgi);
auge::WebRequest* CreateWebRequest_Post(auge::WebEngine* pEngine, rude::CGI& cgi);

void WriteCgiVariables();
void WriteCgiVariable(const char* key, const char* value);

void WriteTest();

int main()
{
	if(!Startup())
	{
		Shutdown();
		return 0;
	}

	auge::GLogger	 *pLogger = auge::augeGetLoggerInstance();
	auge::WebWriter  *pWebWriter = auge::augeCreateWebWriter();

	auge::ServiceManager *pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();
	auge::WebContext *pWebContext = NULL;
	pWebContext = auge::augeGetWebContextInstance();


#ifdef WIN32
	while(FCGI_Accept_t(&environ) >= 0) 
	{
		rude::CGI cgi(&environ);
#else
	while(FCGI_Accept() >= 0)
	{
		rude::CGI cgi;
#endif
		cgi.setCaseSensitive(false);

		WriteCgiVariables();

		auge::GError	*pError = auge::augeGetErrorInstance();
		auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
		auge::WebEngineManager* pEngineManager =  auge::augeGetWebEngineManagerInstance();

		auge::WebRequest	*pWebRequest  = NULL;
		auge::WebResponse	*pWebResponse = NULL;

		const char* engine_type = "ims";
		auge::WebEngine* pEngine = NULL;
		
		pEngine = pEngineManager->GetEngine(engine_type);
		if(pEngine==NULL)
		{
			const char* msg = pError->GetLastError();
			pLogger->Error(msg, __FILE__, __LINE__);
			return NULL;
		}
		
		pWebRequest = CreateWebRequest(pEngine, cgi);
		if(pWebRequest==NULL)
		{
			auge::GError* pError = auge::augeGetErrorInstance();
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pExpResponse->Write(pWebWriter);
			AUGE_SAFE_RELEASE(pWebResponse);

			cgi.finish();
			break;
		}
		clock_t t_s = clock();
		pWebResponse = pEngine->Execute(pWebRequest);
		if(pWebResponse!=NULL)
		{
			//pLogger->Error("Write Web Response", __FILE__, __LINE__);
			pWebResponse->Write(pWebWriter);
			AUGE_SAFE_RELEASE(pWebResponse);
		}
		clock_t t_e = clock();
		char temp[AUGE_MSG_MAX];
		g_sprintf(temp, "[%s]:%.3fºÁÃë", pWebRequest->GetRequest() , (double) (t_e - t_s) / (double) CLOCKS_PER_SEC);
		pLogger->Info(temp);

		AUGE_SAFE_RELEASE(pWebRequest);
		cgi.finish(); 
	}

	AUGE_SAFE_RELEASE(pWebWriter);
	Shutdown();

	return 0;
}

auge::WebRequest* CreateWebRequest(auge::WebEngine* pEngine, rude::CGI& cgi)
{
	auge::WebRequest* pRequest = NULL;
	auge::GError	*pError = auge::augeGetErrorInstance();
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();

	const char* method = getenv("REQUEST_METHOD");
	if(method==NULL)
	{
		pRequest = CreateWebRequest_Get(pEngine, cgi);
		return pRequest;
	}

	if(g_stricmp(method, "POST")==0)
	{
		const char* content_type = getenv("CONTENT_TYPE");
		if(g_stricmp(content_type, "text/xml")==0)
		{
			pRequest = CreateWebRequest_Post(pEngine, cgi);
		}
	}
	else// if(g_stricmp(method), "GET")==0)
	{
		pRequest = CreateWebRequest_Get(pEngine, cgi);
	}
	return pRequest;
}


auge::WebRequest* CreateWebRequest_Get(auge::WebEngine* pEngine, rude::CGI& cgi)
{
	auge::WebRequest *pRequest = NULL;
	auge::GError	 *pError = auge::augeGetErrorInstance();
	auge::GLogger	 *pLogger = auge::augeGetLoggerInstance();

	const char* query_string = getenv("QUERY_STRING");
	if(query_string==NULL)
	{
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#e01356";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#e01356";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetCapabilities";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#e01356";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#e01356";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=&bbox=-180,-90,180,90&width=800&height=600&transparent=false&bgcolor=#ff0000";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=country,cities,rivers&styles=&bbox=-180,-90,180,90&width=800&height=600&transparent=false&bgcolor=#ff0000";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=country&styles=country_red&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#ff0000";
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=rivers,country&styles=,polygon_red&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#e01356";
		query_string = "service=ims&version=1.0.0&request=GetCapabilities";
	}
	pLogger->Debug(query_string);
	pRequest = pEngine->ParseRequest(query_string);

	return pRequest;
}

auge::WebRequest* CreateWebRequest_Post(auge::WebEngine* pEngine, rude::CGI& cgi)
{
	auge::WebRequest *pRequest = NULL;
	auge::GError	 *pError = auge::augeGetErrorInstance();
	auge::GLogger	 *pLogger = auge::augeGetLoggerInstance();

	const char* xml = cgi["xml"];
	if(xml==NULL)
	{
		const char* msg = "Parameter [xml] is NULL";
		pError->SetError(msg);
		return NULL;
	}

	pLogger->Info(xml, __FILE__, __LINE__);

	auge::XParser parser;
	auge::XDocument* pxDoc = NULL;
	pxDoc = parser.ParseMemory(xml,0);
	if(pxDoc==NULL)
	{
		const char* msg = "Fail to Parse XML Request";
		pError->SetError(msg);
		return NULL;
	}

	auge::XElement* pxRoot = NULL;
	pxRoot = pxDoc->GetRootNode();
	if(pxRoot==NULL)
	{
		pError->SetError("Ivalid XML Request");
		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return NULL;
	}

	auge::XAttribute* pxAttr =  pxRoot->GetAttribute("service");
	if(pxAttr==NULL)
	{
		pError->SetError("Parameter [Service] is NULL");
		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return NULL;
	}
	const char* engine_type = pxAttr->GetValue();
	if(engine_type==NULL)
	{
		pError->SetError("Parameter [Service] is NULL");
		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return NULL;
	}

	pRequest = pEngine->ParseRequest(pxDoc);

	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);

	return pRequest;
}

bool Startup()
{
	RESULTCODE rc = AG_FAILURE;

	auge::GError	*pError = auge::augeGetErrorInstance();
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	pLogger->Info("===========================================================");
	pLogger->Info("Starting Internet Map Server");

	pLogger->Info("Load Data Engine");
	auge::DataEngineManager* pDataEngineManager = NULL;
	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	rc = pDataEngineManager->Load();
	if(rc!=AG_SUCCESS)
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}

	pLogger->Info("Load Service Engine");
	auge::WebEngineManager* pWebEngineManager = NULL;
	pWebEngineManager = auge::augeGetWebEngineManagerInstance();
	rc = pWebEngineManager->Load();
	if(rc!=AG_SUCCESS)
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}

	pLogger->Info("Load Config File");
	auge::WebContext* pWebContext = NULL;
	pWebContext = auge::augeGetWebContextInstance();
	rc = pWebContext->Initialize();
	if(rc!=AG_SUCCESS)
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}

	pLogger->Info("Initialize IMS Database Connection");
	auge::GConnection* pConnection = NULL;
	pConnection = pWebContext->GetConnection();
	if(!pConnection->IsOpen())
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}
	pLogger->Info("Load ");

	pLogger->Info("Initialize Connection Pool");
	auge::ConnectionManager* pConManager = NULL;
	pConManager = auge::augeGetConnectionManagerInstance();
	rc = pConManager->Initialize(pConnection);
	if(rc!=AG_SUCCESS)
	{
		pWebContext->Unload();
		pWebEngineManager->Unload();
		pDataEngineManager->Unload();
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}

	pLogger->Info("Initialize Carto Pool");
	auge::CartoManager *pCartoManager = auge::augeGetCartoManagerInstance();
	rc = pCartoManager->Initialize(pConnection);
	if(rc!=AG_SUCCESS)
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}

	pLogger->Info("Initialize Service Pool");
	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();
	pServiceManager->Initialize(pConnection);
	if(rc!=AG_SUCCESS)
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return false;
	}

	pLogger->Info("-----------------------------------------------------------");
	pLogger->Info("Internet Map Server Started.");
	pLogger->Info("-----------------------------------------------------------");
	return true; 
}

void Shutdown()
{
	auge::GLogger* pLogger = auge::augeGetLoggerInstance();

	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();
	auge::CartoManager *pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::ConnectionManager* pConManager = NULL;
	pConManager = auge::augeGetConnectionManagerInstance();
	auge::WebContext* pWebContext = NULL;
	pWebContext = auge::augeGetWebContextInstance();
	auge::WebEngineManager* pWebEngineManager = NULL;
	pWebEngineManager = auge::augeGetWebEngineManagerInstance();
	auge::DataEngineManager* pDataEngineManager = NULL;
	pDataEngineManager = auge::augeGetDataEngineManagerInstance();

	pLogger->Info("-----------------------------------------------------------");
	pLogger->Info("Shutdowning Internet Map Server");

	pLogger->Info("Shutdown Service Pool");
	pServiceManager->Unload();
	pLogger->Info("Shutdown Carto Pool");
	pCartoManager->Cleanup();
	pLogger->Info("Shutdown Connection Pool");
	pConManager->Unload();
	pLogger->Info("Shutdown Config");
	pWebContext->Unload();
	pLogger->Info("Unload Service Engine");

	pWebEngineManager->Unload();
	pLogger->Info("Unload Data Engine");
	pDataEngineManager->Unload();

	pLogger->Info("===========================================================");

}


void WriteCgiVariables()
{
	const char* key = "SERVER_NAME";
	WriteCgiVariable(key,getenv(key));
	key = "SERVER_SOFTWARE";
	WriteCgiVariable(key,getenv(key));
	key = "GATEWAY_INTERFACE";
	WriteCgiVariable(key,getenv(key));
	key = "SERVER_PROTOCOL";
	WriteCgiVariable(key,getenv(key));
	key = "SERVER_PORT";
	WriteCgiVariable(key,getenv(key));
	//key = "PATH";
	//WriteCgiVariable(key,getenv(key));
	key = "REQUEST_METHOD";
	WriteCgiVariable(key,getenv(key));
	key = "HTTP_HOST";
	WriteCgiVariable(key,getenv(key));
	key = "HTTP_ACCEPT";
	WriteCgiVariable(key,getenv(key));
	key = "HTTP_USER_AGENT";
	WriteCgiVariable(key,getenv(key));
	key = "HTTP_REFERER";
	WriteCgiVariable(key,getenv(key));
	key = "PATH_INFO";
	WriteCgiVariable(key,getenv(key));
	key = "PATH_TRANSLATED";
	WriteCgiVariable(key,getenv(key));
	key = "SCRIPT_NAME";
	WriteCgiVariable(key,getenv(key));
	key = "SCRIPT_FILENAME";
	WriteCgiVariable(key,getenv(key));
	key = "QUERY_STRING";
	WriteCgiVariable(key,getenv(key));
	key = "REMOTE_HOST";
	WriteCgiVariable(key,getenv(key));
	key = "REMOTE_ADDR";
	WriteCgiVariable(key,getenv(key));
	key = "REMOTE_USER";
	WriteCgiVariable(key,getenv(key));
	key = "REMOTE_IDENT";
	WriteCgiVariable(key,getenv(key));
	key = "CONTENT_TYPE";
	WriteCgiVariable(key,getenv(key));
	key = "CONTENT_LENGTH";
	WriteCgiVariable(key,getenv(key));
}

void WriteCgiVariable(const char* key, const char* value)
{
	char msg[AUGE_MSG_MAX];
	auge::GLogger* pLogger = auge::augeGetLoggerInstance();
	if(value==NULL)
		g_sprintf(msg, "CGI [%s]:", key);
	else
		g_sprintf(msg, "CGI [%s]:%s", key, value);
	pLogger->Info(msg);
}
