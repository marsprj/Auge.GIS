#include "GetDataSetRequest.h"

namespace auge
{
	GetDataSetRequest::GetDataSetRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	GetDataSetRequest::~GetDataSetRequest()
	{

	}

	bool GetDataSetRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetDataSetName(cgi["dataSetName"]);
		return true;
	}

	const char*	GetDataSetRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetDataSetRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetDataSetRequest::GetRequest()
	{
		return "GetDataSet";
	}

	void GetDataSetRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetDataSetRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetDataSetRequest::SetSourceName(const char* name)
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

	const char*	GetDataSetRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void GetDataSetRequest::SetDataSetName(const char* name)
	{
		if(name==NULL)
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

	const char*	GetDataSetRequest::GetDataSetName()
	{
		if(m_dataset_name.empty())
		{
			return NULL;
		}
		return m_dataset_name.c_str();
	}

}