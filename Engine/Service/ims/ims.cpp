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
auge::WebRequest* CreateWebRequest(rude::CGI& cgi);
auge::WebRequest* CreateWebRequest_Get(rude::CGI& cgi);
auge::WebRequest* CreateWebRequest_Post(rude::CGI& cgi);

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
		
		auge::WebRequest	*pWebRequest  = NULL;
		auge::WebResponse	*pWebResponse = NULL;

		const char* service_name = cgi["servicename"];
		// check whether [ServiceName] is valied
		if(service_name==NULL||strlen(service_name)==0)
		{
			const char* msg = "Paramenter [ServiceName] is NULL";
			pLogger->Error(msg, __FILE__, __LINE__);

			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pExpResponse->Write(pWebWriter);
			AUGE_SAFE_RELEASE(pWebResponse);
			cgi.finish();
			break;
		}

		// check whether Service Instance is valid
		auge::Service* pService = NULL;
		pService = pServiceManager->GetService(service_name);
		if(pService==NULL)
		{
			auge::GError* pError = auge::augeGetErrorInstance();
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);

			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			pExpResponse->Write(pWebWriter);
			AUGE_SAFE_RELEASE(pWebResponse);

			cgi.finish();
			break;
		}
		pWebContext->SetService(service_name);
		
		pWebRequest = CreateWebRequest(cgi);
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
		pWebResponse = pService->Execute(pWebRequest);
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

auge::WebRequest* CreateWebRequest(rude::CGI& cgi)
{
	auge::WebRequest* pRequest = NULL;
	auge::GError	*pError = auge::augeGetErrorInstance();
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();

	const char* method = getenv("REQUEST_METHOD");
	if(method==NULL)
	{
		pRequest = CreateWebRequest_Get(cgi);
		return pRequest;
	}

	if(g_stricmp(method, "POST")==0)
	{
		const char* content_type = getenv("CONTENT_TYPE");
		if(g_stricmp(content_type, "text/xml")==0)
		{
			pRequest = CreateWebRequest_Post(cgi);
		}
	}
	else// if(g_stricmp(method), "GET")==0)
	{
		pRequest = CreateWebRequest_Get(cgi);
	}
	return pRequest;
}


auge::WebRequest* CreateWebRequest_Get(rude::CGI& cgi)
{
	auge::WebRequest *pRequest = NULL;
	auge::GError	 *pError = auge::augeGetErrorInstance();
	auge::GLogger	 *pLogger = auge::augeGetLoggerInstance();

	const char* engine_type = cgi["service"];
	if(engine_type==NULL)
	{
		const char* msg = "Parameter [Service] is NULL";
		pError->SetError(msg);
		return NULL;
	}

	auge::WebEngine* pEngine = NULL;
	auge::WebEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetWebEngineManagerInstance();
	pEngine = pEngineManager->GetEngine(engine_type);
	if(pEngine==NULL)
	{
		const char* msg = pError->GetLastError();
		pLogger->Error(msg, __FILE__, __LINE__);
		return NULL;
	}

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
		//query_string = "servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities&styles=&bbox=-180,-90,180,90&width=800&height=600&transparent=false&bgcolor=#ff0000";

		//query_string = "servicename=world&service=wfs&version=1.1.0&request=getcapabilities";
		//query_string = "servicename=world&service=WFS&version=1.1.0&request=DescribeFeatureType&typeName=radi:rivers";
		query_string = "servicename=world&service=WFS&version=1.1.0&request=GetFeature&typeName=radi:rivers";
		
	}
	pLogger->Debug(query_string);
	pRequest = pEngine->ParseRequest(query_string);

	return pRequest;
}

auge::WebRequest* CreateWebRequest_Post(rude::CGI& cgi)
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

	auge::WebEngine* pEngine = NULL;
	auge::WebEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetWebEngineManagerInstance();
	pEngine = pEngineManager->GetEngine(engine_type);
	if(pEngine==NULL)
	{
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

void WriteTest()
{
	static int count = 0;
	printf("Content-Type: text/plain \r\n" "\r\n");
	printf("\r\n");
	printf("%d \r\n", count++);
}


//int main()
//{
//	int count = 0;
//	while(FCGI_Accept() >= 0) 
//	{
//		rude::CGI cgi; 
//	
//		printf("ContentType: text/plain \r\n" "\r\n");
//		printf("\r\n");
//		printf("%d \r\n", count++);
//
//		for(int i=0;i<100000; i++)
//		{
//		}
//		/*
//		char* szContentType = getenv("CONTENT_TYPE");
//		printf("ContentType:%s \n", (szContentType==NULL?"NULL":szContentType));
//		printf("<br>");
//		printf("ServiceName:%s \n", cgi["SERVICENAME"]);
//		printf("Path:%s \n", getenv("PATH"));
//
//		printf("xml:%s \n", cgi["xml"]);
//		*/
//
//		printf("\n\n");
//		
//		cgi.finish(); 
//	}
//	return 0;
//}