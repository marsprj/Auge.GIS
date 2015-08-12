#include "RegisterDataSourceRequest.h"

namespace auge
{
	RegisterDataSourceRequest::RegisterDataSourceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
		m_source_type = augeWorkspaceUnknown;
	}

	RegisterDataSourceRequest::~RegisterDataSourceRequest()
	{

	}

	bool RegisterDataSourceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetDataEngine(cgi["engine"]);
		SetURI(cgi["uri"]);
		SetSourceType(cgi["type"]);
		return true;
	}

	const char*	RegisterDataSourceRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	RegisterDataSourceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	RegisterDataSourceRequest::GetRequest()
	{
		return "RegisterDataSource";
	}

	const char* RegisterDataSourceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void RegisterDataSourceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	RegisterDataSourceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void RegisterDataSourceRequest::SetName(const char* name)
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

	const char*	RegisterDataSourceRequest::GetName()
	{
		return m_name.c_str();
	}
	
	const char*	RegisterDataSourceRequest::GetDataEngine()
	{
		return m_data_engine.c_str();
	}

	const char* RegisterDataSourceRequest::GetURI()
	{
		return m_uri.c_str();
	}

	void RegisterDataSourceRequest::SetDataEngine(const char* engine)
	{
		if(engine==NULL)
		{
			m_data_engine.clear();
		}
		else
		{
			m_data_engine = engine;
		}
	}

	void RegisterDataSourceRequest::SetURI(const char* uri)
	{
		if(uri==NULL)
		{
			m_uri.clear();
		}
		else
		{
			m_uri = uri;
		}
	}

	void RegisterDataSourceRequest::SetSourceType(const char* type)
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

	augeWorkspaceType RegisterDataSourceRequest::GetSourceType()
	{
		return m_source_type;
	}
}