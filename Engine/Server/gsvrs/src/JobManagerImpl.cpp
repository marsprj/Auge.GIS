#include "JobManagerImpl.h"

namespace auge
{
	JobManagerImpl::JobManagerImpl()
	{

	}

	JobManagerImpl::~JobManagerImpl()
	{

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
		return AG_SUCCESS;
	}
}