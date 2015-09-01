#include "JobImpl.h"

namespace auge
{
	JobImpl::JobImpl()
	{

	}

	JobImpl::~JobImpl()
	{

	}

	const char* JobImpl::GetUUID()
	{
		return m_uuid.c_str();
	}

	const char* JobImpl::GetUser()
	{
		return m_user.c_str();
	}

	const char* JobImpl::GetClient()
	{
		return m_client.c_str();
	}

	const char* JobImpl::GetServer()
	{
		return m_server.c_str();
	}

	const char*	JobImpl::GetOperation()
	{
		return m_operation.c_str();
	}

	const char*	JobImpl::GetParams()
	{
		return m_params.c_str();
	}

	bool JobImpl::GetStartTime(TIME_STRU& time)
	{
		return true;
	}

	bool JobImpl::GetEndTime(TIME_STRU& time)
	{
		return true;
	}

	bool JobImpl::IsFinished()
	{
		return true;
	}
}