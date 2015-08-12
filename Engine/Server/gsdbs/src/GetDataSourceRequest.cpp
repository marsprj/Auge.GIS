#include "GetDataSourceRequest.h"

namespace auge
{
	GetDataSourceRequest::GetDataSourceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetDataSourceRequest::~GetDataSourceRequest()
	{

	}

	bool GetDataSourceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetSourceType(cgi["type"]);
		return true;
	}

	const char*	GetDataSourceRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	GetDataSourceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetDataSourceRequest::GetRequest()
	{
		return "GetDataSource";
	}

	const char*	GetDataSourceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetDataSourceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetDataSourceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void GetDataSourceRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}

	}

	const char*	GetDataSourceRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

	void GetDataSourceRequest::SetSourceType(const char* type)
	{
		if(type==NULL)
		{
			return;
		}

		if(g_stricmp(type, "Feature")==0)
		{
			m_source_type = augeWorkspaceFeature;
		}
		else if(g_stricmp(type, "Raster")==0)
		{
			m_source_type = augeWorkspaceRaster;
		}
		else if(g_stricmp(type, "Tile")==0)
		{
			m_source_type = augeWorkspaceTile;
		}
		else
		{
			m_source_type = augeWorkspaceUnknown;
		}
	}

	augeWorkspaceType GetDataSourceRequest::GetSourceType()
	{
		return m_source_type;
	}
}