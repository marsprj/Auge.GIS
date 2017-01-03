#include "EnumWebEngineImpl.h"

namespace auge
{
	EnumWebEngineImpl::EnumWebEngineImpl()
	{
		m_iter = m_engines.begin();
	}

	EnumWebEngineImpl::~EnumWebEngineImpl()
	{
		WebEngine* pWebEngine = NULL;
		std::vector<WebEngine*>::iterator iter;
		for(iter=m_engines.begin(); iter!=m_engines.end(); iter++)
		{
			pWebEngine = *iter;
			if(pWebEngine!=NULL)
			{
				pWebEngine->Release();
			}
		}
		m_engines.clear();
	}

	WebEngine* EnumWebEngineImpl::Next()
	{
		if(m_iter==m_engines.end())
		{
			return NULL;
		}
		WebEngine* pWebEngine = *m_iter;
		m_iter++;
		return pWebEngine;
	}

	void EnumWebEngineImpl::Add(WebEngine* pWebEngine)
	{
		if(pWebEngine!=NULL)
		{
			m_engines.push_back(pWebEngine);
			pWebEngine->AddRef();
		}
	}

	void EnumWebEngineImpl::Reset()
	{
		m_iter = m_engines.begin();
	}

	WebEngine* EnumWebEngineImpl::GetEngine(g_uint i)
	{
		if(i>=m_engines.size())
		{
			return NULL;
		}
	}

	WebEngine* EnumWebEngineImpl::GetEngine(const char* type)
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

	WebEngine* EnumWebEngineImpl::GetEngineById(const char* id)
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

	g_uint EnumWebEngineImpl::GetCount()
	{
		return m_engines.size();
	}
}