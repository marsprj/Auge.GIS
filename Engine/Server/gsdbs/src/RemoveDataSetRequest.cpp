#include "RemoveDataSetRequest.h"

namespace auge
{
	RemoveDataSetRequest::RemoveDataSetRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = "GBK";
	}

	RemoveDataSetRequest::~RemoveDataSetRequest()
	{

	}

	bool RemoveDataSetRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetDataSetName(cgi["dataSetName"]);
		return true;
	}

	const char*	RemoveDataSetRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	RemoveDataSetRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveDataSetRequest::GetRequest()
	{
		return "RemoveDataSet";
	}

	void RemoveDataSetRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveDataSetRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RemoveDataSetRequest::SetSourceName(const char* name)
	{
		if(name==NULL)
		{
			m_source_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = name;
		}

	}

	const char*	RemoveDataSetRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void RemoveDataSetRequest::SetDataSetName(const char* name)
	{
		if(name==NULL||(strlen(name)==0))
		{
			m_dataset_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_dataset_name.clear();
		}
		else
		{
			m_dataset_name = name;
		}

	}

	const char*	RemoveDataSetRequest::GetDataSetName()
	{
		if(m_dataset_name.empty())
		{
			return NULL;
		}
		return m_dataset_name.c_str();
	}

	const char* RemoveDataSetRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}