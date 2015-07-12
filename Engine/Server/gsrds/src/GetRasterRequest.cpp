#include "GetRasterRequest.h"

namespace auge
{
	GetRasterRequest::GetRasterRequest()
	{
		m_version = "1.0.0";
		m_encoding = "GBK";
	}

	GetRasterRequest::~GetRasterRequest()
	{

	}

	bool GetRasterRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetPath(cgi["path"]);
		SetRasterName(cgi["rasterName"]);
		SetSourceName(cgi["sourceName"]);
		SetFormat(cgi["format"]);
		SetExtent(cgi["extent"]);
		SetBands(cgi["bands"]);
		return true;
	}

	const char*	GetRasterRequest::GetEngine()
	{
		return "rds";
	}

	const char*	GetRasterRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetRasterRequest::GetRequest()
	{
		return "GetRaster";
	}

	void GetRasterRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetRasterRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetRasterRequest::SetPath(const char* path)
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

	const char* GetRasterRequest::GetPath()
	{
		return m_path.empty() ? NULL : m_path.c_str();
	}

	const char*	GetRasterRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetRasterRequest::SetRasterName(const char* rasterName)
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

	const char* GetRasterRequest::GetName()
	{
		return m_raster_name.empty() ? NULL : m_raster_name.c_str();
	}

	void GetRasterRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_sourc_name.clear();
		}
		else
		{
			m_sourc_name = sourceName;
		}
	}

	const char* GetRasterRequest::GetSourceName()
	{
		return m_sourc_name.c_str();
	}

	void GetRasterRequest::SetFormat(const char* format)
	{
		if(format==NULL)
		{
			m_format.clear();
		}
		else
		{
			m_format = format;
		}
	}

	const char* GetRasterRequest::GetFormat()
	{
		return m_format.empty() ? NULL : m_format.c_str();
	}

	GEnvelope& GetRasterRequest::GetExtent()
	{
		return m_extent;
	}

	void GetRasterRequest::SetExtent(const char* extent)
	{
		if(extent==NULL)
		{
			return;
		}
		double xmin,ymin,xmax,ymax;
		int ret = sscanf(extent,"%lf,%lf,%lf,%lf",&xmin,&ymin,&xmax,&ymax);
		if(ret==4)
		{
			m_extent.Set(xmin,ymin,xmax,ymax);
		}
	}

	void GetRasterRequest::SetBands(const char* bands)
	{	
		if(strlen(bands)==0)
		{
			m_bands.resize(0);
			return;
		}

		char* str = strdup(bands);
		char* ptr = strtok(str, ",");
		while(ptr!=NULL)
		{
			m_bands.push_back(atoi(ptr));

			ptr = strtok(NULL, ",");
		}
		free(str);

	}
}