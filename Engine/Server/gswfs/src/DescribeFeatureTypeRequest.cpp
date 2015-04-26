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

	//const char*	DescribeFeatureTypeRequest::GetEngine()
	//{
	//	return "wfs";
	//}

	//const char*	DescribeFeatureTypeRequest::GetVersion()
	//{
	//	return m_version.c_str();
	//}

	const char* DescribeFeatureTypeRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	const char*	DescribeFeatureTypeRequest::GetRequest()
	{
		return "DescribeFeatureType";
	}

	//void DescribeFeatureTypeRequest::SetVersion(const char* value)
	//{
	//	if(value==NULL)
	//	{
	//		return;
	//	}
	//	m_version = value;
	//}

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

	const char*	DescribeFeatureTypeRequest::GetSourceName()
	{
		return m_source_name.c_str();
	}

	void DescribeFeatureTypeRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name  = sourceName;
		}
	}

	//const char*	DescribeFeatureTypeRequest::GetMimeType()
	//{
	//	return m_mime_type.c_str();
	//}

	bool DescribeFeatureTypeRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetMapName(cgi["mapName"]);
		SetTypeName(cgi["typeName"]);
		SetSourceName(cgi["sourceName"]);
		return true;
	}

}