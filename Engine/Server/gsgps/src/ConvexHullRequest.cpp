#include "ConvexHullRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string ConvexHullRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string ConvexHullRequest::DEFAULT_METHOD = "gps";

	ConvexHullRequest::ConvexHullRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_pFilter(NULL),
		m_output_format("GML2"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	ConvexHullRequest::~ConvexHullRequest()
	{
		if(m_pFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFilter);
		}
	}

	const char*	ConvexHullRequest::GetEngine()
	{
		return "gps";
	}

	const char*	ConvexHullRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	ConvexHullRequest::GetRequest()
	{
		return "ConvexHull";
	}

	void ConvexHullRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	ConvexHullRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool ConvexHullRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetInTypeName(cgi["typeName"]);
		SetInSourceName(cgi["sourceName"]);

		return true;
	}

	const char*	ConvexHullRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void ConvexHullRequest::SetUser(const char* user)
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

	const char* ConvexHullRequest::GetUser()
	{
		return m_user.c_str();
	}

	void ConvexHullRequest::SetRequestMethod(const char* method)
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

	void ConvexHullRequest::SetHost(const char* host)
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

	const char* ConvexHullRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* ConvexHullRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* ConvexHullRequest::GetTypeName()
	{
		return m_in_type_name.c_str();
	}

	void ConvexHullRequest::SetInTypeName(const char* typeName)
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

	const char* ConvexHullRequest::GetSourceName()
	{
		return m_in_source_name.empty() ? NULL : m_in_source_name.c_str();
	}

	void ConvexHullRequest::SetInSourceName(const char* sourceName)
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

	void ConvexHullRequest::SetOutputFormat(const char* format)
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

	const char* ConvexHullRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	GFilter* ConvexHullRequest::GetFilter()
	{
		return m_pFilter;
	}

	void ConvexHullRequest::SetFilter(XElement* pxFilter)
	{

	}

	void ConvexHullRequest::Info()
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
