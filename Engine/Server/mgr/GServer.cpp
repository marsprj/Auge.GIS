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
		m_counter = 0;

		WebWriter  *pWebWriter = augeCreateWebWriter();

		while(AUGE_FCGI_ACCEPT>=0)
		{
			AUGE_RUDE_CGI;

			//printf("Content-type: text/html\r\n"
			//	"\r\n"
			//	"%d",m_counter++);


			WebResponse* pWebResponse = NULL;

			switch(GetMethod())
			{
			case augeHttpGet:
				pWebResponse = DoGet(cgi);
				break;
			case augeHttpPost:
				pWebResponse = DoPost(cgi);
			}

			pWebResponse->Write(pWebWriter);
			AUGE_SAFE_RELEASE(pWebResponse);

			cgi.finish();
		}
	}

	WebResponse* GServer::DoGet(rude::CGI& cgi)
	{
		const char	*szService = NULL;
		WebEngine	*pWebEngine = NULL;
		WebRequest	*pWebRequest = NULL;
		WebResponse	*pWebResponse = NULL;
		
		szService = cgi["servicename"];
		pWebEngine = augeGetWebEngineInstance();
		if(pWebEngine==NULL)
		{
			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		pWebRequest = pWebEngine->ParseRequest(cgi);
		if(pWebRequest==NULL)
		{
			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		pWebResponse = Execute(pWebRequest);
		pWebRequest->Release();
		
		return pWebResponse;
	}

	WebResponse* GServer::DoPost(rude::CGI& cgi)
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

		pWebRequest = pWebEngine->ParseRequest(cgi["xml"]);
		if(pWebRequest==NULL)
		{
			// wrong service engine
			auge::WebExceptionResponse *pExpResponse = NULL;
			pExpResponse = auge::augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(m_pError->GetLastError());
			pWebResponse = pExpResponse;
			return pWebResponse;
		}

		pWebResponse = Execute(pWebRequest);
		pWebRequest->Release();

		return pWebResponse;
	}

	WebResponse* GServer::Execute(WebRequest* pWebRequest)
	{
		WebEngine	*pWebEngine = NULL;

		pWebEngine = augeGetWebEngineInstance();
		return pWebEngine->Execute(pWebRequest);
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
		return augeHttpGet;
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
}
