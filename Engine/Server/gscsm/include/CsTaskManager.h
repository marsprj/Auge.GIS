#ifndef __CSTASK_MANAGER_H__
#define __CSTASK_MANAGER_H__

#include "AugeCore.h"

namespace auge
{
	class CsTask;
	class EnumTask;
	class GConnection;

	class TaskManager : public GObject
	{
	public:
		TaskManager();
		virtual ~TaskManager();
	public:
		RESULTCODE	Initialize(GConnection* pConnection);

		RESULTCODE	CreateTask(const char* name, const char* description, g_uint mapID, g_uint owner);
		RESULTCODE	Join(g_uint task, g_uint user, bool isowner);
		bool		Joined(g_uint task, g_uint user);

		CsTask*		GetTask(g_uint tid);
		CsTask*		GetTask(g_uint user, const char* task);
		EnumTask*	GetTask(g_uint user_id, g_int role);

	private:
		GConnection	*m_pConnection;
	};

	extern "C"
	{
		TaskManager* augeGetTaskManagerInstance();
	}
}

#endif //__CSTASK_MANAGER_H__
