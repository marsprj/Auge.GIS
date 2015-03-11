#include "GServer.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeCarto.h"
#include "AugeData.h"
#include "AugeService.h"
#include "AugeWebCore.h"

#ifdef WIN32
#	define	AUGE_FCGI_ACCEPT	FCGI_Accept_t(&environ)
#else
#	define	AUGE_FCGI_ACCEPT	FCGI_Accept()
#endif

#ifdef WIN32
#	define	AUGE_RUDE_CGI	rude::CGI cgi(&environ)
#else
#	define	AUGE_RUDE_CGI	rude::CGI cgi
#endif

namespace auge
{
	GServer::GServer():
	m_pLogger(NULL),
	m_pDataEngineManager(NULL),
	m_pWebEngineManager(NULL),
	m_pWebContext(NULL),
	m_pConnection(NULL),
	m_pConnManager(NULL),
	m_pCartoManager(NULL),
	m_pServiceManager(NULL),
	m_pError(augeGetErrorInstance())
	{

	}

	GServer::~GServer()
	{
		Shutdown();
	}

	//========================================================================
	// Run Server
	//========================================================================
	void GServer::Run()
	{
		WebWriter  *pWebWriter = augeCreateWebWriter();

		while(AUGE_FCGI_ACCEPT>=0)
		{
			AUGE_RUDE_CGI;
			cgi.setCaseSensitive(false);

			g_ulong ts = auge_get_time();

			Service* pService = NULL;
			WebResponse* pWebResponse = NULL;

			const char* szServiceName = cgi["servicename"];
			pService = m_pServiceManager->GetService(szServiceName);
			if(pService==NULL)
			{
				// wrong service engine
				auge::WebExceptionResponse *pExpResponse = NULL;
				pExpResponse = auge::augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(m_pError->GetLastError());
				pWebResponse = pExpResponse;
			}
			else
			{
				switch(GetMethod())
				{
				case augeHttpGet:
					pWebResponse = DoGet(pService, cgi);
					break;
				case augeHttpPost:
					pWebResponse = DoPost(pService, cgi);
				}
			}
			pWebResponse->Write(pWebWriter);
			AUGE_SAFE_RELEASE(pWebResponse);

			g_ulong te = auge_get_time();
			WriteTime(ts, te);

			cgi.finish();
		}
	}
	
