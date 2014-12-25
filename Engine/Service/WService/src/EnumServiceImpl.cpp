#include "EnumServiceImpl.h"

namespace auge
{
	EnumServiceImpl::EnumServiceImpl()
	{
		m_iter = m_services.begin();
	}

	EnumServiceImpl::~EnumServiceImpl()
	{
		Service* pService = NULL;
		std::vector<Service*>::iterator iter;
		for(iter=m_services.begin(); iter!=m_services.end(); iter++)
		{
			pService = *iter;
			if(pService!=NULL)
			{
				pService->Release();
			}
		}
		m_services.clear();
	}

	Service* EnumServiceImpl::Next()
	{
		if(m_iter==m_services.end())
		{
			return NULL;
		}
		Service* pService = *m_iter;
		m_iter++;
		return pService;
	}

	void EnumServiceImpl::Add(Service* pService)
	{
		if(pService!=NULL)
		{
			m_services.push_back(pService);
			pService->AddRef();
		}
	}

	void EnumServiceImpl::Reset()
	{
		m_iter = m_services.begin();
	}
}