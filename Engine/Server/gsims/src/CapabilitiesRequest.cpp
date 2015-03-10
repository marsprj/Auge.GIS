#include "CapabilitiesRequest.h"

namespace auge
{
	CapabilitiesRequest::CapabilitiesRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml")
	{
		
	}

	CapabilitiesRequest::~CapabilitiesRequest()
	{

	}

	const char*	CapabilitiesRequest::GetEngine()
	{
		return "ims";
	}

	const char*	CapabilitiesRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CapabilitiesRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	void CapabilitiesRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CapabilitiesRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool CapabilitiesRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		return true;
	}

}