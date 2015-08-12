#include "CreateTileStoreRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	CreateTileStoreRequest::CreateTileStoreRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	CreateTileStoreRequest::~CreateTileStoreRequest()
	{

	}

	bool CreateTileStoreRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetSourceName(cgi["sourceName"]);
		SetStoreName(cgi["storeName"]);
		SetType(cgi["type"]);
		return true;
	}

	const char*	CreateTileStoreRequest::GetEngine()
	{
		return "dbs";
	}

	const char*	CreateTileStoreRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateTileStoreRequest::GetRequest()
	{
		return "CreateTileStore";
	}

	void CreateTileStoreRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CreateTileStoreRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void CreateTileStoreRequest::SetSourceName(const char* name)
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
			//m_source_name = name;
			WebContext* pWebContext = augeGetWebContextInstance();
			m_source_name = pWebContext->ParameterEncoding(name);
		}

	}

	const char*	CreateTileStoreRequest::GetSourceName()
	{
		if(m_source_name.empty())
		{
			return NULL;
		}
		return m_source_name.c_str();
	}

	void CreateTileStoreRequest::SetStoreName(const char* name)
	{
		if(name==NULL||(strlen(name)==0))
		{
			m_store_name.clear();
		}
		else if(strlen(name)==0)
		{
			m_store_name.clear();
		}
		else
		{
			//m_store_name = name;
			WebContext* pWebContext = augeGetWebContextInstance();
			m_store_name = pWebContext->ParameterEncoding(name);
		}

	}

	const char*	CreateTileStoreRequest::GetStoreName()
	{
		if(m_store_name.empty())
		{
			return NULL;
		}
		return m_store_name.c_str();
	}

	const char* CreateTileStoreRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void CreateTileStoreRequest::SetType(const char* type)
	{
		if(type==NULL)
		{
			m_type = augeTileGoogleCRS84Quad;
		}
		else
		{
			if(g_stricmp(type, "PGIS")==0)
			{
				m_type = augeTilePGIS;
			}
			else if(g_stricmp(type, "GoogleCRS84Quad")==0)
			{
				m_type = augeTileGoogleCRS84Quad;
			}
			else if(g_stricmp(type, "EPSG4326")==0)
			{
				m_type = augeTileEPSG4326;
			}
		}
	}

	augeTileType CreateTileStoreRequest::GetType()
	{
		return m_type;
	}

}