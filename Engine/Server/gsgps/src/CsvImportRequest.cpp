#include "CsvImportRequest.h"

namespace auge
{
	CsvImportRequest::CsvImportRequest()
	{
		m_srid = 4326;
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	CsvImportRequest::~CsvImportRequest()
	{

	}

	const char*	CsvImportRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	const char*	CsvImportRequest::GetUser()
	{
		return m_user.empty() ? NULL : m_user.c_str();
	}

	const char*	CsvImportRequest::GetEngine()
	{
		return "gps";
	}

	const char*	CsvImportRequest::GetVersion()
	{
		return m_user.empty() ? NULL : m_user.c_str();
	}

	const char*	CsvImportRequest::GetRequest()
	{
		return "CsvImport";
	}

	const char*	CsvImportRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	const char*	CsvImportRequest::GetCsvPath()
	{
		return m_csv_path.empty() ? NULL : m_csv_path.c_str();
	}

	const char*	CsvImportRequest::GetCsvName()
	{
		return m_csv_name.empty() ? NULL : m_csv_name.c_str();
	}

	const char* CsvImportRequest::GetTypeName()
	{
		return m_type_name.empty() ? NULL : m_type_name.c_str();
	}

	g_uint CsvImportRequest::GetSrid()
	{
		return m_srid;
	}

	void CsvImportRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	void CsvImportRequest::SetTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_type_name.clear();
		}
		else
		{
			m_type_name = typeName;
		}
	}

	void CsvImportRequest::SetCsvPath(const char* path)
	{
		if(path==NULL)
		{
			m_csv_path.clear();
		}
		else
		{
			m_csv_path = path;
		}
	}

	void CsvImportRequest::SetCsvName(const char* name)
	{
		if(name==NULL)
		{
			m_csv_name.clear();
		}
		else
		{
			m_csv_name = name;
		}
	}

	void CsvImportRequest::SetSrid(const char* srid)
	{
		if(srid==NULL)
		{
			m_srid = 4326;
		}
		else
		{
			if(isdigit(srid[0]))
			{
				m_srid = atoi(srid);
			}
			else
			{
				m_srid = 4326;
			}
		}
	}

	bool CsvImportRequest::Create(rude::CGI& cgi)
	{
		SetSourceName(cgi["sourceName"]);
		SetTypeName(cgi["typeName"]);
		SetCsvPath(cgi["csvpath"]);
		SetCsvName(cgi["csvname"]);
		SetSrid(cgi["srid"]);
		return true;
	}
}