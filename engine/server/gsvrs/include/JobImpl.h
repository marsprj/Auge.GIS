#ifndef __AUGE_JOB_MANAGER_IMPL_H__
#define __AUGE_JOB_MANAGER_IMPL_H__

#include "AugeService.h"

namespace auge
{
	class JobManagerImpl : public JobManager
	{
	public:
		JobManagerImpl();
		virtual ~JobManagerImpl();
	public:
		virtual	Job*			AddJob(const char* user, const char* operation, const char* params, const char* client, const char* server);
		virtual RESULTCODE		GetJob(const char* uuid);
		virtual RESULTCODE		SetEndTime(const char* uuid);

		virtual RESULTCODE		Initialize(GConnection* pConnection);
	};
}

#endif //__AUGE_JOB_MANAGER_IMPL_H__
