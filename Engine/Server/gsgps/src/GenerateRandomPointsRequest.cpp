#include "GenerateRandomPointsRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string GenerateRandomPointsRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GenerateRandomPointsRequest::DEFAULT_METHOD = "gps";

	GenerateRandomPointsRequest::GenerateRandomPointsRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	GenerateRandomPointsRequest::~GenerateRandomPointsRequest()
	{
		
	}

	const char*	GenerateRandomPointsRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GenerateRandomPointsRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GenerateRandomPointsRequest::GetRequest()
	{
		return "GenerateRandomPoints";
	}

	void GenerateRandomPointsRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GenerateRandomPointsRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GenerateRandomPointsRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetOutputTypeName(cgi["TypeName"]);
		SetOutputSourceName(cgi["SourceName"]);
		SetExtent(cgi["extent"]);
		SetSRID(cgi["Srid"]);
		SetCount(cgi["count"]);

		return true;
	}

/************************************************************************
<GenerateRandomPoints service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<TypeName>cities</TypeName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<TypeName>cities_900913_post</TypeName>
  </Output>
</GenerateRandomPoints>
************************************************************************/
	bool GenerateRandomPointsRequest::Create(XDocument* pxDoc)
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
		// output
		XNode* pxNodeOutput = pxRoot->GetFirstChild("Output");
		if(pxNodeOutput==NULL)
		{
			return false;
		}
		//pxNodeSourceName = pxNodeOutput->GetFirstChild("SourceName");
		//if(pxNodeSourceName!=NULL)
		//{
		//	SetOutputSourceName(pxNodeSourceName->GetContent());
		//}
		//pxTypeName = pxNodeOutput->GetFirstChild("TypeName");
		//if(pxTypeName!=NULL)
		//{
		//	SetOutputTypeName(pxTypeName->GetContent());
		//}

		return true;
	}

	const char*	GenerateRandomPointsRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GenerateRandomPointsRequest::SetUser(const char* user)
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

	const char* GenerateRandomPointsRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GenerateRandomPointsRequest::SetRequestMethod(const char* method)
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

	void GenerateRandomPointsRequest::SetHost(const char* host)
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

	const char* GenerateRandomPointsRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GenerateRandomPointsRequest::GetHost()
	{
		return m_host.c_str();
	}
	
	const char* GenerateRandomPointsRequest::GetOutputTypeName()
	{
		return m_out_type_name.c_str();
	}

	void GenerateRandomPointsRequest::SetOutputTypeName(const char* typeName)
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

	const char* GenerateRandomPointsRequest::GetOutputSourceName()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	void GenerateRandomPointsRequest::SetOutputSourceName(const char* sourceName)
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

	void GenerateRandomPointsRequest::SetOutputFormat(const char* format)
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

	const char* GenerateRandomPointsRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	void GenerateRandomPointsRequest::SetSRID(const char* srid)
	{
		m_srid = atoi(srid);
	}

	g_uint GenerateRandomPointsRequest::GetSRID()
	{
		return m_srid;
	}

	void GenerateRandomPointsRequest::SetExtent(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		float xmin, ymin, xmax, ymax;
		int count = sscanf(value,"%f,%f,%f,%f", &xmin, &ymin, &xmax, &ymax);
		if(count==4)
		{
			m_extent.Set(xmin, ymin, xmax, ymax);
		}
	}

	GEnvelope& GenerateRandomPointsRequest::GetExtent()
	{
		return m_extent;
	}

	void GenerateRandomPointsRequest::SetCount(const char* count)
	{
		m_count = atoi(count);
	}

	g_uint GenerateRandomPointsRequest::GetCount()
	{
		return m_count;
	}

	
	void GenerateRandomPointsRequest::Info()
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
