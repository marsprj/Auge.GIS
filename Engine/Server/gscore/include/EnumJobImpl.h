#ifndef __AUGE_ENUM_JOB_IMPL_H__
#define __AUGE_ENUM_JOB_IMPL_H__

#include "AugeWebCore.h"
#include <vector>

namespace auge
{
	class EnumJobImpl : public EnumJob
	{
	public:
		EnumJobImpl();
		virtual ~EnumJobImpl();
	public:
		virtual Job*		Next();
		virtual void		Reset();
		virtual void		Release();

	public:
		void	Add(Job* pJob);

	private:
		void	Cleanup();

	private:
		std::vector<Job*>	m_jobs;
		std::vector<Job*>::iterator m_iter;
	};
}

#endif //__AUGE_ENUM_JOB_IMPL_H__
