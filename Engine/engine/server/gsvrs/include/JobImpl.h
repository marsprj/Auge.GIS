#ifndef __AUGE_JOB_IMPL_H__
#define __AUGE_JOB_IMPL_H__

#include "AugeService.h"
#include <string>

namespace auge
{
	class JobImpl : public Job
	{
	public:
		JobImpl();
		virtual ~JobImpl();
	public:
		virtual const char* GetUUID();

		virtual const char* GetUser();
		virtual const char* GetClient();
		virtual const char* GetServer();

		virtual const char*	GetOperation();	
		virtual const char*	GetParams();		
		virtual bool		GetStartTime(TIME_STRU& time);
		virtual bool		GetEndTime(TIME_STRU& time);
		virtual	bool		IsFinished();

	private:
		std::string		m_user; 
		std::string		m_uuid;
		std::string		m_client;
		std::string		m_server;
		std::string		m_operation;
		std::string		m_params;
		TIME_STRU		m_start;
		TIME_STRU		m_end;
	};
}

#endif //__AUGE_JOB_IMPL_H__
