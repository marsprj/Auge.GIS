#include "TestConnectionRequest.h"

namespace auge
{
	TryConnectionRequest::TryConnectionRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	TryConnectionRequest::~TryConnectionRequest()
	{

	}

	bool TryConnectionRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetDataEngine(cgi["engine"]);
		SetURI(cgi["uri"]);
		return true;
	}

	const char*	TryConnectionRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	TryConnectionRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	TryConnectionRequest::GetRequest()
	{
		return "TryConnection";
	}

	const char* TryConnectionRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void TryConnectionRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	TryConnectionRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}
		
	const char*	TryConnectionRequest::GetDataEngine()
	{
		return m_data_engine.c_str();
	}

	const char* TryConnectionRequest::GetURI()
	{
		return m_uri.c_str();
	}

	void TryConnectionRequest::SetDataEngine(const char* engine)
	{
		if(engine==NULL)
		{
			m_data_engine.clear();
		}
		else
		{
			m_data_engine = engine;
		}
	}

	void TryConnectionRequest::SetURI(const char* uri)
	{
		if(uri==NULL)
		{
			m_uri.clear();
		}
		else
		{
			m_uri = uri;
		}
	}
}