#include "EnumTask.h"
#include "CsTask.h"

namespace auge
{
	EnumTask::EnumTask()
	{

	}

	EnumTask::~EnumTask()
	{
		Cleanup();
	}

	void EnumTask::Add(CsTask* pTask)
	{
		if(pTask!=NULL)
		{
			m_tasks.push_back(pTask);
		}
	}

	void EnumTask::Reset()
	{
		m_iter = m_tasks.begin();
	}

	CsTask* EnumTask::Next()
	{
		if(m_iter==m_tasks.end())
		{
			return NULL;
		}
		return (*m_iter++);
	}

	void EnumTask::Cleanup()
	{
		std::vector<CsTask*>::iterator iter;
		for(iter=m_tasks.begin(); iter!=m_tasks.end(); iter++)
		{
			CsTask* pTask = *iter;
			pTask->Release();
		}
		m_tasks.clear();
	}
}