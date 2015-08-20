#include "GetAreaRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string GetAreaRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GetAreaRequest::DEFAULT_METHOD = "gps";

	GetAreaRequest::GetAreaRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_pFilter(NULL),
		m_output_format("GML2"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	GetAreaRequest::~GetAreaRequest()
	{
		if(m_pFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFilter);
		}
	}

	const char*	GetAreaRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GetAreaRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetAreaRequest::GetRequest()
	{
		return "GetArea";
	}

	void GetAreaRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetAreaRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetAreaRequest::Create(rude::CGI& cgi)
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

	const char*	GetAreaRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetAreaRequest::SetUser(const char* user)
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

	const char* GetAreaRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetAreaRequest::SetRequestMethod(const char* method)
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

	void GetAreaRequest::SetHost(const char* host)
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

	const char* GetAreaRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetAreaRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* GetAreaRequest::GetInputTypeName()
	{
		return m_in_type_name.c_str();
	}

	void GetAreaRequest::SetInputTypeName(const char* typeName)
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

	const char* GetAreaRequest::GetInputSourceName()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void GetAreaRequest::SetInputSourceName(const char* sourceName)
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

	const char* GetAreaRequest::GetOutputTypeName()
	{
		return m_out_type_name.c_str();
	}

	void GetAreaRequest::SetOutputTypeName(const char* typeName)
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

	const char* GetAreaRequest::GetOutputSourceName()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	void GetAreaRequest::SetOutputSourceName(const char* sourceName)
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
	void GetAreaRequest::SetOutputFormat(const char* format)
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

	const char* GetAreaRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	GFilter* GetAreaRequest::GetFilter()
	{
		return m_pFilter;
	}

	void GetAreaRequest::SetFilter(XElement* pxFilter)
	{

	}

	void GetAreaRequest::Info()
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
