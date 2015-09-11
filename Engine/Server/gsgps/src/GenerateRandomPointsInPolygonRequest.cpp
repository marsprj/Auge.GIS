#include "GenerateRandomPointsInPolygonRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string GenerateRandomPointsInPolygonRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GenerateRandomPointsInPolygonRequest::DEFAULT_METHOD = "gps";

	GenerateRandomPointsInPolygonRequest::GenerateRandomPointsInPolygonRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	GenerateRandomPointsInPolygonRequest::~GenerateRandomPointsInPolygonRequest()
	{
		
	}

	const char*	GenerateRandomPointsInPolygonRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GenerateRandomPointsInPolygonRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GenerateRandomPointsInPolygonRequest::GetRequest()
	{
		return "GenerateRandomPointsInPolygon";
	}

	void GenerateRandomPointsInPolygonRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GenerateRandomPointsInPolygonRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GenerateRandomPointsInPolygonRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputTypeName(cgi["inputTypeName"]);
		SetInputSourceName(cgi["inputSourceName"]);

		SetOutputTypeName(cgi["outputTypeName"]);
		SetOutputSourceName(cgi["outputSourceName"]);

		SetCount(cgi["count"]);

		return true;
	}

/************************************************************************
<GenerateRandomPointsInPolygon service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<TypeName>cities</TypeName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<TypeName>cities_900913_post</TypeName>
  </Output>
</GenerateRandomPointsInPolygon>
************************************************************************/
	bool GenerateRandomPointsInPolygonRequest::Create(XDocument* pxDoc)
	{
		XElement	*pxRoot = NULL;
		XAttribute	*pxAttr = NULL;

		m_pxDoc = pxDoc;

		pxRoot = pxDoc->GetRootNode();
		pxAttr = pxRoot->GetAttribute("version");
		if(pxAttr!=NULL)
		{
			SetVersion(pxAttr->GetValue());
		}

		XNodeSet* pxNodeSet = NULL;
		// Input 
		XNode* pxNodeInput = pxRoot->GetFirstChild("Input");
		if(pxNodeInput==NULL)
		{
			return false;
		}
		XNode* pxNodeSourceName = pxNodeInput->GetFirstChild("SourceName");
		if(pxNodeSourceName!=NULL)
		{
			SetInputSourceName(pxNodeSourceName->GetContent());
		}
		XNode* pxTypeName = pxNodeInput->GetFirstChild("TypeName");
		if(pxTypeName!=NULL)
		{
			SetInputTypeName(pxTypeName->GetContent());
		}

		// output
		XNode* pxNodeOutput = pxRoot->GetFirstChild("Output");
		if(pxNodeOutput==NULL)
		{
			return false;
		}
		pxNodeSourceName = pxNodeOutput->GetFirstChild("SourceName");
		if(pxNodeSourceName!=NULL)
		{
			SetOutputSourceName(pxNodeSourceName->GetContent());
		}
		pxTypeName = pxNodeOutput->GetFirstChild("TypeName");
		if(pxTypeName!=NULL)
		{
			SetOutputTypeName(pxTypeName->GetContent());
		}

		return true;
	}

	const char*	GenerateRandomPointsInPolygonRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GenerateRandomPointsInPolygonRequest::SetUser(const char* user)
	{
		if(user==NULL)
		{
			m_user.clear();
		}
		else
		{
			m_user = user;
		}
	}

	const char* GenerateRandomPointsInPolygonRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GenerateRandomPointsInPolygonRequest::SetRequestMethod(const char* method)
	{
		if(method==NULL)
		{
			m_request_method = DEFAULT_METHOD;
		}
		else if(!strlen(method))
		{
			m_request_method = DEFAULT_METHOD;
		}
		else
		{
			m_request_method = method;
		}
	}

	void GenerateRandomPointsInPolygonRequest::SetHost(const char* host)
	{
		if(host==NULL)
		{
			m_host = DEFAULT_HOST;
		}
		else if(!strlen(host))
		{
			m_host = DEFAULT_HOST;
		}
		else
		{
			m_host = host;
		}
	}

	const char* GenerateRandomPointsInPolygonRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GenerateRandomPointsInPolygonRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* GenerateRandomPointsInPolygonRequest::GetInputTypeName()
	{
		return m_in_type_name.c_str();
	}

	void GenerateRandomPointsInPolygonRequest::SetInputTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_in_type_name.clear();
		}
		else
		{
			m_in_type_name = typeName;
		}
	}

	const char* GenerateRandomPointsInPolygonRequest::GetInputSourceName()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void GenerateRandomPointsInPolygonRequest::SetInputSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_in_source_name.clear();
		}
		else
		{
			m_in_source_name = sourceName;
		}
	}

	const char* GenerateRandomPointsInPolygonRequest::GetOutputTypeName()
	{
		return m_out_type_name.c_str();
	}

	void GenerateRandomPointsInPolygonRequest::SetOutputTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_out_type_name.clear();
		}
		else
		{
			m_out_type_name = typeName;
		}
	}

	const char* GenerateRandomPointsInPolygonRequest::GetOutputSourceName()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	void GenerateRandomPointsInPolygonRequest::SetOutputSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_out_source_name.clear();
		}
		else
		{
			m_out_source_name = sourceName;
		}
	}

	void GenerateRandomPointsInPolygonRequest::SetCount(const char* count)
	{
		if(count==NULL)
		{
			return;
		}
		m_count = atoi(count);
	}

	g_int GenerateRandomPointsInPolygonRequest::GetCount()
	{
		return m_count;
	}

	void GenerateRandomPointsInPolygonRequest::SetOutputFormat(const char* format)
	{
		if(format==NULL)
		{
			m_output_format = "GML2";
		}
		else
		{
			m_output_format = format;
		}
	}

	const char* GenerateRandomPointsInPolygonRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}
	
	void GenerateRandomPointsInPolygonRequest::Info()
	{
		GLogger* pLogger = augeGetLoggerInstance();
		char str[AUGE_MSG_MAX];
		const char* value = GetRequest();
		pLogger->Debug("[Requet Parameters]");
		g_sprintf(str,"\t%s:%s", "Request", (value==NULL)?"":value);
		pLogger->Debug(str);
		value = GetUser();
		g_sprintf(str,"\t%s:%s", "User", (value==NULL)?"":value);
		pLogger->Debug(str);
		g_sprintf(str,"\t%s:%s", "Service", "ims");
		pLogger->Debug(str);
		value = GetVersion();
		g_sprintf(str,"\t%s:%s", "Version", (value==NULL)?"":value);
		pLogger->Debug(str);

	}

}
