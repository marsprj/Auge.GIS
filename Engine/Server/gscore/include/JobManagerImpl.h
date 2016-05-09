#ifndef __AUGE_JOB_MANAGER_IMPL_H__
#define __AUGE_JOB_MANAGER_IMPL_H__

#include "AugeWebCore.h"
#include "AugeData.h"

namespace auge
{
	class JobManagerImpl : public JobManager
	{
	public:
		JobManagerImpl();
		virtual ~JobManagerImpl();
	public:
		virtual	Job*			AddJob(g_uint user, const char* service, const char* operation, const char* params, const char* client, const char* server);
		virtual	Job*			AddJob(const char* user, const char* service, const char* operation, const char* params, const char* client, const char* server);		
		virtual RESULTCODE		SetEndTime(const char* uuid);

		virtual Job*			GetJob(const char* uuid);
		virtual EnumJob*		GetJob(User* pUser, augeProcssState state ,g_int maxJobs=20, g_uint offset=0);

		virtual RESULTCODE		Initialize(GConnection* pConnection);
		virtual RESULTCODE		Unload();

	private:
		RESULTCODE	CreateJobTable();
		Job*		CreateJob(GResultSet* pResult, g_uint i);

	private:
		GConnection*	m_pConnection;
	};
}

#endif //__AUGE_JOB_MANAGER_IMPL_H__
