#include "ConnectionPgs.h"
#include "ResultSetPgs.h"
#include "AugeCore.h"

namespace auge
{
	ConnectionPgs::ConnectionPgs():
	m_pgConnection(NULL),
	m_geom_oid(-1),
	m_client_encoding("GBK")
	{

	}

	ConnectionPgs::~ConnectionPgs()
	{
		Close();
	}

	RESULTCODE ConnectionPgs::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}
		m_connstring = conn_string;
		m_props.Parse(conn_string);		
		return AG_SUCCESS;
	}

	const char*	ConnectionPgs::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	const char*	ConnectionPgs::GetDatabase()
	{
		if(m_pgConnection==NULL)
		{
			return NULL;
		}
		return PQdb(m_pgConnection);
	}

	RESULTCODE ConnectionPgs::Open()
	{
		if(m_pgConnection!=NULL)
		{
			return AG_FAILURE;
		}

		const char* szServer   = m_props.GetValue(AUGE_DB_SERVER);
		const char* szInstance = m_props.GetValue(AUGE_DB_INSTANCE);
		const char* szDatabase = m_props.GetValue(AUGE_DB_DATABASE);
		const char* szUser     = m_props.GetValue(AUGE_DB_USER);
		const char* szPassword = m_props.GetValue(AUGE_DB_PASSWORD);
		const char* szEncoding = m_props.GetValue(AUGE_DB_ENCODING);
		//if(szEncoding!=NULL)	m_client_encoding = szEncoding;

		if(szServer==NULL||szDatabase==NULL||szUser==NULL||szPassword==NULL)
		{
			return AG_FAILURE;
		}

		ConnStatusType result;
		char constr[AUGE_NAME_MAX] = {0};
		g_sprintf(constr, "hostaddr=%s port=%s dbname=%s user=%s password=%s", szServer, szInstance, szDatabase, szUser, szPassword);
		m_pgConnection = PQconnectdb(constr);
		result = PQstatus(m_pgConnection);
		if(result != CONNECTION_OK)
		{
			const char* errmsg = PQerrorMessage(m_pgConnection);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(errmsg, __FILE__, __LINE__);

			GError* pError = augeGetErrorInstance();
			pError->SetError(errmsg);

			PQfinish(m_pgConnection);
			m_pgConnection = NULL;
			return AG_FAILURE;
		}
		PQsetClientEncoding(m_pgConnection, m_client_encoding.c_str());
		m_geom_oid = GetPgTypeOid("geometry");

		return AG_SUCCESS;
	}

	void ConnectionPgs::Close()
	{
		if(m_pgConnection!=NULL)
		{
			PQfinish(m_pgConnection);
			m_pgConnection = NULL;
		}
	}

	bool ConnectionPgs::IsOpen()
	{
		if(m_pgConnection==NULL)
		{
			return false;
		}
		ConnStatusType status = PQstatus(m_pgConnection);
		return (status == CONNECTION_OK);
	}

	GResultSet*	ConnectionPgs::ExecuteQuery(const char* sql)
	{
		PGresult *pgResult = NULL;	
		GLogger* pLogger = augeGetLoggerInstance();
		pLogger->Debug(sql, __FILE__, __LINE__);

		g_ulong ts = auge_get_time();

		pgResult = PQexec(m_pgConnection, sql);

		g_ulong te = auge_get_time();
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg, "[SQL Query]:%ld ms",te-ts);
		pLogger->Debug(msg, __FILE__, __LINE__);

		int status = PQresultStatus(pgResult);
		if((status!=PGRES_TUPLES_OK))
		{
			const char* errmsg = PQresultErrorMessage(pgResult);

			GError* pError = augeGetErrorInstance();
			pError->SetError(errmsg);

			pLogger->Error(errmsg, __FILE__, __LINE__);

			PQclear(pgResult);
			return NULL;
		}

		ResultSetPgs *pResultSet = new ResultSetPgs();
		pResultSet->Create(this, pgResult);
		return pResultSet;
	}

	RESULTCODE ConnectionPgs::ExecuteSQL(const char* sql)
	{
		if(sql==NULL)
		{
			return AG_FAILURE;
		}

		g_ulong ts = auge_get_time();

		PGresult* pgResult = PgExecute(sql);

		g_ulong te = auge_get_time();
		char msg[AUGE_MSG_MAX];
		g_sprintf(msg, "[SQL Execute]:%ld ms",te-ts);
		augeGetLoggerInstance()->Debug(msg, __FILE__, __LINE__);

		if(pgResult==NULL)
		{
			return AG_FAILURE;
		}
		PQclear(pgResult);
		return AG_SUCCESS;
	}


	void ConnectionPgs::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	PGresult* ConnectionPgs::PgExecute(const char* sql)
	{
		PGresult *pgResult = NULL;	
		GLogger* pLogger = augeGetLoggerInstance();
		//pLogger->Debug(sql, __FILE__, __LINE__);

		pgResult = PQexec(m_pgConnection, sql);

		int status = PQresultStatus(pgResult);
		if((status!=PGRES_TUPLES_OK)&&(status!=PGRES_COMMAND_OK))
		{
			const char* errmsg = PQresultErrorMessage(pgResult);

			GError* pError = augeGetErrorInstance();
			pError->SetError(errmsg);

			pLogger->Error(errmsg, __FILE__, __LINE__);

			PQclear(pgResult);
			return NULL;
		}

		return pgResult;
	}

	bool ConnectionPgs::HasTable(const char* name)
	{
		if(name==NULL)
		{
			return false;
		}

		char sql[AUGE_NAME_MAX] = {0};
		g_sprintf(sql, "select count(*) from pg_class where relname='%s'", name);

		PGresult* pgResult = NULL;
		pgResult = PgExecute(sql);

		if(pgResult==NULL)
		{
			return false;
		}

		int count = atoi(PQgetvalue(pgResult, 0, 0));
		PQclear(pgResult);

		return (count>0);
	}

	//////////////////////////////////////////////////////////////////////////
	Oid	ConnectionPgs::GetPgTypeOid(const char* type)
	{
		char sql[AUGE_NAME_MAX] = {0};
		g_snprintf(sql, AUGE_NAME_MAX, "select oid from pg_type where typname='%s'", type);
		PGresult* pgResult = NULL;
		pgResult = PgExecute(sql);
		if(pgResult==NULL)
		{
			return -1;
		}
		
		if(PQntuples(pgResult)==0)
		{
			return -1;
		}
		Oid oid = atoi(PQgetvalue(pgResult, 0 ,0));
		PQclear(pgResult);
		return oid;
	}

	Oid	ConnectionPgs::GetPgTableOid(const char* name)
	{
		char sql[AUGE_NAME_MAX] = {0};
		g_snprintf(sql, AUGE_NAME_MAX, "select oid from pg_class where relname='%s'", name);
		PGresult* pgResult = NULL;
		pgResult = PgExecute(sql);
		if(pgResult==NULL)
		{
			return -1;
		}

		if(PQntuples(pgResult)==0)
		{
			return -1;
		}
		Oid oid = atoi(PQgetvalue(pgResult, 0 ,0));
		PQclear(pgResult);
		return oid;
	}

	augeFieldType ConnectionPgs::GetFieldType(Oid oid)
	{
		if(oid==m_geom_oid)
		{
			return augeFieldTypeGeometry;
		}

		augeFieldType ftype = augeFieldTypeNone;
		switch(oid)
		{
		case 17:
			ftype = augeFieldTypeBLOB;
			break;
		case 20:
			ftype = augeFieldTypeInt;
			break;
		case 23:
			ftype = augeFieldTypeInt;
			break;
		case 700:
			ftype = augeFieldTypeFloat;
			break;
		case 701:
			ftype = augeFieldTypeDouble;
			break;
		case 1042:
		case 1043:
			ftype = augeFieldTypeString;
			break;
		case 1114:
			ftype = augeFieldTypeTime;
			break;
			// 	case 16400:
			// 	case 16408:
			// 	case 16402:
			// 	case 16416:
			// 	case 16857:
			// 	case 16890:
			// 	case 17195:
			// 	case 93496:
			// 		ftype = augeFieldTypeGeometry;
			// 		break;
		default:
			ftype = augeFieldTypeNone;
		}

		return ftype;
	}
}
