#ifndef __AUGE_CONNECTION_PGS_H__
#define __AUGE_CONNECTION_PGS_H__

#include "AugeFeature.h"
#include "AugeCore.h"
#include <libpq-fe.h>

namespace auge
{
	class ConnectionPgs : public GConnection
	{
	public:
		ConnectionPgs();
		virtual ~ConnectionPgs();
	public:
		virtual	RESULTCODE		SetConnectionString(const char* conn_string);

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		virtual GResultSet*		ExecuteQuery(const char* sql);
		virtual RESULTCODE		ExecuteSQL(const char* sql);

		virtual bool			HasTable(const char* name);

		virtual void			Release();

	public:
		PGresult*		PgExecute(const char* sql);
		Oid				GetPgTypeOid(const char* type);
		Oid				GetPgTableOid(const char* name);

	public:
		augeFieldType	GetFieldType(Oid oid);

	private:
		PGconn			*m_pgConnection;
		std::string		m_client_encoding;
		g_int			m_pg_version;
		g_int			m_pg_protocol_version;

		Oid				m_geom_oid;

		std::string		m_connstring;
		GPropertySet	m_props;
	};

}

#endif //__AUGE_CONNECTION_PGS_H__