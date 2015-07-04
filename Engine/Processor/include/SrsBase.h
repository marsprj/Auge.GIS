#ifndef __AUGE_SRS_BASE_H__
#define __AUGE_SRS_BASE_H__

#include "AugeType.h"
#include "sqlite3.h"

namespace auge
{
	class SrsBase
	{
	public:
		SrsBase();
		virtual ~SrsBase();
	public:
		RESULTCODE	Open();
		void		Close();

		RESULTCODE	GetProj4Text(g_uint srid, char* text, size_t size);
		RESULTCODE	GetSrsText(g_uint srid, char* text, size_t size);

	private:
		sqlite3* m_psqlite;
	};
}

#endif //__AUGE_SRS_BASE_H__
