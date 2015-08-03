#include "DescribeFeatureTypeRequest.h"
#include "AugeWebCore.h"

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
			m_type_name.clear();
		}
		else
		{
			const char* sep = strstr(value,":");			
			const char* typeName = (sep==NULL ? value : sep+1);

			WebContext* pWebContext = augeGetWebContextInstance();
			m_full_name = pWebContext->ParameterEncoding(value);
			m_type_name = pWebContext->ParameterEncoding(typeName);
			
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

	void DescribeFeatureTypeRequest::Info()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetRequest();
		pLogger->Debug("[Requet Parameters]");
		g_sprintf(str,"\t%s:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);

		value = GetVersion();
		g_sprintf(str,"\t%s:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);

		g_sprintf(str,"\t%s:%s", "Service", "wfs");
		pLogger->Debug(str);

		value = GetSourceName();
		g_sprintf(str,"\t%s:%s", "sourceName", (value==NULL)?"":value);
		pLogger->Debug(str);

		value = GetMapName();
		g_sprintf(str,"\t%s:%s", "mapName", (value==NULL)?"":value);
		pLogger->Debug(str);

		value = GetTypeName();
		g_sprintf(str,"\t%s:%s", "typeName", (value==NULL)?"":value);
		pLogger->Debug(str);

	}
}