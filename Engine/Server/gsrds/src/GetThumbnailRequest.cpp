#include "GetThumbnailRequest.h"

namespace auge
{
	GetThumbnailRequest::GetThumbnailRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
		m_refresh = false;
	}

	GetThumbnailRequest::~GetThumbnailRequest()
	{
		
	}

	bool GetThumbnailRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["Path"]);
		SetRasterName(cgi["rasterName"]);
		SetSourceName(cgi["sourceName"]);
		SetRefresh(cgi["refresh"]);
		return true;
	}

	const char*	GetThumbnailRequest::GetEngine()
	{
		return "rds";
	}

	const char*	GetThumbnailRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetThumbnailRequest::GetRequest()
	{
		return "GetThumbnail";
	}

	void GetThumbnailRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetThumbnailRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetThumbnailRequest::SetPath(const char* path)
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

	const char* GetThumbnailRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	GetThumbnailRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetThumbnailRequest::SetRasterName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	const char* GetThumbnailRequest::GetRasterName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	void GetThumbnailRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	const char* GetThumbnailRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	void GetThumbnailRequest::SetRefresh(const char* refresh)
	{
		if(refresh==NULL)
		{
			m_refresh = false;
		}
		else
		{
			if(g_stricmp(refresh,"TRUE")==0)
			{
				m_refresh = true;
			}
			else
			{
				m_refresh = false;
			}
		}
	}

	bool GetThumbnailRequest::IsRefresh()
	{
		return m_refresh;
	}
}