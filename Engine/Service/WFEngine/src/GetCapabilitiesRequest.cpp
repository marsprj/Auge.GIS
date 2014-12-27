#include "GetCapabilitiesRequest.h"

namespace auge
{
	GetCapabilitiesRequest::GetCapabilitiesRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
	}

	GetCapabilitiesRequest::~GetCapabilitiesRequest()
	{

	}

	const char*	GetCapabilitiesRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	GetCapabilitiesRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetCapabilitiesRequest::GetRequest()
	{
		return "GetCapabilities";
	}

	void GetCapabilitiesRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetCapabilitiesRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetCapabilitiesRequest::Create(GPropertySet& props)
	{
		const char* val = NULL;
		val = props.GetValue("version");
		SetVersion(val);
		return true;
	}

}