#ifndef __AUGE_ROW_IMPL_H__
#define __AUGE_ROW_IMPL_H__

#include "AugeData.h"

namespace auge
{
	class DataSetCsv;

	class RowCsv : public Row
	{
	public:
		RowCsv();
		virtual ~RowCsv();
	public:
		//virtual GValue*				GetValue(g_uint i)		const;
		virtual bool				GetBool(g_uint i)		const;
		virtual char				GetChar(g_uint i)		const;
		virtual short				GetShort(g_uint i)		const;
		virtual int					GetInt(g_uint i)		const;
		virtual long				GetLong(g_uint i)		const;
		virtual float				GetFloat(g_uint i)		const;
		virtual double				GetDouble(g_uint i)		const;
		virtual int64				GetInt64(g_uint i)		const;
		virtual const char*			GetString(g_uint i)		const;
		//virtual const AgString*	GetString(g_uint i)		const;
		//virtual const AgBlob*	GetBlob(g_uint i)		const;
		//virtual const TIME_STRU*GetTime(g_uint i)		const;

		//virtual GValue*				GetValue(const char* name)		const;
		virtual bool				GetBool(const char* name)		const;
		virtual char				GetChar(const char* name)		const;
		virtual short				GetShort(const char* name)		const;
		virtual int					GetInt(const char* name)		const;
		virtual long				GetLong(const char* name)		const;
		virtual float				GetFloat(const char* name)		const;
		virtual double				GetDouble(const char* name)		const;
		virtual int64				GetInt64(const char* name)		const;
		virtual const char*			GetString(const char* name)		const;

		virtual void				Release();

	public:
		bool	Create(const char* line, DataSetCsv* pDataset);

	private:
		DataSetCsv*	m_pDataset;
		char*		m_line;
		char**		m_values;
		g_uint		m_field_count;

	};
}

#endif //__AUGE_ROW_IMPL_H__
