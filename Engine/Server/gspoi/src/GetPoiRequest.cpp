#include "GetPoiRequest.h"

namespace auge
{
	GetPoiRequest::GetPoiRequest()
	{
		m_version = "1.0.0";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetPoiRequest::~GetPoiRequest()
	{

	}

	bool GetPoiRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetExtent(cgi["extent"]);
		SetLimit(cgi["limit"]);
		SetOffset(cgi["offset"]);
		return true;
	}

	const char*	GetPoiRequest::GetEngine()
	{
		return "poi";
	}

	const char*	GetPoiRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetPoiRequest::GetRequest()
	{
		return "GetPoi";
	}

	void GetPoiRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetPoiRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}


	const char*	GetPoiRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetPoiRequest::SetName(const char* name)
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

	const char* GetPoiRequest::GetName()
	{
		return m_name.empty() ? NULL : m_name.c_str();
	}

	const char* GetPoiRequest::GetFormat()
	{
		return m_format.empty() ? NULL : m_format.c_str();
	}

	GEnvelope& GetPoiRequest::GetExtent()
	{
		return m_extent;
	}

	void GetPoiRequest::SetExtent(const char* extent)
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

	g_uint GetPoiRequest::GetLimit()
	{
		return m_limit;
	}

	g_uint GetPoiRequest::GetOffset()
	{
		return m_offset;
	}

	void GetPoiRequest::SetLimit(const char* limit)
	{
		if(limit==NULL)
		{
			m_limit = 50;
		}
		else if(!isdigit(limit[0]))
		{
			m_limit = 50;
		}
		else
		{
			m_limit = atoi(limit);
		}
	}

	void GetPoiRequest::SetOffset(const char* offset)
	{
		if(offset==NULL)
		{
			m_offset = 0;
		}
		else if(!isdigit(offset[0]))
		{
			m_offset = 10;
		}
		else
		{
			m_offset = atoi(offset);
		}
	}
}