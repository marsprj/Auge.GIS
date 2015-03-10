#include "GetGmlObjectRequest.h"

namespace auge
{
	GetGmlObjectRequest::GetGmlObjectRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
	}

	GetGmlObjectRequest::~GetGmlObjectRequest()
	{

	}

	const char*	GetGmlObjectRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	GetGmlObjectRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetGmlObjectRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	GetGmlObjectRequest::GetRequest()
	{
		return "GetGmlObject";
	}

	void GetGmlObjectRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void GetGmlObjectRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}

	const char*	GetGmlObjectRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetGmlObjectRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTypeName(cgi["typeName"]);
		return true;
	}

}