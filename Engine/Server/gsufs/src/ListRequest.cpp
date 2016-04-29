#include "ListRequest.h"

namespace auge
{
	ListRequest::ListRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
		//m_encoding = "UTF-8";
	}

	ListRequest::~ListRequest()
	{

	}

	bool ListRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		SetEncoding(cgi["encoding"]);
		return true;
	}

	const char*	ListRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	ListRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	ListRequest::GetRequest()
	{
		return "List";
	}

	void ListRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	ListRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void ListRequest::SetPath(const char* path)
	{
		if(path==NULL)
		{
			m_path.clear();
		}
		else
		{
			m_path = path;
		}
	}

	const char* ListRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	ListRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void ListRequest::SetEncoding(const char* encoding)
	{
		if(encoding==NULL)
		{
			m_encoding = "UTF-8";
		}
		else if(strlen(encoding)==0)
		{
			m_encoding = "UTF-8";
		}
		else
		{
			m_encoding = encoding;
		}
	}
}