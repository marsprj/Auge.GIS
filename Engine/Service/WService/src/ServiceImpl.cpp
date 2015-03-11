#include "ServiceImpl.h"
#include "AugeCore.h"

namespace auge
{
	ServiceImpl::ServiceImpl()
	{
		m_engines = "wms;wfs;wps";
		m_pContext = augeGetWebContextInstance();
		m_pMap = NULL;
	}

	ServiceImpl::~ServiceImpl()
	{
		//AUGE_SAFE_RELEASE(m_pMap);
		//AUGE_SAFE_RELEASE(m_pContext);
	}

	const char*	ServiceImpl::GetName()
	{
		return m_name.c_str();
	}

	void ServiceImpl::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
	}

	WebResponse* ServiceImpl::Execute(WebRequest* pRequest)
	{
		const char* engine = pRequest->GetEngine();
		if(!Support(engine))
		{
			char msg[AUGE_NAME_MAX];
			g_sprintf(msg, "Service [%s] doesn't support Action [%s].", m_name.c_str(), engine);
			
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);

			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		WebEngine* pWebEngine = NULL;
		WebEngineManager* pWebEngingManager = NULL;
		pWebEngingManager = augeGetWebEngineManagerInstance();

		pWebEngine = pWebEngingManager->GetEngine(engine);
		if(pWebEngine==NULL)
		{
			GError* pError = augeGetErrorInstance();
			const char* msg = pError->GetLastError();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}
		
		WebContext* pWebContext = augeGetWebContextInstance();
		return pWebEngine->Execute(pRequest, pWebContext, GetMap());
	}

	void ServiceImpl::Release()
	{
		if(ReleaseRef()==0)
		{
			delete this;
		}
	}

	bool ServiceImpl::Support(const char* engine)
	{
		if(engine==NULL)
		{
			return false;
		}

		std::size_t pos = m_engines.find(engine);
		return (pos!=std::string::npos);
	}

	void ServiceImpl::SetMapName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		m_name = name;
		if(m_pMap!=NULL)
		{
			if(strcmp(m_name.c_str(),m_pMap->GetName()))
			{
				m_pMap = NULL;
			}
		}
	}

	WebContext*	ServiceImpl::GetWebContext()
	{
		return m_pContext;
	}

	void ServiceImpl::SetMap(Map* pMap)
	{
		m_pMap = pMap;
	}

	Map* ServiceImpl::GetMap()
	{
		return m_pMap;
	}

	RESULTCODE ServiceImpl::Start()
	{
		return AG_SUCCESS;
	}

	RESULTCODE ServiceImpl::Shutdown()
	{
		return AG_SUCCESS;
	}
}
