#include "DescribeFeatureTypeRequest.h"

namespace auge
{
	DescribeFeatureTypeRequest::DescribeFeatureTypeRequest()
	{
		m_version = "1.1.0";
		m_mime_type = "text/xml";
	}

	DescribeFeatureTypeRequest::~DescribeFeatureTypeRequest()
	{

	}

	const char*	DescribeFeatureTypeRequest::GetEngine()
	{
		return "wfs";
	}

	const char*	DescribeFeatureTypeRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char* DescribeFeatureTypeRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	DescribeFeatureTypeRequest::GetRequest()
	{
		return "DescribeFeatureType";
	}

	void DescribeFeatureTypeRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void DescribeFeatureTypeRequest::SetTypeName(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		const char* sep = strstr(value,":");
		m_full_name = value;
		m_type_name = sep==NULL ? value : sep+1;
	}

	const char*	DescribeFeatureTypeRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool DescribeFeatureTypeRequest::Create(GPropertySet& props)
	{
		const char* val = NULL;
		val = props.GetValue("version");
		SetVersion(val);
		val = props.GetValue("typeName");
		SetTypeName(val);
		return true;
	}

}