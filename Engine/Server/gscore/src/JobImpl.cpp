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

	bool JobImpl::GetStartTime(TIME_STRU* start)
	{
		if(start!=NULL)
		{
			memcpy(start, &m_start, sizeof(TIME_STRU));
		}
		return true;
	}

	bool JobImpl::GetEndTime(TIME_STRU* end)
	{
		if(end!=NULL)
		{
			memcpy(end, &m_end, sizeof(TIME_STRU));
		}
		return true;
	}

	bool JobImpl::IsFinished()
	{
		return true;
	}

	void JobImpl::SetUser(const char* user)
	{
		if(user==NULL)
		{
			m_user.clear();
		}
		m_user = user;
	}

	void JobImpl::SetUUID(const char* uuid)
	{
		if(uuid!=NULL)
		{
			m_uuid.clear();
		}
		m_uuid = uuid;
	}

	void JobImpl::SetClient(const char* client)
	{
		if(client==NULL)
		{
			m_client.clear();
		}
		m_client = client;
	}

	void JobImpl::SetServer(const char* server)
	{
		if(server!=NULL)
		{
			m_server.clear();
		}
		m_server = server;
	}

	void JobImpl::SetOperation(const char* operation)
	{
		if(operation!=NULL)
		{
			m_operation.clear();
		}
		m_operation = operation;
	}

	void JobImpl::SetParams(const char* params)
	{
		if(params!=NULL)
		{
			m_params.clear();
		}
		m_params = params;
	}

	void JobImpl::SetStartTime(TIME_STRU* start)
	{
		if(start!=NULL)
		{
			memcpy(&m_start, start, sizeof(TIME_STRU));
		}
	}

	void JobImpl::SetEndTime(TIME_STRU* end)
	{
		if(end!=NULL)
		{
			memcpy(&m_end, end, sizeof(TIME_STRU));
		}
	}

	augeProcssState JobImpl::GetState()
	{
		return m_state;
	}

	void JobImpl::SetState(g_uint state)
	{
		m_state = (augeProcssState)state;
	}

	void JobImpl::SetService(const char* service)
	{
		if(service==NULL)
		{
			m_service = "";
		}
		else
		{
			m_service = service;
		}
	}

	const char* JobImpl::GetService()
	{
		return m_service.c_str();
	}
}