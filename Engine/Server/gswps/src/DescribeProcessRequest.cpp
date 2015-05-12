#include "DescribeProcessRequest.h"

namespace auge
{
	std::string DescribeProcessRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string DescribeProcessRequest::DEFAULT_METHOD = "wps";

	DescribeProcessRequest::DescribeProcessRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	DescribeProcessRequest::~DescribeProcessRequest()
	{

	}

	const char*	DescribeProcessRequest::GetEngine()
	{
		return "wps";
	}

	const char*	DescribeProcessRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	DescribeProcessRequest::GetRequest()
	{
		return "DescribeProcess";
	}

	void DescribeProcessRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	DescribeProcessRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool DescribeProcessRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetLanguage(cgi["language"]);
		SetIdentifiers(cgi["identifier"]);
		return true;
	}

	void DescribeProcessRequest::SetUser(const char* user)
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

	const char* DescribeProcessRequest::GetUser()
	{
		return m_user.c_str();
	}

	void DescribeProcessRequest::SetRequestMethod(const char* method)
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

	void DescribeProcessRequest::SetHost(const char* host)
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

	const char* DescribeProcessRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* DescribeProcessRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char*	DescribeProcessRequest::GetLanguage()
	{
		return m_language.c_str();
	}

	const char*	DescribeProcessRequest::GetIdentifier(g_uint i)
	{
		if(i>=m_identifiers.size())
		{
			return NULL;
		}
		return m_identifiers[i].c_str();
	}

	g_uint DescribeProcessRequest::GetIdentifierCount()
	{
		return m_identifiers.size();
	}

	void DescribeProcessRequest::SetLanguage(const char* language)
	{
		if(language==NULL)
		{
			m_language.clear();
		}
		else
		{
			m_language = language;
		}
	}

	void DescribeProcessRequest::SetIdentifiers(const char* identifiers)
	{
		if(identifiers==NULL)
		{
			m_identifiers.clear();
			return;
		}

		if(identifiers[0]=='\0')
		{
			m_identifiers.clear();
			return;
		}

		char* str = strdup(identifiers);
		char* ptr = strtok(str, ",");
		while(ptr!=NULL)
		{
			m_identifiers.push_back(ptr);

			ptr = strtok(NULL, ",");
		}
		free(str);
	}

	void DescribeProcessRequest::Info()
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
