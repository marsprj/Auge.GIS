#include "CreateMapRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	CreateMapRequest::CreateMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	CreateMapRequest::~CreateMapRequest()
	{

	}

	bool CreateMapRequest::Create(rude::CGI& cgi)
	{
		char str[AUGE_NAME_MAX];
		WebContext* pWebContext = augeGetWebContextInstance();

		auge_web_parameter_encoding(cgi["name"], str, AUGE_NAME_MAX, pWebContext->IsIE());
		SetName(str);
		SetExtent(cgi["extent"]);
		SetSRID(cgi["srid"]);
		SetVersion(cgi["version"]);
		return true;
	}

	const char*	CreateMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	CreateMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateMapRequest::GetRequest()
	{
		return "CreateMap";
	}

	const char* CreateMapRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void CreateMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CreateMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	void CreateMapRequest::SetName(const char* name)
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
	
	const char*	CreateMapRequest::GetName()
	{
		if(m_name.empty())
		{
			return NULL;
		}
		return m_name.c_str();
	}

	const char*	CreateMapRequest::GetExtent()
	{
		if(m_extent.empty())
		{
			return NULL;
		}
		return m_extent.c_str();
	}

	const char*	CreateMapRequest::GetSRID()
	{
		return m_srid.c_str();
	}
	
	void CreateMapRequest::SetExtent(const char* extent)
	{
		if(extent==NULL)
		{
			m_extent.clear();
		}
		else
		{
			m_extent = extent;
		}
	}

	void CreateMapRequest::SetSRID(const char* srid)
	{
		if(srid==NULL)
		{
			m_srid.clear();
		}
		else
		{
			m_srid = srid;
		}
	}

}