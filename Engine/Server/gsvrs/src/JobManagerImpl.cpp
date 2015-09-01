#include "JobManagerImpl.h"
#include "AugeData.h"

namespace auge
{
	JobManager*	augeGetJobManagerInstance()
	{
		static JobManagerImpl g_jobManager;
		return &g_jobManager;
	}

	JobManagerImpl::JobManagerImpl()
	{
		m_pConnection = NULL;
	}

	JobManagerImpl::~JobManagerImpl()
	{
		m_pConnection = NULL;
	}

	Job* JobManagerImpl::AddJob(const char* user, const char* operation, const char* params, const char* client, const char* server)
	{
		return NULL;
	}

	RESULTCODE JobManagerImpl::GetJob(const char* uuid)
	{
		return AG_SUCCESS;
	}

	RESULTCODE JobManagerImpl::SetEndTime(const char* uuid)
	{
		return AG_SUCCESS;
	}

	RESULTCODE JobManagerImpl::Initialize(GConnection* pConnection)
	{
		if(m_pConnection==NULL)
		{
			return AG_FAILURE;
		}
		if(!m_pConnection->IsOpen())
		{
			return AG_FAILURE;
		}



		return AG_SUCCESS;
	}

	RESULTCODE JobManagerImpl::Unload()
	{
		return AG_FAILURE;
	}
}