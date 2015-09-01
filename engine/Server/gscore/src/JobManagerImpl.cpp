#include "JobImpl.h"
#include "JobManagerImpl.h"
#include "AugeData.h"

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

	Job* JobManagerImpl::AddJob(g_uint user, const char* operation, const char* params, const char* client, const char* server)
	{
		//const char* format = "insert into %s (uuid,operation,params,user_id,client,server,start_time,end_time,state) values('%s','%s','%s',%d,'%s','%s','%s','%s',%d) returning gid";
		const char* format = "insert into %s (uuid,operation,params,user_id,client,server) values('%s','%s','%s',%d,'%s','%s')";

		char uuid[AUGE_PATH_MAX];
		memset(uuid, 0, AUGE_PATH_MAX);
		auge_generate_uuid(uuid, AUGE_PATH_MAX);

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);
		g_snprintf(sql, AUGE_SQL_MAX, format, AUGE_JOB_TABLE, uuid, operation, params,user, client, server);

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

	Job* JobManagerImpl::AddJob(const char* user, const char* operation, const char* params, const char* client, const char* server)
	{
		//const char* format = "insert into %s (uuid,operation,params,user_id,client,server,start_time,end_time,state) values('%s','%s','%s',%d,'%s','%s','%s','%s',%d) returning gid";
		const char* format = "insert into %s (uuid,operation,params,user_id,client,server) values('%s','%s','%s',%d,'%s','%s') returning gid";

		char sql[AUGE_SQL_MAX];
		memset(sql, 0, AUGE_SQL_MAX);

		//g_snprintf(sql, AUGE_SQL_MAX, format, );

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
			"  state integer, " \
			"  CONSTRAINT g_job_gid_pk PRIMARY KEY (gid) " \
			")";

		return m_pConnection->ExecuteSQL(sql);
	}
}