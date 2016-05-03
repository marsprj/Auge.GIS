#include "CreateServiceRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	CreateServiceRequest::CreateServiceRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	CreateServiceRequest::~CreateServiceRequest()
	{

	}

	bool CreateServiceRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetName(cgi["name"]);
		SetMapName(cgi["mapName"]);
		SetURI(cgi["uri"]);
		return true;
	}

	const char*	CreateServiceRequest::GetEngine()
	{
		return "ims";
	}

	const char*	CreateServiceRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateServiceRequest::GetRequest()
	{
		return "CreateService";
	}

	const char* CreateServiceRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void CreateServiceRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CreateServiceRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool CreateServiceRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void CreateServiceRequest::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			m_name = pWebContext->ParameterEncoding(name);
		}
	}

	const char*	CreateServiceRequest::GetName()
	{
		return m_name.c_str();
	}

	void CreateServiceRequest::SetMapName(const char* mapName)
	{
		if(mapName==NULL)
		{
			m_map_name.clear();
		}
		else
		{
			WebContext* pWebContext = augeGetWebContextInstance();
			m_map_name = pWebContext->ParameterEncoding(mapName);
		}
	}

	const char* CreateServiceRequest::GetMapName()
	{
		return m_map_name.empty() ? NULL : m_map_name.c_str();
	}

	void CreateServiceRequest::SetURI(const char* uri)
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

	const char* CreateServiceRequest::GetURI()
	{
		return m_uri.c_str();
	}
}