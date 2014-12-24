#include "ResultSetPgs.h"
#include "ConnectionPgs.h"

namespace auge
{
	ResultSetPgs::ResultSetPgs():
	m_pgResult(NULL)
	{

	}

	ResultSetPgs::~ResultSetPgs()
	{
		if(m_pgResult!=NULL)
		{
			PQclear(m_pgResult);
			m_pgResult = NULL;
		}
	}

	g_uint ResultSetPgs::GetCount()
	{
		return m_count;
	}

	g_int ResultSetPgs::GetInt(g_uint row, g_uint col)
	{
		return atoi(PQgetvalue(m_pgResult,row, col));
	}

	const char*	ResultSetPgs::GetString(g_uint row, g_uint col)
	{
		return PQgetvalue(m_pgResult, row, col);
	}

	void ResultSetPgs::Create(ConnectionPgs* pConnection, PGresult* pgResult)
	{
		m_pConnection = pConnection;
		m_pgResult = pgResult;
		m_count = PQntuples(m_pgResult);
	}

	void ResultSetPgs::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}
}

