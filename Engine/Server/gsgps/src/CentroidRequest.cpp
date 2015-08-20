#include "CentroidRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string CentroidRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string CentroidRequest::DEFAULT_METHOD = "gps";

	CentroidRequest::CentroidRequest():
	m_version("1.0.0"),
	m_mime_type("text/xml"),
	m_pFilter(NULL),
	m_output_format("GML2"),
	m_host(DEFAULT_HOST),
	m_request_method(DEFAULT_METHOD),
	m_encoding(AUGE_DEFAULT_ENCODING)
	{
		
	}

	CentroidRequest::~CentroidRequest()
	{
		if(m_pFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFilter);
		}
	}

	const char*	CentroidRequest::GetEngine()
	{
		return "gps";
	}

	const char*	CentroidRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	CentroidRequest::GetRequest()
	{
		return "Centroid";
	}

	void CentroidRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	CentroidRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool CentroidRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInputTypeName(cgi["inputTypeName"]);
		SetInputSourceName(cgi["inputSourceName"]);

		SetOutputTypeName(cgi["outputTypeName"]);
		SetOutputSourceName(cgi["outputSourceName"]);

		return true;
	}

	void CentroidRequest::SetUser(const char* user)
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

	const char* CentroidRequest::GetUser()
	{
		return m_user.c_str();
	}

	void CentroidRequest::SetRequestMethod(const char* method)
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

	void CentroidRequest::SetHost(const char* host)
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

	const char* CentroidRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* CentroidRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* CentroidRequest::GetInputTypeName()
	{
		return m_in_type_name.c_str();
	}

	void CentroidRequest::SetInputTypeName(const char* typeName)
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

	const char* CentroidRequest::GetInputSourceName()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void CentroidRequest::SetInputSourceName(const char* sourceName)
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

	const char* CentroidRequest::GetOutputTypeName()
	{
		return m_out_type_name.c_str();
	}

	void CentroidRequest::SetOutputTypeName(const char* typeName)
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

	const char* CentroidRequest::GetOutputSourceName()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	void CentroidRequest::SetOutputSourceName(const char* sourceName)
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

	void CentroidRequest::SetOutputFormat(const char* format)
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

	const char* CentroidRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	GFilter* CentroidRequest::GetFilter()
	{
		return m_pFilter;
	}

	void CentroidRequest::SetFilter(XElement* pxFilter)
	{
		
	}

	const char*	CentroidRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void CentroidRequest::Info()
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
