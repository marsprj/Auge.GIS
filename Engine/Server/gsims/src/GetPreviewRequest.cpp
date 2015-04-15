#include "GetPreviewRequest.h"

namespace auge
{
	GetPreviewRequest::GetPreviewRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "image/png";
		m_format = "image/png";
	}

	GetPreviewRequest::~GetPreviewRequest()
	{

	}

	bool GetPreviewRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetDataSetName(cgi["dataSetName"]);
		SetWidth(cgi["width"]);
		SetHeight(cgi["height"]);
		SetExtent(cgi["bbox"]);
		return true;
	}

	const char*	GetPreviewRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetPreviewRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetPreviewRequest::GetRequest()
	{
		return "GetPreview";
	}

	void GetPreviewRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetPreviewRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetPreviewRequest::SetSourceName(const char* name)
	{
		if(name==NULL)
		{
			m_source_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = name;
		}

	}

	const char*	GetPreviewRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void GetPreviewRequest::SetDataSetName(const char* name)
	{
		if(name==NULL)
		{
			m_dataset_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_dataset_name.clear();
		}
		else
		{
			m_dataset_name = name;
		}

	}

	const char*	GetPreviewRequest::GetDataSetName()
	{
		if(m_dataset_name.empty())
		{
			return NULL;
		}
		return m_dataset_name.c_str();
	}

	const char* GetPreviewRequest::GetFormat()
	{
		return m_format.c_str();
	}

	g_int GetPreviewRequest::GetWidth()
	{
		return m_width;
	}

	g_int GetPreviewRequest::GetHeight()
	{
		return m_height;
	}

	GEnvelope& GetPreviewRequest::GetExtent()
	{
		return m_extent;
	}

	void GetPreviewRequest::SetExtent(const char* extent)
	{
		if(extent!=NULL)
		{
			double xmin, ymin, xmax, ymax;
			sscanf(extent, "%lf,%lf,%lf,%lf,",&xmin, &ymin, &xmax, &ymax);
			m_extent.Set(xmin, ymin, xmax, ymax);
		}
	}

	void GetPreviewRequest::SetWidth(const char* width)
	{
		if(width)
		{
			m_width = atoi(width);
		}
	}

	void GetPreviewRequest::SetHeight(const char* height)
	{
		if(height)
		{
			m_height = atoi(height);
		}
	}

}