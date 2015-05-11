#include "ConnectionMy.h"

namespace auge
{
	ConnectionMy::ConnectionMy()
	{
		m_myConnection = NULL;
	}

	ConnectionMy::~ConnectionMy()
	{
		Close();
	}

	RESULTCODE ConnectionMy::SetConnectionString(const char* conn_string)
	{
		if(conn_string==NULL)
		{
			return AG_FAILURE;
		}
		m_connstring = conn_string;
		m_props.Parse(conn_string);		
		return AG_SUCCESS;
	}

	const char*	ConnectionMy::GetConnectionString()
	{
		return m_connstring.c_str();
	}

	const char*	ConnectionMy::GetDatabase()
	{
		return m_props.GetValue("DATABASE");
	}

	RESULTCODE ConnectionMy::Open()
	{
		if(m_myConnection!=NULL)
		{
			return AG_FAILURE;
		}

		const char* szServer   = m_props.GetValue(AUGE_DB_SERVER);
		const char* szInstance = m_props.GetValue(AUGE_DB_INSTANCE);
		const char* szDatabase = m_props.GetValue(AUGE_DB_DATABASE);
		const char* szUser     = m_props.GetValue(AUGE_DB_USER);
		const char* szPassword = m_props.GetValue(AUGE_DB_PASSWORD);
		const char* szEncoding = m_props.GetValue(AUGE_DB_ENCODING);
		if(szEncoding!=NULL)	m_client_encoding = szEncoding;

		if(szServer==NULL||szDatabase==NULL||szUser==NULL||szPassword==NULL)
		{
			return AG_FAILURE;
		}

		m_myConnection = mysql_init((MYSQL*) 0); 
		if(m_myConnection==NULL)
		{
			return AG_FAILURE;
		}

		mysql_options(m_myConnection,MYSQL_SET_CHARSET_NAME,"gbk");  
		if(!mysql_real_connect(m_myConnection,szServer,szUser,szPassword,szDatabase,atoi(szInstance),NULL,0))
		{
			return AG_FAILURE;
		}

		return AG_SUCCESS;
	}

	void ConnectionMy::Close()
	{
		if(m_myConnection!=NULL)
		{
			mysql_close(m_myConnection);
			m_myConnection = NULL;
		}
	}

	bool ConnectionMy::IsOpen()
	{
		return (m_myConnection!=NULL);
	}

	GResultSet*	ConnectionMy::ExecuteQuery(const char* sql)
	{
		return NULL;
	}

	RESULTCODE ConnectionMy::ExecuteSQL(const char* sql)
	{
		return AG_SUCCESS;
	}

	bool ConnectionMy::HasTable(const char* name)
	{
		return true;
	}

	void ConnectionMy::Release()
	{

	}

}