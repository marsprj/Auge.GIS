#include "EnumJobImpl.h"

namespace auge
{
	EnumJobImpl::EnumJobImpl()
	{
		m_iter = m_jobs.begin();
	}

	EnumJobImpl::~EnumJobImpl()
	{
		Cleanup();
	}

	void EnumJobImpl::Reset()
	{
		m_iter = m_jobs.begin();
	}

	Job* EnumJobImpl::Next()
	{
		if(m_iter==m_jobs.end())
		{
			return NULL;
		}
		return *(m_iter++);
	}

	void EnumJobImpl::Add(Job* pJob)
	{
		if(pJob!=NULL)
		{
			m_jobs.push_back(pJob);
		}
	}

	void EnumJobImpl::Cleanup()
	{
		Job* pJob = NULL;
		std::vector<Job*>::iterator iter;
		for(iter=m_jobs.begin(); iter!=m_jobs.end(); iter++)
		{
			pJob = *iter;
			if(pJob!=NULL)
			{
				pJob->Release();
			}
		}
		m_jobs.clear();
	}

	void EnumJobImpl::Release()
	{
		delete this;
	}
}