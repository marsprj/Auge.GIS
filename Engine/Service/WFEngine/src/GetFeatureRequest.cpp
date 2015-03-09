#include "GetFeatureRequest.h"

namespace auge
{
	GetFeatureRequest::GetFeatureRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
	}

	GetFeatureRequest::~GetFeatureRequest()
	{

	}

	const char*	GetFeatureRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	GetFeatureRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* GetFeatureRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	GetFeatureRequest::GetRequest()
	{
		return "GetFeature";
	}

	void GetFeatureRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void GetFeatureRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}

	const char*	GetFeatureRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetFeatureRequest::Create(GPropertySet& props)
	{
		const char* val = NULL;
		val = props.GetValue("version");
		SetVersion(val);
		val = props.GetValue("typeName");
		SetTypeName(val);
		return true;
	}

}