#include "CsTaskManager.h"
#include "CsTask.h"
#include "EnumTask.h"
#include "AugeData.h"

namespace auge
{
	TaskManager* augeGetTaskManagerInstance()
	{
		static TaskManager g_csTaskManager;
		return &g_csTaskManager;
	}

	TaskManager::TaskManager():
	m_pConnection(NULL)
	{

	}

	TaskManager::~TaskManager()
	{

	}

	RESULTCODE TaskManager::Initialize(GConnection* pConnection)
	{
		if(pConnection==NULL)
		{
			return AG_FAILURE;
		}

		m_pConnection = pConnection;
		return AG_SUCCESS;
	}

	RESULTCODE TaskManager::CreateTask(const char* name, const char* description, g_uint mapID, g_uint owner)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		g_snprintf(sql, AUGE_SQL_MAX, "insert into cs_task(name, description, map_id, owner, create_time) values('%s','%s',%d, %d,now()) returning gid", name, description, mapID, owner);
		GResultSet* rst = m_pConnection->ExecuteQuery(sql);

		if(rst==NULL)
		{
			return AG_FAILURE;
		}

		g_uint tid = rst->GetInt(0,0);
		rst->Release();

		return tid;
	}

	CsTask*	 TaskManager::GetTask(g_uint tid)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select name, description, map_id, owner, create_time from cs_task where gid=%d", tid);

		GResultSet* rst = m_pConnection->ExecuteQuery(sql);
		if(rst==NULL)
		{
			return NULL;
		}
		if(!rst->GetCount())
		{
			rst->Release();
			return NULL;
		}

		const char* name = rst->GetString(0,0);
		const char* desc = rst->GetString(0,1);
		g_int		mapid= rst->GetInt(0,2);
		g_int		owner= rst->GetInt(0,3);

		CsTask* pTask = new CsTask();
		pTask->Create(tid, name, desc, mapid, owner);		
		rst->Release();

		return pTask;
	}

	EnumTask* TaskManager::GetTask(g_uint user_id, g_int role)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		if(role<0)
		{
			//g_snprintf(sql, AUGE_SQL_MAX, "select gid, name, description, map_id, create_time from cs_task where owner=%d", user_id);
			g_snprintf(sql, AUGE_SQL_MAX, "select t.gid, t.name, t.description, t.map_id, t.owner, t.create_time from cs_task t,cs_task_join j where j.user_id=%d and j.task_id=t.gid", user_id);
		}
		else
		{
			//g_snprintf(sql, AUGE_SQL_MAX, "select gid, name, description, map_id, create_time from cs_task where owner=%d and owner=%s", user_id, role?"false":"true");
			g_snprintf(sql, AUGE_SQL_MAX, "select t.gid, t.name, t.description, t.map_id, t.owner, t.create_time from cs_task t,cs_task_join j where j.user_id=%d and j.task_id=t.gid and j.isowner=%s", user_id,role?"false":"true");
		}
		

		EnumTask* pTasks = new EnumTask();

		GResultSet* rst = m_pConnection->ExecuteQuery(sql);
		if(rst==NULL)
		{
			return pTasks;
		}

		g_uint count = rst->GetCount();
		for(g_uint i=0; i<count; i++)
		{
			g_int		tid  = rst->GetInt(i, 0);
			const char* name = rst->GetString(i, 1);
			const char* desc = rst->GetString(i, 2);
			g_int		mapid= rst->GetInt(i, 3);
			g_uint		owner= rst->GetInt(i, 4);

			CsTask* pTask = new CsTask();
			pTask->Create(tid, name, desc, mapid, owner);

			pTasks->Add(pTask);
		}
		
		rst->Release();

		return pTasks;	
	}

	CsTask* TaskManager::GetTask(g_uint user, const char* task)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select gid, description, map_id, owner, create_time from cs_task where owner=%d and name='%s'", user, task);

		GResultSet* rst = m_pConnection->ExecuteQuery(sql);
		if(rst==NULL)
		{
			return NULL;
		}

		if(!rst->GetCount())
		{
			rst->Release();
			return NULL;
		}

		g_int        tid = rst->GetInt(0,0);
		const char* desc = rst->GetString(0,1);
		g_int		mapid= rst->GetInt(0,2);
		g_int		owner= rst->GetInt(0,3);

		CsTask* pTask = new CsTask();
		pTask->Create(tid, task, desc, mapid, owner);		
		rst->Release();

		return pTask;
	}

	EnumTask* TaskManager::GetTask()
	{
		EnumTask* pTasks = new EnumTask();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select gid, description, map_id, owner, create_time from cs_task order by gid desc limit 20 offset 0");

		GResultSet* rst = m_pConnection->ExecuteQuery(sql);
		if(rst==NULL)
		{
			return pTasks;
		}

		g_uint count = rst->GetCount();
		for(g_uint i=0; i<count; i++)
		{
			g_int		tid  = rst->GetInt(i, 0);
			const char* name = rst->GetString(i, 1);
			const char* desc = rst->GetString(i, 2);
			g_int		mapid= rst->GetInt(i, 3);
			g_uint		owner= rst->GetInt(i, 4);

			CsTask* pTask = new CsTask();
			pTask->Create(tid, name, desc, mapid, owner);

			pTasks->Add(pTask);
		}

		rst->Release();

		return pTasks;	
	}

	bool TaskManager::Joined(g_uint task, g_uint user)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select count(*) from cs_task where user_id=%d and task_id=%d", user, task);

		GResultSet* rst = m_pConnection->ExecuteQuery(sql);
		if(rst==NULL)
		{
			return false;
		}

		g_uint count = rst->GetCount();		
		rst->Release();

		return (count>0);	
	}

	RESULTCODE	TaskManager::Join(g_uint task, g_uint user, bool isowner)
	{
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "insert into cs_task_join (task_id, user_id, isowner) values(%d,%d,%s)", task, user, isowner?"true":"false");

		return m_pConnection->ExecuteSQL(sql);
	}
}
