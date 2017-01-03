#ifndef __CSTASK_ENUM_H__
#define __CSTASK_ENUM_H__

#include <vector>
#include "AugeCore.h"

namespace auge
{
	class CsTask;

	class EnumTask : public GObject
	{
	public:
		EnumTask();
		virtual ~EnumTask();
		
	public:
		void		Reset();
		CsTask*		Next();

	public:
		void		Add(CsTask* pTask);

	private:
		void		Cleanup();

	private:
		std::vector<CsTask*>	m_tasks;
		std::vector<CsTask*>::iterator m_iter;
	};
}

#endif //__CSTASK_ENUM_H__
