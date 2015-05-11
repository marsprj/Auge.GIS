#ifndef __AUGE_CONNECTION_MY_H__
#define __AUGE_CONNECTION_MY_H__

#include "AugeFeature.h"
#include "AugeCore.h"
#include "mysql.h"

namespace auge
{
	class ConnectionMy : public GConnection
	{
	public:
		ConnectionMy();
		virtual ~ConnectionMy();
	public:
		virtual	RESULTCODE		SetConnectionString(const char* conn_string);
		virtual const char*		GetConnectionString();
		virtual	const char*		GetDatabase();

		virtual RESULTCODE		Open();
		virtual void			Close();
		virtual bool			IsOpen();

		virtual GResultSet*		ExecuteQuery(const char* sql);
		virtual RESULTCODE		ExecuteSQL(const char* sql);

		virtual bool			HasTable(const char* name);

		virtual void			Release();

	private:
		MYSQL*			m_myConnection;
		std::string		m_client_encoding;
		std::string		m_connstring;
		GPropertySet	m_props;
	};

}

#endif //__AUGE_CONNECTION_MY_H__