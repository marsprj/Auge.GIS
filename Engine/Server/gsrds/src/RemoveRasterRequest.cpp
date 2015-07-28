#include "RemoveRasterRequest.h"

namespace auge
{
	RemoveRasterRequest::RemoveRasterRequest()
	{
		m_version = "1.0.0";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	RemoveRasterRequest::~RemoveRasterRequest()
	{

	}

	bool RemoveRasterRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetRasterPath(cgi["rasterPath"]);
		SetRasterName(cgi["rasterName"]);
		SetSourceName(cgi["sourceName"]);
		return true;
	}

	const char*	RemoveRasterRequest::GetEngine()
	{
		return "rds";
	}

	const char*	RemoveRasterRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RemoveRasterRequest::GetRequest()
	{
		return "RemoveRaster";
	}

	void RemoveRasterRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RemoveRasterRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RemoveRasterRequest::SetRasterPath(const char* path)
	{
		if(path==NULL)
		{
			m_raster_path.clear();
		}
		else
		{
			m_raster_path = path;
		}
	}

	const char* RemoveRasterRequest::GetRasterPath()
	{
		return m_raster_path.empty() ? NULL : m_raster_path.c_str();
	}

	const char*	RemoveRasterRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RemoveRasterRequest::SetRasterName(const char* name)
	{
		if(name==NULL)
		{
			m_raster_name.clear();
		}
		else
		{
			m_raster_name = name;
		}
	}

	const char* RemoveRasterRequest::GetRasterName()
	{
		return m_raster_name.empty() ? NULL : m_raster_name.c_str();
	}

	void RemoveRasterRequest::SetSourceName(const char* sourceName)
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

	const char* RemoveRasterRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}
}