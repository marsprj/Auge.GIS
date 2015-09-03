#include "DelaunayRequest.h"

namespace auge
{
	std::string DelaunayRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string DelaunayRequest::DEFAULT_METHOD = "gps";

	DelaunayRequest::DelaunayRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	DelaunayRequest::~DelaunayRequest()
	{
		
	}

	const char*	DelaunayRequest::GetEngine()
	{
		return "gps";
	}

	const char*	DelaunayRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DelaunayRequest::GetRequest()
	{
		return "Delaunay";
	}

	void DelaunayRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DelaunayRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool DelaunayRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputTypeName(cgi["inputTypeName"]);
		SetInputSourceName(cgi["inputSourceName"]);
		SetInputZField(cgi["inputZField"]);

		SetOutputTypeName(cgi["outputTypeName"]);
		SetOutputSourceName(cgi["outputSourceName"]);

		return true;
	}

/************************************************************************
<Delaunay service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<TypeName>cities</TypeName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<TypeName>cities_900913_post</TypeName>
	<SRID>900913</SRID>
  </Output>
</Delaunay>
************************************************************************/
	bool DelaunayRequest::Create(XDocument* pxDoc)
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


	const char*	DelaunayRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void DelaunayRequest::SetUser(const char* user)
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

	const char* DelaunayRequest::GetUser()
	{
		return m_user.c_str();
	}

	void DelaunayRequest::SetRequestMethod(const char* method)
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

	void DelaunayRequest::SetHost(const char* host)
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

	const char* DelaunayRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* DelaunayRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* DelaunayRequest::GetInputTypeName()
	{
		return m_in_type_name.c_str();
	}

	void DelaunayRequest::SetInputTypeName(const char* typeName)
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

	const char* DelaunayRequest::GetInputSourceName()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void DelaunayRequest::SetInputSourceName(const char* sourceName)
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

	void DelaunayRequest::SetInputZField(const char* zfield)
	{
		if(zfield==NULL)
		{
			m_in_z_filed.clear();
		}
		else
		{
			m_in_z_filed = zfield;
		}
	}

	const char* DelaunayRequest::GetInputZField()
	{
		return m_in_z_filed.empty() ? NULL : m_in_z_filed.c_str();
	}

	const char* DelaunayRequest::GetOutputTypeName()
	{
		return m_out_type_name.c_str();
	}

	void DelaunayRequest::SetOutputTypeName(const char* typeName)
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

	const char* DelaunayRequest::GetOutputSourceName()
	{
		return m_out_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void DelaunayRequest::SetOutputSourceName(const char* sourceName)
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

	void DelaunayRequest::SetOutputFormat(const char* format)
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

	//const char* DelaunayRequest::GetOutputFormat()
	//{
	//	return m_output_format.c_str();
	//}
	//
	void DelaunayRequest::Info()
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
