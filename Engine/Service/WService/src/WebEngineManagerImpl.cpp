#include "WebEngineManagerImpl.h"
#include "AugeCore.h"

namespace auge
{
	WebEngineManager* augeGetWebEngineManagerInstance()
	{
		static WebEngineManagerImpl g_svsEngineManager;
		return &g_svsEngineManager;
	}

	WebEngineManagerImpl::WebEngineManagerImpl()
	{

	}

	WebEngineManagerImpl::~WebEngineManagerImpl()
	{
		Unload();
	}

	g_uint WebEngineManagerImpl::GetCount()
	{
		return 0;
	}

	WebEngine* WebEngineManagerImpl::GetEngine(const char* type)
	{
		if(type==NULL)
		{
			return NULL;
		}
		WebEngine* pEngine = NULL;
		std::vector<WebEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = *iter;
			if(g_stricmp(pEngine->GetType(), type)==0)
			{
				return pEngine;
			}
		}
		return NULL;
	}

	WebEngine* WebEngineManagerImpl::Register(const char* path)
	{
		if(path==NULL)
		{
			return NULL;
		}

		WebEngine* pEngine = LoadEngine(path);
		if(pEngine!=NULL)
		{
			m_engines.push_back(pEngine);
			Save();
		}
		return pEngine;
	}

	RESULTCODE WebEngineManagerImpl::Unregister(const char* id)
	{
		return AG_SUCCESS;
	}

	RESULTCODE WebEngineManagerImpl::Save()
	{
		XDocument *pxDoc = new XDocument();
		if(pxDoc==NULL)
		{
			return AG_FAILURE;
		}

		XElement *pxNode = NULL;
		XElement *pxRoot = NULL;
		pxRoot = pxDoc->CreateRootNode("Engines", NULL, NULL);

		WebEngine* pEngine = NULL;
		std::vector<WebEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = *iter;
			pxNode = pxRoot->AddChild("Engine", NULL);
			pxNode->SetAttribute("description", pEngine->GetDescription(), NULL);
			pxNode->SetAttribute("path", "", NULL);
			pxNode->SetAttribute("id", pEngine->GetID(), NULL);
		}

		pxDoc->Save(m_path.c_str());
		pxDoc->Release();

		return AG_SUCCESS;
	}

	WebEngine* WebEngineManagerImpl::Find(const char* id)
	{
		if(id==NULL)
		{
			return NULL;
		}

		WebEngine* pEngine = NULL;
		std::vector<WebEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = (*iter);
			if(strcmp(pEngine->GetID(), id)==0)
			{
				return pEngine;
			}
		}

		return NULL;
	}

	WebEngine*	WebEngineManagerImpl::LoadEngine(const char* path)
	{
		GError	*pError  = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		void* handler = NULL;
		handler = auge_load_library(path);
		if(handler==NULL)
		{
			pLogger->Error(path, __FILE__, __LINE__);
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return NULL;
		}

		typedef WebEngine* (*FF)();
		FF fp = NULL;
		fp = (FF)auge_get_function_pointer(handler, "augeGetWebEngineInstance");
		if(fp==NULL)
		{
			pLogger->Error(path, __FILE__, __LINE__);
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			auge_free_library(handler);
			return NULL;
		}

		WebEngine* pEngine = (*fp)();
		if(GetEngine(pEngine->GetID())!=NULL)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "[%s] already registed.", pEngine->GetID());
			GError	*pError  = augeGetErrorInstance();
			pError->SetError(msg);
			return NULL;
		}

		pEngine->SetHandler(handler);
		return pEngine;
	}

	WebEngine*	WebEngineManagerImpl::LoadEngine(XNode* pxNode)
	{	
		XElement* pxElmt = dynamic_cast<XElement*>(pxNode);

		XAttribute* pxID	= pxElmt->GetAttribute("id");
		XAttribute* pxPath	= pxElmt->GetAttribute("path");

		const char* id = NULL;
		const char* path = NULL;
		if(pxID!=NULL)
		{
			id = pxID->GetContent();
		}
		if(pxPath!=NULL)
		{
			path = pxPath->GetContent();
		}

		if(id==NULL)
		{
			return NULL; 
		}

		char libpath[AUGE_PATH_MAX] = {0};
		MakeLibraryPath(libpath, path, id);

		WebEngine* pEngine = NULL;
		pEngine = LoadEngine(libpath);
		if(pEngine!=NULL)
		{
			m_engines.push_back(pEngine);
		}

		return pEngine;
	}

	RESULTCODE WebEngineManagerImpl::UnloadEngine(WebEngine* pEngine)
	{
		return AG_SUCCESS;
	}

	RESULTCODE WebEngineManagerImpl::Load(const char* config_path/*=NULL*/)
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
		XNodeSet* pxNodeSet = NULL;
		pxRoot = pxDoc->GetRootNode();
		if(pxRoot!=NULL)
		{
			pxNodeSet = pxRoot->GetChildren();
			if(pxNodeSet!=NULL)
			{
				XNode* pxNode = NULL;
				WebEngine* pEngine = NULL;
				int count = pxNodeSet->Count();
				for(int i=0; i<count; i++)
				{
					pxNode = pxNodeSet->Item(i);
					if(pxNode!=NULL)
					{
						pEngine = LoadEngine(pxNode);
					}
				}
			}
		}
		pxDoc->Close();
		pxDoc->Release();

		return AG_SUCCESS;
	}

	void WebEngineManagerImpl::Unload()
	{
		WebEngine* pEngine = NULL;
		std::vector<WebEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = *iter;
			// some bug here, unknown reason!!!!
			//auge_free_library(pEngine->GetHandler());
		}
		m_engines.clear();
	}

	void WebEngineManagerImpl::MakeConfigPath()
	{
		char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		auge_make_path(path, NULL, cdir, "conf\\web_engine", "xml");
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(path, NULL, pdir, "conf/web_engine", "xml");
#endif
		m_path = path;
	}

	void WebEngineManagerImpl::MakeLibraryPath(char* libpath, const char* dir, const char* id)
	{
		char lib_dir[AUGE_PATH_MAX] = {0};
		if((dir!=NULL)&&(strlen(dir)>0))
		{
			strcpy(lib_dir, dir);
		}
		else
		{
			auge_get_cwd(lib_dir, AUGE_PATH_MAX);
		}

#ifdef WIN32
#	ifdef _DEBUG
		char sid[AUGE_NAME_MAX];
		g_sprintf(sid, "%sD", id);
		auge_make_path(libpath, NULL, lib_dir, sid, "dll");
#	else
		auge_make_path(libpath, NULL, lib_dir, id, "dll");
#	endif
#else
		char pdir[AUGE_PATH_MAX] = {0};
		char sdir[AUGE_PATH_MAX] = {0};
		if(auge_get_parent_dir(pdir, lib_dir, AUGE_PATH_MAX))
		{
			auge_make_path(sdir, NULL, pdir, "lib", NULL);
			auge_make_path(libpath, NULL, sdir, id, "so");
		}
#endif

	}
}
