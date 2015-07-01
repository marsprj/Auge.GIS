#include "RemoveFileRequest.h"

namespace auge
{
	RemoveFileRequest::RemoveFileRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	RemoveFileRequest::~RemoveFileRequest()
	{

	}

	bool RemoveFileRequest::Create(rude::CGI& cgi) 
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		return true;
	}

	const char*	RemoveFileRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	RemoveFileRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveFileRequest::GetRequest()
	{
		return "RemoveFile";
	}

	void RemoveFileRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveFileRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RemoveFileRequest::SetPath(const char* path)
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

	const char* RemoveFileRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	RemoveFileRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}