#include "RemoveTileStoreRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	RemoveTileStoreRequest::RemoveTileStoreRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	RemoveTileStoreRequest::~RemoveTileStoreRequest()
	{

	}

	bool RemoveTileStoreRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetStoreName(cgi["storeName"]);
		return true;
	}

	const char*	RemoveTileStoreRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	RemoveTileStoreRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveTileStoreRequest::GetRequest()
	{
		return "RemoveTileStore";
	}

	void RemoveTileStoreRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveTileStoreRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RemoveTileStoreRequest::SetSourceName(const char* name)
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

	const char*	RemoveTileStoreRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void RemoveTileStoreRequest::SetStoreName(const char* name)
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

	const char*	RemoveTileStoreRequest::GetStoreName()
	{
		if(m_store_name.empty())
		{
			return NULL;
		}
		return m_store_name.c_str();
	}

	const char* RemoveTileStoreRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}