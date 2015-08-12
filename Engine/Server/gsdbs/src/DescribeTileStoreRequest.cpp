#include "DescribeTileStoreRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	DescribeTileStoreRequest::DescribeTileStoreRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	DescribeTileStoreRequest::~DescribeTileStoreRequest()
	{

	}

	bool DescribeTileStoreRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetStoreName(cgi["storeName"]);
		return true;
	}

	const char*	DescribeTileStoreRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	DescribeTileStoreRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeTileStoreRequest::GetRequest()
	{
		return "DescribeTileStore";
	}

	void DescribeTileStoreRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeTileStoreRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void DescribeTileStoreRequest::SetSourceName(const char* name)
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
			//m_source_name = name;
			WebContext* pWebContext = augeGetWebContextInstance();
			m_source_name = pWebContext->ParameterEncoding(name);
		}

	}

	const char*	DescribeTileStoreRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void DescribeTileStoreRequest::SetStoreName(const char* name)
	{
		if(name==NULL||(strlen(name)==0))
		{
			m_store_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_store_name.clear();
		}
		else
		{
			//m_store_name = name;
			WebContext* pWebContext = augeGetWebContextInstance();
			m_store_name = pWebContext->ParameterEncoding(name);
		}

	}

	const char*	DescribeTileStoreRequest::GetStoreName()
	{
		if(m_store_name.empty())
		{
			return NULL;
		}
		return m_store_name.c_str();
	}

	const char* DescribeTileStoreRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}