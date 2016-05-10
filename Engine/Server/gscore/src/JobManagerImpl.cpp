#include "JobImpl.h"
#include "JobManagerImpl.h"
#include "EnumJobImpl.h"
#include "AugeData.h"
#include "AugeUser.h"

namespace auge
{
	#define AUGE_JOB_TABLE	"g_job"

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

	Job* JobManagerImpl::AddJob(g_uint user, const char* service, const char* operation, const char* params, const char* client, const char* server)
	{
		const char* format = "insert into %s (uuid,service, operation,params,user_id,client,server,start_time,state) values('%s','%s','%s','%s',%d,'%s','%s','%s',%d)";
		//const char* format = "insert into %s (uuid,operation,params,user_id,client,server) values('%s','%s','%s',%d,'%s','%s')";

		char uuid[AUGE_PATH_MAX];
		memset(uuid, 0, AUGE_PATH_MAX);
		auge_generate_uuid(uuid, AUGE_PATH_MAX);
		//const char* service, 
		TIME_STRU start_time, end_time;
		auge_get_time_struct(&start_time);
		char time_str[AUGE_NAME_MAX];
		memset(time_str, 0, AUGE_NAME_MAX);
		//g_snprintf(time_str,AUGE_NAME_MAX,"%d-%02d-%02d %02d:%02d:%02d.%03d",start_time.usYear,
		//																start_time.usMonth,
		//																start_time.usDay,
		//																start_time.usHour,
		//																start_time.usMinute,
		//																start_time.usSecond,
		//																start_time.usMilliseconds);
		g_snprintf(time_str,AUGE_NAME_MAX,"%d-%02d-%02d %02d:%02d:%02d",start_time.usYear,
			start_time.usMonth,
			start_time.usDay,
			start_time.usHour,
			start_time.usMinute,
			start_time.usSecond);

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, AUGE_JOB_TABLE, uuid, service, operation, params,user, client, server,time_str,0);

		RESULTCODE rc = m_pConnection->ExecuteSQL(sql);
		if(rc!=AG_SUCCESS)
		{
			return NULL;
		}

		JobImpl* pJob = new JobImpl();
		pJob->SetUUID(uuid);
		pJob->SetClient(client);
		pJob->SetServer(server);
		pJob->SetOperation(operation);
		pJob->SetParams(params);

