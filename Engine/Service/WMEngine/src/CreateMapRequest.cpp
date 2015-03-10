#include "CreateMapRequest.h"

namespace auge
{
	CreateMapRequest::CreateMapRequest()
	{
		m_version = "1.3.0";
		m_mime_type = "text/xml";
	}

	CreateMapRequest::~CreateMapRequest()
	{

	}

	const char*	CreateMapRequest::GetEngine()
	{
		return "wms";
	}

	const char*	CreateMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CreateMapRequest::GetRequest()
	{
		return "GetCapabilities";
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

	bool CreateMapRequest::Create(GPropertySet& props)
	{
		const char* val = NULL;
		val = props.GetValue("version");
		SetVersion(val);
		return true;
	}
}