#ifndef __AUGE_RESULT_SET_PGS_H__
#define __AUGE_RESULT_SET_PGS_H__

#include "AugeData.h"
#include <libpq-fe.h>

namespace auge
{
	class ConnectionPgs;

	class ResultSetPgs : public GResultSet
	{
	public:
		ResultSetPgs();
		virtual ~ResultSetPgs();

	public:
		virtual	g_uint			GetCount();
		virtual g_int			GetInt(g_uint row, g_uint col);
		virtual const char*		GetString(g_uint row, g_uint col);
		virtual void			Release();

	public:
		void	Create(ConnectionPgs* pConnection, PGresult* pgResult);

	private:
		ConnectionPgs	*m_pConnection;
		PGresult		*m_pgResult;
		g_uint			m_count;
	};
}

#endif //__AUGE_RESULT_SET_PGS_H__