		return pJob;
	}

	Job* JobManagerImpl::AddJob(const char* user, const char* service, const char* operation, const char* params, const char* client, const char* server)
	{
		//const char* format = "insert into %s (uuid,operation,params,user_id,client,server,start_time,end_time,state) values('%s','%s','%s',%d,'%s','%s','%s','%s',%d) returning gid";
		const char* format = "insert into %s (uuid,service,operation,params,user_id,client,server) values('%s','%s','%s','%s',%d,'%s','%s') returning gid";

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		//g_snprintf(sql, AUGE_SQL_MAX, format, );

		return NULL;
	}

	Job* JobManagerImpl::GetJob(const char* uuid)
	{
		if(uuid==NULL)
		{
			return NULL;
		}

		const char* format = "select gid,uuid,service,operation,params,user_id,client,server,start_time,end_time,state from %s where uuid='%s'";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, AUGE_JOB_TABLE, uuid);

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult==NULL)
		{
			return NULL;
		}

		if(pResult->GetCount()==0)
		{
			pResult->Release();
			return NULL;
		}

		Job* pJob = CreateJob(pResult, 0);

		pResult->Release();
		return pJob;
	}

	EnumJob* JobManagerImpl::GetJob(User* pUser, augeProcssState state ,g_int maxJobs/*=20*/, g_uint offset/*=0*/)
	{
		const char* format = "select gid,uuid,service, operation,params,user_id,client,server,start_time,end_time,state from %s where user_id=%d and state=%d limit %d offset %d";
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, AUGE_JOB_TABLE, pUser->GetID(), state, maxJobs, offset);

		Job* pJob = NULL;
		EnumJobImpl* pJobs = new EnumJobImpl();

		GResultSet* pResult = NULL;
		pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = pResult->GetCount();
			for(g_uint i=0; i<count; i++)
			{
				pJob = CreateJob(pResult, i);
				if(pJob!=NULL)
				{
					pJobs->Add(pJob);
				}
			}

			pResult->Release();
		}

		return pJobs;
	}

	Job* JobManagerImpl::CreateJob(GResultSet* pResult, g_uint i)
	{
		g_uint gid = pResult->GetInt(i, 0);
		const char* uuid = pResult->GetString(i,1);
		const char* operation = pResult->GetString(i, 2);
		const char* service = pResult->GetString(i, 3);
		const char* params = pResult->GetString(i, 4);
		g_uint user_id = pResult->GetInt(i, 5);
		const char* client = pResult->GetString(i, 7);
		const char* server = pResult->GetString(i, 8);
		const char* start = pResult->GetString(i, 9);
		const char* end = pResult->GetString(i, 0);
		g_uint state = pResult->GetInt(i, 10);
		TIME_STRU tim;

		JobImpl* pJob = new JobImpl();
		pJob->SetUUID(uuid);
		pJob->SetOperation(operation);
		pJob->SetService(service);
		pJob->SetParams(params);
		pJob->SetClient(client);
		pJob->SetServer(server);
		pJob->SetState(state);

		memset(&tim,0, sizeof(TIME_STRU));
		sscanf(start,"%d-%2d-%2d %2d:%2d:%2d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond));
		//memset(&tim,0, sizeof(TIME_STRU));
		//sscanf(start,"%d-%2d-%2d %2d:%2d:%2d.%d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond),&(tim.usMilliseconds));
		pJob->SetStartTime(&tim);

		memset(&tim,0, sizeof(TIME_STRU));
		sscanf(end,"%d-%2d-%2d %2d:%2d:%2d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond));
		//memset(&tim,0, sizeof(TIME_STRU));
		//sscanf(end,"%d-%2d-%2d %2d:%2d:%2d.%d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond),&(tim.usMilliseconds));
		//sscanf(end,"%d-%2d-%2d %2d:%2d:%2d.%3d",&(tim.usYear),&(tim.usMonth),&(tim.usDay),&(tim.usHour),&(tim.usMinute),&(tim.usSecond),&(tim.usMilliseconds));
		pJob->SetEndTime(&tim);

		return pJob;
	}

	RESULTCODE JobManagerImpl::SetEndTime(const char* uuid)
	{
		const char* format = "update %s set end_time='%s',state=1 where uuid='%s'";

		TIME_STRU start_time, end_time;
		auge_get_time_struct(&start_time);
		char time_str[AUGE_NAME_MAX];
		memset(time_str, 0, AUGE_NAME_MAX);
		//g_snprintf(time_str,AUGE_NAME_MAX,"%d-%02d-%02d %02d:%02d:%02d.%03d",start_time.usYear,
		//	start_time.usMonth,
		//	start_time.usDay,
		//	start_time.usHour,
		//	start_time.usMinute,
		//	start_time.usSecond,
		//	start_time.usMilliseconds);

		g_snprintf(time_str,AUGE_NAME_MAX,"%d-%02d-%02d %02d:%02d:%02d",start_time.usYear,
			start_time.usMonth,
			start_time.usDay,
			start_time.usHour,
			start_time.usMinute,
			start_time.usSecond);
		
		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, AUGE_JOB_TABLE, time_str, uuid);

		return m_pConnection->ExecuteSQL(sql);
	}

	RESULTCODE JobManagerImpl::Initialize(GConnection* pConnection)
	{
		m_pConnection = pConnection;
		if(m_pConnection==NULL)
		{
			return AG_FAILURE;
		}
		if(!m_pConnection->IsOpen())
		{
			return AG_FAILURE;
		}

		if(!m_pConnection->HasTable(AUGE_JOB_TABLE))
		{
			CreateJobTable();
		}

		return AG_SUCCESS;
	}

	RESULTCODE JobManagerImpl::Unload()
	{
		return AG_FAILURE;
	}

	GStatistics* JobManagerImpl::Statistics(const char* field)
	{
		if(field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job group by %s", field, field);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}

		return pStatistics;
	}

	GStatistics* JobManagerImpl::Statistics(const char* field, const char* start_time, const char* end_time)
	{
		if(field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where start_time>'%s' and start_time<'%s' group by %s", field, start_time, end_time, field);
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}

		return pStatistics;
	}

	GStatistics* JobManagerImpl::Statistics(g_int user_id, const char* field)
	{
		if(field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		if(user_id<1)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job group by %s", field, field);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where user_id=%d group by %s", field, user_id, field);
		}
		
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}

		return pStatistics;
	}

	GStatistics* JobManagerImpl::Statistics(g_int user_id, const char* field, const char* start_time, const char* end_time)
	{
		if(field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		if(user_id<=0)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where start_time>'%s' and start_time<'%s' group by %s", field, start_time, end_time, field);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where user_id=%d and start_time>'%s' and start_time<'%s' group by %s", field, user_id, start_time, end_time, field);
		}
		
		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}
		return pStatistics;
	}

	GStatistics* JobManagerImpl::StatisticsByClient(g_int user_id, const char* client, const char* field)
	{
		if(client==NULL||field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		if(user_id<1)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where client='%s' group by %s", field, client, field);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where user_id=%d and client='%s' group by %s", field, user_id, client, field);
		}

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}

		return pStatistics;
	}

	GStatistics* JobManagerImpl::StatisticsByClient(g_int user_id, const char* client, const char* field, const char* start_time, const char* end_time)
	{
		if(client==NULL||field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		if(user_id<=0)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where (client='%s' and start_time>'%s' and start_time<'%s') group by %s", field, client, start_time, end_time, field);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where (client='%s' and user_id=%d and start_time>'%s' and start_time<'%s') group by %s", field, client, user_id, start_time, end_time, field);
		}

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}
		return pStatistics;
	}

	GStatistics* JobManagerImpl::StatisticsByServer(g_int user_id, const char* server, const char* field)
	{
		if(server==NULL||field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		if(user_id<1)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where server='%s' group by %s", field, server, field);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where user_id=%d and server='%s' group by %s", field, user_id, server, field);
		}

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}

		return pStatistics;
	}

	GStatistics* JobManagerImpl::StatisticsByServer(g_int user_id, const char* server, const char* field, const char* start_time, const char* end_time)
	{
		if(server==NULL||field==NULL)
		{
			return NULL;
		}

		GStatistics* pStatistics = new GStatistics();

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		if(user_id<=0)
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where (server='%s' and start_time>'%s' and start_time<'%s') group by %s", field, server, start_time, end_time, field);
		}
		else
		{
			g_snprintf(sql, AUGE_SQL_MAX, "select %s, count(*) from g_job where (server='%s' and user_id=%d and start_time>'%s' and start_time<'%s') group by %s", field, server, user_id, start_time, end_time, field);
		}

		GResultSet* pResult = m_pConnection->ExecuteQuery(sql);
		if(pResult!=NULL)
		{
			g_uint count = 0;
			g_uint item_count = 0;
			const char* item = NULL;

			count = pResult->GetCount();
			pStatistics->SetCount(count);

			for(g_uint i=0; i<count; i++)
			{
				item = pResult->GetString(i, 0);
				item_count = pResult->GetInt(i, 1);
				pStatistics->SetValue(i,item, item_count);
			}

			pResult->Release();
		}

		return pStatistics;
	}

	RESULTCODE JobManagerImpl::CreateJobTable()
	{
		const char* sql = "CREATE TABLE g_job " \
			"( " \
			"  gid serial NOT NULL, " \
			"  uuid character varying(128), " \
			"  operation character varying(64), " \
			"  params character varying(1024), " \
			"  user_id integer, " \
			"  client character varying(32), " \
			"  server character varying(32), " \
			"  start_time timestamp without time zone, " \
			"  end_time timestamp without time zone, " \
			"  state integer DEFAULT 0, " \
			"  CONSTRAINT g_job_gid_pk PRIMARY KEY (gid) " \
			")";

		return m_pConnection->ExecuteSQL(sql);
	}
}