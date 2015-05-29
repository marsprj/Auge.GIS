#include "WebContextImpl.h"
#include "AugeCore.h"
#include "AugeXML.h"
#include "AugeData.h"

namespace auge
{
	WebContext*	augeGetWebContextInstance()
	{
		static WebContextImpl g_webContext;
		return &g_webContext;
	}

	WebContextImpl::WebContextImpl()
	{
		m_name = "world";
		m_cache_path = "G:\\temp\\map\\";
		m_pConnection = NULL;
		m_response_encoding = "GBK";
		m_uri = "http://www.radi.ac.cn";
	}

	WebContextImpl::~WebContextImpl()
	{
		Unload();
	}
	
	RESULTCODE WebContextImpl::Initialize(const char* config_path) 
	{
		RESULTCODE rc = AG_FAILURE;
		rc = LoadConfig(config_path);
		if(rc!=AG_SUCCESS)
		{
			return rc;
		}

		InitializeCache();

		return AG_SUCCESS;
	}

	RESULTCODE WebContextImpl::LoadConfig(const char* config_path)
	{
		if(config_path!=NULL)
			m_path = config_path;
		else
			MakeConfigPath();

		XParser parser;
		XDocument* pxDoc = NULL;
		pxDoc = parser.Parse(m_path.c_str());
		if(pxDoc==NULL)
		{
			return AG_FAILURE;
		}

		XNode *pxRoot = NULL;
		pxRoot = pxDoc->GetRootNode();
		if(pxRoot==NULL)
		{
			pxDoc->Close();
			pxDoc->Release();
			return AG_FAILURE;
		}

		XNode *pxStorage = NULL;
		pxStorage = pxRoot->GetFirstChild("Store");
		if(pxStorage==NULL)
		{
			pxDoc->Close();
			pxDoc->Release();
			return AG_FAILURE;
		}

		const char* nodeName = NULL;
		XNode *pxNode = NULL;
		XNodeSet* pxNodeSet = NULL;
		pxNodeSet = pxStorage->GetChildren();
		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			nodeName = pxNode->GetName();
			if(g_stricmp(nodeName, "Engine")==0)
			{
				if(pxNode->GetContent()!=NULL)
				{
					m_data_engine = pxNode->GetContent();
				}
			}
			else if(g_stricmp(nodeName, "URI")==0)
			{
				if(pxNode->GetContent()!=NULL)
				{
					m_data_uri = pxNode->GetContent();
				}
			}
		}
		pxDoc->Close();
		pxDoc->Release();
		return AG_SUCCESS;
	}

	RESULTCODE WebContextImpl::Unload()
	{
		if(m_pConnection!=NULL)
		{
			m_pConnection->Close();
			m_pConnection->Release();
			m_pConnection = NULL;
		}
		return AG_SUCCESS;
	}

	void WebContextImpl::MakeConfigPath()
	{
		char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		auge_make_path(path, NULL, cdir, "conf\\service", "xml");
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(path, NULL, pdir, "conf/service", "xml");
#endif
		m_path = path;
	}

	const char*	WebContextImpl::GetService()
	{
		return m_name.c_str();
	}

	void WebContextImpl::SetService(const char* named) 
	{
		if(named==NULL)
		{
			m_name.clear();
		}
		m_name = named;
	}

	void WebContextImpl::SetURI(const char* uri)
	{
		if(uri==NULL)
		{
			m_uri.clear();
		}
		else
		{
			m_uri = uri;
		}
	}

	const char* WebContextImpl::GetURI()
	{
		return m_uri.c_str();
	}

	const char*	WebContextImpl::GetCachePath()
	{
		return m_cache_path.c_str();
	}

	const char* WebContextImpl::GetCacheMapPath()
	{
		return m_cache_map_path.c_str();
	}

	const char*	WebContextImpl::GetCacheProtocolPath()
	{
		return m_cache_protocol_path.c_str();
	}

	const char* WebContextImpl::GetUploadPath()
	{
		return m_upload_path.c_str();
	}

	GConnection* WebContextImpl::GetConnection()
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		if(m_pConnection==NULL)
		{
			DataEngine* pEngine = NULL;
			DataEngineManager* pDataEngineManager = NULL;
			pDataEngineManager = augeGetDataEngineManagerInstance();
			pEngine = pDataEngineManager->GetEngine(m_data_engine.c_str());
			if(pEngine==NULL)
			{
				pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			}
			else
			{
				m_pConnection = pEngine->CreateConnection();
			}
		}
		if(m_pConnection==NULL)
		{
			return NULL;
		}
		if(!m_pConnection->IsOpen())
		{
			m_pConnection->SetConnectionString(m_data_uri.c_str());
			m_pConnection->Open();
		}
		return m_pConnection;
	}

	const char* WebContextImpl::GetResponseEncoding()
	{
		return m_response_encoding.c_str();
	}

	const char*	WebContextImpl::GetServer()
	{
		const char* val = getenv("SERVER_NAME");
		return (val==NULL) ? "" : val;
	}

	const char*	WebContextImpl::GetPort()
	{
		const char* val = getenv("SERVER_PORT");
		return (val==NULL) ? "" : val;
	}

	const char*	WebContextImpl::GetScrpitName()
	{
		const char* val = getenv("SCRIPT_NAME");
		return (val==NULL) ? "" : val;
	}

	const char*	WebContextImpl::GetServerSoftware()
	{
		const char* val = getenv("SERVER_SOFTWARE");
		return (val==NULL) ? "" : val;
	} 

	const char*	WebContextImpl::GetHttpUserAgent()
	{
		const char* val = getenv("HTTP_USER_AGENT");
		return (val==NULL) ? "" : val;
	}

	const char* WebContextImpl::GetRequestMethod()
	{
		const char* val = getenv("REQUEST_METHOD");
		return (val==NULL) ? "" : val;
	}

	const char* WebContextImpl::GetQueryString()
	{
		const char* val = getenv("QUERY_STRING");
		return (val==NULL) ? "" : val;
	}

	void WebContextImpl::InitializeCache()
	{
		//char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		char cache_home[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		auge_make_path(cache_home, NULL, cdir, "cache", NULL);
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(cache_home, NULL, pdir, "cache", NULL);
#endif
		m_cache_path = cache_home;
		if(g_access(cache_home, 4))
		{
			auge_mkdir(cache_home);
		}

		char cache_protocol[AUGE_PATH_MAX];
		auge_make_path(cache_protocol, NULL, cache_home, "protocol", NULL);
		if(g_access(cache_protocol, 4))
		{
			auge_mkdir(cache_protocol);
		}
		m_cache_protocol_path = cache_protocol;

		char cache_map[AUGE_PATH_MAX];
		auge_make_path(cache_map, NULL, cache_home, "map", NULL);
		if(g_access(cache_map, 4))
		{
			auge_mkdir(cache_map);
		}
		m_cache_map_path = cache_map;

		char upload_path[AUGE_PATH_MAX];
#ifdef WIN32
		auge_make_path(upload_path, NULL, cdir, "upload", NULL);
#else
		auge_make_path(upload_path, NULL, pdir, "upload", NULL);
#endif 
		if(g_access(upload_path, 4))
		{
			auge_mkdir(upload_path); 
		}
		m_upload_path = upload_path;		
	}
}