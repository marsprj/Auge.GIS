#include "RowImpl.h"
#include "DataSetCvs.h"
#include "AugeField.h"

namespace auge
{
	RowCsv::RowCsv()
	{
		m_line = NULL;
		m_values = NULL;
	}

	RowCsv::~RowCsv()
	{
		if(m_line)
		{
			free(m_line);
			m_line = NULL;
		}
		if(m_values)
		{
			free(m_values);
			m_values = NULL;
		}
	}


	bool RowCsv::GetBool(g_uint i) const
	{
		return true;
	}

	char RowCsv::GetChar(g_uint i) const
	{
		return '0';
	}

	short RowCsv::GetShort(g_uint i) const
	{
		return 0;
	}

	int	RowCsv::GetInt(g_uint i) const
	{
		return 0;
	}

	long RowCsv::GetLong(g_uint i) const
	{
		return 0;
	}

	float RowCsv::GetFloat(g_uint i) const
	{
		return 0.0f;
	}

	double RowCsv::GetDouble(g_uint i) const
	{
		return 0.0f;
	}

	int64 RowCsv::GetInt64(g_uint i)	const
	{
		return 0;
	}

	const char*	RowCsv::GetString(g_uint i)	const
	{
		if(i>=m_field_count)
		{
			return NULL;
		}
		return m_values[i];
	}

	bool RowCsv::GetBool(const char* name) const
	{
		return true;
	}

	char RowCsv::GetChar(const char* name) const
	{
		return '0';
	}

	short RowCsv::GetShort(const char* name) const
	{
		return 0;
	}

	int	RowCsv::GetInt(const char* name) const
	{
		return 0;
	}

	long RowCsv::GetLong(const char* name) const
	{
		return 0;
	}

	float RowCsv::GetFloat(const char* name) const
	{
		return 0;
	}

	double RowCsv::GetDouble(const char* name) const
	{
		return 0;
	}

	int64 RowCsv::GetInt64(const char* name)	const
	{
		return 0;
	}

	const char*	RowCsv::GetString(const char* name)	const
	{
		if(name==NULL)
		{
			return NULL;
		}
		int findex = m_pDataset->GetFields()->FindField(name);
		if(findex<0)
		{
			return NULL;
		}
		return m_values[findex];
	}

	bool RowCsv::Create(const char* line, DataSetCsv* pDataset)
	{
		m_pDataset = pDataset;
		m_line = strdup(line);
		auge_str_trim_last_r_n(m_line);

		GFields* pFields = pDataset->GetFields();
		m_field_count = pFields->Count();
		
		m_values = (char**)malloc(sizeof(char*)*m_field_count);
		memset(m_values, 0, sizeof(char*)*m_field_count);

		g_uint index = 0;
		char* ptr = strtok(m_line, ",");
		while(ptr)
		{
			m_values[index++] = ptr;
			ptr = strtok(NULL, ",");
		}
		return true;
	}

	void RowCsv::Release()
	{
		if(ReleaseRef()<=0)
		{
			delete this;
		}
	}
}