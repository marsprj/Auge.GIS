#ifndef __AUGE_JOB_MANAGER_IMPL_H__
#define __AUGE_JOB_MANAGER_IMPL_H__

#include "AugeWebCore.h"

namespace auge
{
	class JobManagerImpl : public JobManager
	{
	public:
		JobManagerImpl();
		virtual ~JobManagerImpl();
	public:
		virtual	Job*			AddJob(g_uint user, const char* operation, const char* params, const char* client, const char* server);
		virtual	Job*			AddJob(const char* user, const char* operation, const char* params, const char* client, const char* server);
		virtual RESULTCODE		GetJob(const char* uuid);
		virtual RESULTCODE		SetEndTime(const char* uuid);

		virtual RESULTCODE		Initialize(GConnection* pConnection);
		virtual RESULTCODE		Unload();

	private:
		RESULTCODE	CreateJobTable();

	private:
		GConnection*	m_pConnection;
	};
}

#endif //__AUGE_JOB_MANAGER_IMPL_H__
