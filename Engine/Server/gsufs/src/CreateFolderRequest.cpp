#include "CreateFolderRequest.h"

namespace auge
{
	CreateFolderRequest::CreateFolderRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	CreateFolderRequest::~CreateFolderRequest()
	{

	}

	bool CreateFolderRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		return true;
	}

	const char*	CreateFolderRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	CreateFolderRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateFolderRequest::GetRequest()
	{
		return "CreateFolder";
	}

	void CreateFolderRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CreateFolderRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void CreateFolderRequest::SetPath(const char* path)
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

	const char* CreateFolderRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	CreateFolderRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

}