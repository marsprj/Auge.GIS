#include "RefreshDataSetRequest.h"

namespace auge
{
	RefreshDatasetRequest::RefreshDatasetRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	RefreshDatasetRequest::~RefreshDatasetRequest()
	{

	}

	bool RefreshDatasetRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetDataSetName(cgi["dataSetName"]);
		return true;
	}

	const char*	RefreshDatasetRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	RefreshDatasetRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RefreshDatasetRequest::GetRequest()
	{
		return "Refresh";
	}

	void RefreshDatasetRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RefreshDatasetRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RefreshDatasetRequest::SetSourceName(const char* name)
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

	const char*	RefreshDatasetRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void RefreshDatasetRequest::SetDataSetName(const char* name)
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

	const char*	RefreshDatasetRequest::RefreshDatasetName()
	{
		if(m_dataset_name.empty())
		{
			return NULL;
		}
		return m_dataset_name.c_str();
	}

	const char* RefreshDatasetRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}