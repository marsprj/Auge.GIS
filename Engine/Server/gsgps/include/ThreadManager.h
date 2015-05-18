#ifndef __AUGE_THREAD_MANAGER_H__
#define __AUGE_THREAD_MANAGER_H__

#include "spthread.h"
#include "threadpool.h"

namespace auge
{
	class ThreadManager
	{
	public:
		ThreadManager();
		virtual ~ThreadManager();
	public:
		static ThreadManager*	GetInstance();
	private:
		static ThreadManager*	m_instance;
	public:
		void	Dispatch(dispatch_fn process_func, void *arg);
	private:
		threadpool	m_pool;
		int			m_size;
	};
}

#endif //__AUGE_THREAD_MANAGER_H__
