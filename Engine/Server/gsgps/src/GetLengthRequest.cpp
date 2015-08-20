#include "GetLengthRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string GetLengthRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GetLengthRequest::DEFAULT_METHOD = "gps";

	GetLengthRequest::GetLengthRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_pFilter(NULL),
		m_output_format("GML2"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	GetLengthRequest::~GetLengthRequest()
	{
		if(m_pFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFilter);
		}
	}

	const char*	GetLengthRequest::GetEngine()
	{
		return "gps";
	}

	const char*	GetLengthRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetLengthRequest::GetRequest()
	{
		return "GetLength";
	}

	void GetLengthRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetLengthRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetLengthRequest::Create(rude::CGI& cgi)
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

	const char*	GetLengthRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetLengthRequest::SetUser(const char* user)
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

	const char* GetLengthRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetLengthRequest::SetRequestMethod(const char* method)
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

	void GetLengthRequest::SetHost(const char* host)
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

	const char* GetLengthRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetLengthRequest::GetHost()
	{
		return m_host.c_str();
	}
	const char* GetLengthRequest::GetInputTypeName()
	{
		return m_in_type_name.c_str();
	}

	void GetLengthRequest::SetInputTypeName(const char* typeName)
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

	const char* GetLengthRequest::GetInputSourceName()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void GetLengthRequest::SetInputSourceName(const char* sourceName)
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

	const char* GetLengthRequest::GetOutputTypeName()
	{
		return m_out_type_name.c_str();
	}

	void GetLengthRequest::SetOutputTypeName(const char* typeName)
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

	const char* GetLengthRequest::GetOutputSourceName()
	{
		return m_out_source_name.empty() ? NULL : m_out_source_name.c_str();
	}

	void GetLengthRequest::SetOutputSourceName(const char* sourceName)
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

	void GetLengthRequest::SetOutputFormat(const char* format)
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

	const char* GetLengthRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	GFilter* GetLengthRequest::GetFilter()
	{
		return m_pFilter;
	}

	void GetLengthRequest::SetFilter(XElement* pxFilter)
	{

	}

	void GetLengthRequest::Info()
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
