#include "ThreadManager.h"

namespace auge
{
	ThreadManager* ThreadManager::m_instance = NULL;

	ThreadManager::ThreadManager()
	{
		m_size = 20;
		m_pool = create_threadpool(m_size);
	}

	ThreadManager::~ThreadManager()
	{
		destroy_threadpool(m_pool);
	}

	ThreadManager* ThreadManager::GetInstance()
	{
		if(m_instance==NULL)
		{
			m_instance = new ThreadManager();
		}
		return m_instance;
	}

	void ThreadManager::Dispatch(dispatch_fn process_func, void *arg)
	{
		
	}
}