	WebResponse* GServer::DoGet(Service* pService, rude::CGI& cgi)
	{
		const char	*szService = NULL;
		WebEngine	*pWebEngine = NULL;
		WebRequest	*pWebRequest = NULL;
		WebResponse	*pWebResponse = NULL;
		
		szService = cgi["service"];
		pWebEngine = m_pWebEngineManager->GetEngine(szService);
		if(pWebEngine==NULL)
		{
			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		
		pWebRequest = pWebEngine->ParseRequest(cgi, pService->GetWebContext(), pService->GetMap());
		if(pWebRequest==NULL)
		{
			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		pWebResponse = pService->Execute(pWebRequest);
		pWebRequest->Release();
		
		return pWebResponse;
	}

	void GServer::DoGet_2(rude::CGI& cgi)
	{
		printf("Content-type: text/html\r\n"
				"\r\n"
				"%d",m_counter++);
	}

	WebResponse* GServer::DoPost(Service* pService, rude::CGI& cgi)
	{
		const char	*service = NULL;
		WebEngine	*pWebEngine = NULL;
		WebRequest	*pWebRequest = NULL;
		WebResponse	*pWebResponse = NULL;

		const char* xml_string = cgi["xml"];
		//const char* xml_string = "<wfs:GetFeature service=\"WFS\" version=\"1.1.0\"	xmlns:topp=\"http://www.openplans.org/topp\"	xmlns:wfs=\"http://www.opengis.net/wfs\"	xmlns:ogc=\"http://www.opengis.net/ogc\"	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"	xsi:schemaLocation=\"http://www.opengis.net/wfs	http://schemas.opengis.net/wfs/1.1.0/wfs.xsd\">	<wfs:Query typeName=\"world:cities\">		<ogc:Filter>			<ogc:FeatureId fid=\"world.3\"/>		</ogc:Filter>	</wfs:Query></wfs:GetFeature>";
		//const char* xml_string = "<wfs:Transaction service=\"WFS\" version=\"1.0.0\" xmlns:wfs=\"http://www.opengis.net/wfs\" xmlns:world=\"http://www.openplans.org/world\" xmlns:gml=\"http://www.opengis.net/gml\" xmlns:ogc=\"http://www.opengis.net/ogc\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.opengis.net/wfs http://schemas.opengis.net/wfs/1.0.0/WFS-transaction.xsd http://www.openplans.org/cities  http://localhost:8080/geoserver/wfs/DescribeFeaturename?namename=world:cities\"><wfs:Insert><world:cities><world:the_geom><gml:Point srsName=\"http://www.opengis.net/gml/srs/epsg.xml#4326\"><gml:coordinates>33.086040496826172,68.963546752929687</gml:coordinates></gml:Point></world:the_geom><world:name>alley</world:name></world:cities></wfs:Insert><wfs:Update name=\"cities\"><wfs:Property><wfs:Name>name</wfs:Name><wfs:Value>xxxx</wfs:Value></wfs:Property><ogc:Filter><ogc:FeatureId fid=\"cities.1\"/></ogc:Filter></wfs:Update><wfs:Delete name=\"cities\"><ogc:Filter><ogc:PropertyIsEqualTo><ogc:PropertyName>gid</ogc:PropertyName><ogc:Literal>607</ogc:Literal></ogc:PropertyIsEqualTo></ogc:Filter></wfs:Delete></wfs:Transaction>";
		m_pLogger->Debug("[Request]",__FILE__, __LINE__);
		m_pLogger->Debug(xml_string,__FILE__, __LINE__);

		XParser parser;
		XDocument* pxDoc = parser.ParseMemory(xml_string);
		if(pxDoc==NULL)
		{	
			const char* msg = "Bad xml document.";
			WebExceptionResponse *pExpResponse = NULL;
			GError* pError = augeGetErrorInstance();
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pError->SetError(msg);
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		XElement* pxRoot = pxDoc->GetRootNode();
		XAttribute* pxAttr = pxRoot->GetAttribute("service");
		if(pxAttr==NULL)
		{
			pxDoc->Close();
			pxDoc->Release();

			const char* msg = "Parameter Service is not defined";
			WebExceptionResponse *pExpResponse = NULL;
			GError* pError = augeGetErrorInstance();
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pError->SetError(msg);
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		service = pxAttr->GetValue();
		pWebEngine = m_pWebEngineManager->GetEngine(service);
		if(pWebEngine==NULL)
		{
			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}
		
		pWebRequest = pWebEngine->ParseRequest(pxDoc);
		if(pWebRequest==NULL)
		{
			pxDoc->Close();
			pxDoc->Release();

			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		//pxDoc->Close();
		pxDoc->Release();

		pWebResponse = pService->Execute(pWebRequest);
		pWebRequest->Release();

		return pWebResponse;
	}

	augeHttpMethodType GServer::GetMethod()
	{
		const char* method = getenv("REQUEST_METHOD");
		if(method == NULL)
		{
			return augeHttpGet;
		}

		if(strcmp(method, "GET")==0)
		{
			return augeHttpGet;
		}
		else if(strcmp(method, "POST")==0)
		{
			return augeHttpPost;
		}
	}

	//========================================================================
	// Startup Server
	//========================================================================
	bool GServer::Startup()
	{
		LoadLogger();

		m_pLogger->Info("===========================================================");
		m_pLogger->Info("Starting Internet Map Server");

		LoadConfig();

		LoadDataEngine();
		LoadServiceEngine();

		OpenServerBase();		
		LoadServerConfig();

		LoadConnectionPool();
		LoadCartoPool();
		LoadServicePool();

		m_pLogger->Info("-----------------------------------------------------------");
		m_pLogger->Info("Internet Map Server Started.");
		m_pLogger->Info("-----------------------------------------------------------");

		return true;
	}
	
	RESULTCODE GServer::LoadConfig()
	{
		m_pLogger->Info("Load Config File");
		
		m_pWebContext = auge::augeGetWebContextInstance();
		RESULTCODE rc = m_pWebContext->Initialize();
		if(rc!=AG_SUCCESS)
		{
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
		}
		return rc;
	}

	RESULTCODE GServer::LoadLogger()
	{
		m_pLogger = augeGetLoggerInstance();
		m_pLogger->Initialize();

		m_pLogger->Info("===========================================================");
		m_pLogger->Info("Starting Internet Map Server");

		return AG_SUCCESS;
	}

	RESULTCODE GServer::LoadDataEngine()
	{
		m_pLogger->Info("    Load Data Engine");

		m_pDataEngineManager = augeGetDataEngineManagerInstance();
		RESULTCODE rc = m_pDataEngineManager->Load();
		if(rc!=AG_SUCCESS)
		{
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
		}
		return rc;
	}

	RESULTCODE GServer::LoadServiceEngine()
	{
		m_pLogger->Info("    Load Service Engine");
		m_pWebEngineManager = auge::augeGetWebEngineManagerInstance();
		RESULTCODE rc = m_pWebEngineManager->Load();
		if(rc!=AG_SUCCESS)
		{
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
			return false;
		}
		return rc;
	}

	RESULTCODE GServer::OpenServerBase()
	{
		m_pLogger->Info("    Connect Server Base");

		m_pConnection = m_pWebContext->GetConnection();
		if(!m_pConnection->IsOpen())
		{
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
			return false;
		}
		m_pLogger->Info("Load ");
	}

	RESULTCODE GServer::LoadServerConfig()
	{
		m_pLogger->Info("    Load Server Pool");

		return AG_SUCCESS;
	}

	RESULTCODE GServer::LoadConnectionPool()
	{
		m_pLogger->Info("    Load Connection Pool");
		
		m_pConnManager = auge::augeGetConnectionManagerInstance();
		RESULTCODE rc = m_pConnManager->Initialize(m_pConnection);
		if(rc!=AG_SUCCESS)
		{
			m_pWebContext->Unload();
			m_pWebEngineManager->Unload();
			m_pDataEngineManager->Unload();
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
		}
		return rc;
	}

	RESULTCODE GServer::LoadCartoPool()
	{
		m_pLogger->Info("    Load Carto Pool");
		m_pCartoManager = auge::augeGetCartoManagerInstance();
		RESULTCODE rc = m_pCartoManager->Initialize(m_pConnection);
		if(rc!=AG_SUCCESS)
		{
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
		}
		return rc;
	}

	RESULTCODE GServer::LoadServicePool()
	{
		m_pLogger->Info("    Load Service Pool");
		m_pServiceManager = auge::augeGetServiceManagerInstance();
		RESULTCODE rc = m_pServiceManager->Initialize(m_pConnection);
		if(rc!=AG_SUCCESS)
		{
			m_pLogger->Error(m_pError->GetLastError(), __FILE__, __LINE__);
		}
		return rc;
	}

	//========================================================================
	// Startup Server
	//=======================================================================
	void GServer::Shutdown()
	{
		m_pLogger->Info("-----------------------------------------------------------");
		m_pLogger->Info("Shutdowning Internet Map Server");

		m_pLogger->Info("Shutdown Service Pool");
		m_pLogger->Info("Shutdown Carto Pool");
		if(m_pCartoManager!=NULL)
		{
			m_pCartoManager->Cleanup();
		}

		m_pLogger->Info("Shutdown Connection Pool");
		if(m_pConnManager!=NULL)
		{
			m_pConnManager->Unload();
		}

		m_pLogger->Info("Shutdown Config");
		if(m_pWebContext!=NULL)
		{
			m_pWebContext->Unload();
		}

		m_pLogger->Info("Unload Service Engine");
		if(m_pWebEngineManager!=NULL)
		{
			m_pWebEngineManager->Unload();
		}

		m_pLogger->Info("Unload Data Engine");
		if(m_pDataEngineManager!=NULL)
		{
			m_pDataEngineManager->Unload();
		}

		m_pLogger->Info("===========================================================");
	}

	inline
        void GServer::WriteTime(g_ulong ts, g_ulong te)
        {
		char msg[AUGE_MSG_MAX] = {0};
                //g_sprintf(msg, "[%d]:[%d]", te,ts);
		g_sprintf(msg, "[Time]:%3f ms", ((float)(te-ts))/1000.0f);
		m_pLogger->Debug(msg);
	}
}
