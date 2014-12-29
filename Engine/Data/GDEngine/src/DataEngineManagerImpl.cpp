#include "DataEngineManagerImpl.h"
#include "AugeCore.h"
#include "AugeXML.h"

namespace auge
{
	DataEngineManager*	augeGetDataEngineManagerInstance()
	{
		static DataEngineManagerImpl g_DataEngineManager;
		return &g_DataEngineManager;
	}

	DataEngineManagerImpl::DataEngineManagerImpl()
	{

	}

	DataEngineManagerImpl::~DataEngineManagerImpl()
	{
		Unload();
	}

	g_uint DataEngineManagerImpl::GetCount()
	{
		return m_engines.size();
	}

	DataEngine*	DataEngineManagerImpl::GetEngine(g_uint i)
	{
		if(i>=m_engines.size())
		{
			return NULL;
		}
		return m_engines[i];
	}

	DataEngine*	DataEngineManagerImpl::GetEngine(const char* id)
	{
		if(id==NULL)
		{
			return NULL;
		}
		DataEngine* pEngine = NULL;
		std::vector<DataEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = *iter;
			if(g_stricmp(pEngine->GetID(), id)==0)
			{
				return pEngine;
			}
		}
		return NULL;
	}

	DataEngine* DataEngineManagerImpl::Register(const char* path)
	{
		if(path==NULL)
		{
			return NULL;
		}
		
		DataEngine* pEngine = NULL;
		pEngine = LoadEngine(path);
		if(pEngine!=NULL)
		{
			m_engines.push_back(pEngine);
			Save();
		}
		return pEngine;
	}

	RESULTCODE DataEngineManagerImpl::Unregister(const char* id)
	{
		if(id==NULL)
		{
			return AG_FAILURE;
		}
		DataEngine* pEngine = NULL;
		std::vector<DataEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = *iter;
			if(g_stricmp(pEngine->GetID(), id)==0)
			{
				auge_free_library(pEngine->GetHandler());
				m_engines.erase(iter);
			}
		}
		if(iter==m_engines.end())
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "[%s] not registered", id);
			GError* pError = augeGetErrorInstance();
			pError->SetError(msg);
		}

		return AG_SUCCESS;
	}

	RESULTCODE DataEngineManagerImpl::Load(const char* config_path/*=NULL*/)
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
				DataEngine* pEngine = NULL;
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

	RESULTCODE DataEngineManagerImpl::Save()
	{
		XDocument *pxDoc = new XDocument();
		if(pxDoc==NULL)
		{
			return AG_FAILURE;
		}

		XElement *pxNode = NULL;
		XElement *pxRoot = NULL;
		pxRoot = pxDoc->CreateRootNode("Engines", NULL, NULL);
		
		DataEngine* pEngine = NULL;
		std::vector<DataEngine*>::iterator iter;
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
	
	void DataEngineManagerImpl::Unload()
	{
		DataEngine* pEngine = NULL;
		std::vector<DataEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pEngine = *iter;
			auge_free_library(pEngine->GetHandler());
		}
		m_engines.clear();
	}

	void DataEngineManagerImpl::MakeConfigPath()
	{
		char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		auge_make_path(path, NULL, cdir, "conf\\data_engine", "xml");
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(path, NULL, pdir, "conf/data_engine", "xml");
#endif
		m_path = path;
	}

	DataEngine*	DataEngineManagerImpl::LoadEngine(XNode* pxNode)
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

		DataEngine* pEngine = NULL;
		pEngine = LoadEngine(libpath);
		if(pEngine!=NULL)
		{
			m_engines.push_back(pEngine);
		}

		return pEngine;
	}

	DataEngine*	DataEngineManagerImpl::LoadEngine(const char* path)
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

		typedef DataEngine* (*FF)();
		FF fp = NULL;
		fp = (FF)auge_get_function_pointer(handler, "augeGetDataEngineInstance");
		if(fp==NULL)
		{
			pLogger->Error(path, __FILE__, __LINE__);
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			auge_free_library(handler);
			return NULL;
		}

		DataEngine* pEngine = (*fp)();
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

	void DataEngineManagerImpl::MakeLibraryPath(char* libpath, const char* dir, const char* id)
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
		char sid[AUGE_NAME_MAX];
#	ifdef _DEBUG
		g_sprintf(sid, "G%sD", id);
		auge_make_path(libpath, NULL, lib_dir, sid, "dll");
#	else
		
		g_sprintf(sid, "G%s", id);
		auge_make_path(libpath, NULL, lib_dir, sid, "dll");
#	endif
#else
		char pdir[AUGE_PATH_MAX] = {0};
		char sdir[AUGE_PATH_MAX] = {0};
		if(auge_get_parent_dir(pdir, lib_dir, AUGE_PATH_MAX))
		{
			char sid[AUGE_NAME_MAX]  = {0};
			g_sprintf(sid, "G%s", id);
			auge_make_path(sdir, NULL, pdir, "lib", NULL);
			auge_make_path(libpath, NULL, sdir, sid, "so");
		}
#endif

	}
}
