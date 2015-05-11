#include "GetColorMapRequest.h"

namespace auge
{
	std::string GetColorMapRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GetColorMapRequest::DEFAULT_METHOD = "wms"; 

	GetColorMapRequest::GetColorMapRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_count(2),
		m_id(-1),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	GetColorMapRequest::~GetColorMapRequest()
	{

	}

	const char*	GetColorMapRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetColorMapRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetColorMapRequest::GetRequest()
	{
		return "GetColorMap";
	}

	void GetColorMapRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetColorMapRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetColorMapRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetID(cgi["id"]);
		SetCount(cgi["count"]);
		return true;
	}

	void GetColorMapRequest::SetUser(const char* user)
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

	const char* GetColorMapRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetColorMapRequest::SetRequestMethod(const char* method)
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

	void GetColorMapRequest::SetHost(const char* host)
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

	void GetColorMapRequest::SetID(const char* id)
	{
		if(id==NULL)
		{
			m_id = -1;
		}
		else
		{
			if(isdigit(id[0]))
			{
				m_id = atoi(id);
			}
			else
			{
				m_id = -1;
			}			
		}
	}

	void GetColorMapRequest::SetCount(const char* count)
	{
		if(count==NULL)
		{
			m_count = 2;
		}
		else
		{
			if(isdigit(count[0]))
			{
				m_count = atoi(count);
			}
			else
			{
				m_count = 2;
			}
		}
	}

	g_int GetColorMapRequest::GetID()
	{
		return m_id;
	}

	g_uint GetColorMapRequest::GetCount()
	{
		return m_count;
	}

	const char* GetColorMapRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetColorMapRequest::GetHost()
	{
		return m_host.c_str();
	}

	void GetColorMapRequest::Info()
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
