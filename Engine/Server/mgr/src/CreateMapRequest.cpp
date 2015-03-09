#include "CreateMapRequest.h"

namespace auge
{
	CreateMapRequest::CreateMapRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
	}

	CreateMapRequest::~CreateMapRequest()
	{

	}

	bool CreateMapRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetExtent(cgi["extent"]);
		SetSRID(cgi["srid"]);
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