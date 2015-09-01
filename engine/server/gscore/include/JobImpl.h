#ifndef __AUGE_JOB_IMPL_H__
#define __AUGE_JOB_IMPL_H__

#include "AugeWebCore.h"
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
		virtual bool		GetStartTime(TIME_STRU& start);
		virtual bool		GetEndTime(TIME_STRU& end);
		virtual	bool		IsFinished();

	public:
		void SetUUID(const char* uuid);
		void SetUser(const char* user);
		void SetClient(const char* client);
		void SetServer(const char* server);

		void SetOperation(const char* operation);	
		void SetParams(const char* params);		
		void SetStartTime(TIME_STRU& start);
		void SetEndTime(TIME_STRU& end);

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
