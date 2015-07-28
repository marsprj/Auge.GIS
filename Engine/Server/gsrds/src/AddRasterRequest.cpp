#include "AddRasterRequest.h"

namespace auge
{
	AddRasterRequest::AddRasterRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	AddRasterRequest::~AddRasterRequest()
	{

	}

	bool AddRasterRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetFilePath(cgi["filePath"]);
		SetRasterName(cgi["rasterName"]);
		SetRasterPath(cgi["rasterPath"]);
		SetSourceName(cgi["sourceName"]);
		return true;
	}

	const char*	AddRasterRequest::GetEngine()
	{
		return "rds";
	}

	const char*	AddRasterRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	AddRasterRequest::GetRequest()
	{
		return "AddRaster";
	}

	void AddRasterRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	AddRasterRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void AddRasterRequest::SetFilePath(const char* path)
	{
		if(path==NULL)
		{
			m_file_path = "/";
		}
		else
		{
			m_file_path = strlen(path) ? path : "/";
		}
	}

	const char* AddRasterRequest::GetFilePath()
	{
		return m_file_path.empty() ? NULL : m_file_path.c_str();
	}

	const char*	AddRasterRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void AddRasterRequest::SetRasterName(const char* rasterName)
	{
		if(rasterName==NULL)
		{
			m_raster_name.clear();
		}
		else
		{
			m_raster_name = rasterName;
		}
	}

	const char* AddRasterRequest::GetRasterName()
	{
		return m_raster_name.empty() ? NULL : m_raster_name.c_str();
	}

	void AddRasterRequest::SetRasterPath(const char* rasterPath)
	{
		if(rasterPath==NULL)
		{
			m_raster_path = "/";
		}
		else
		{
			m_raster_path = strlen(rasterPath) ? rasterPath : "/";
		}
	}

	const char* AddRasterRequest::GetRasterPath()
	{
		return m_raster_path.empty() ? NULL : m_raster_path.c_str();
	}

	void AddRasterRequest::SetSourceName(const char* sourceName)
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

	const char* AddRasterRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}
}