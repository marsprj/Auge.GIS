#include "RemoveFolderRequest.h"

namespace auge
{
	RemoveFolderRequest::RemoveFolderRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	RemoveFolderRequest::~RemoveFolderRequest()
	{

	}

	bool RemoveFolderRequest::Create(rude::CGI& cgi) 
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		return true;
	}

	const char*	RemoveFolderRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	RemoveFolderRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveFolderRequest::GetRequest()
	{
		return "RemoveFolder";
	}

	void RemoveFolderRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveFolderRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RemoveFolderRequest::SetPath(const char* path)
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

	const char* RemoveFolderRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	RemoveFolderRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}