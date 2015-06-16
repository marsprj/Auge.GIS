#include "BufferRequest.h"
#include "AugeFilter.h"

namespace auge
{
	std::string BufferRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string BufferRequest::DEFAULT_METHOD = "gps";

	BufferRequest::BufferRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_pFilter(NULL),
		m_output_format("GML2"),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD),
		m_distance(-1.0f)
	{

	}

	BufferRequest::~BufferRequest()
	{
		if(m_pFilter!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pFilter);
		}
	}

	const char*	BufferRequest::GetEngine()
	{
		return "gps";
	}

	const char*	BufferRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	BufferRequest::GetRequest()
	{
		return "Buffer";
	}

	void BufferRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	BufferRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool BufferRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);

		SetTypeName(cgi["typeName"]);
		SetSourceName(cgi["sourceName"]);

		SetDistance(cgi["distance"]);
		SetDistanceField(cgi["distancefield"]);

		return true;
	}

	double BufferRequest::GetDistance()
	{
		return m_distance;
	}

	const char* BufferRequest::GetDistanceField()
	{
		return m_distance_field.empty() ? NULL : m_distance_field.c_str();
	}

	void BufferRequest::SetDistance(const char* distance)
	{
		if(distance==NULL)
		{
			m_distance = -1.0f;
		}
		else if(!isdigit(distance[0]))
		{
			m_distance = -1.0f;
		}
		else
		{
			m_distance = atof(distance);
		}
	}

	void BufferRequest::SetDistanceField(const char* distance_field)
	{
		if(distance_field==NULL)
		{
			m_distance_field.clear();
		}
		else
		{
			m_distance_field = distance_field;
		}
	}

	void BufferRequest::SetUser(const char* user)
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

	const char* BufferRequest::GetUser()
	{
		return m_user.c_str();
	}

	void BufferRequest::SetRequestMethod(const char* method)
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

	void BufferRequest::SetHost(const char* host)
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

	const char* BufferRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* BufferRequest::GetHost()
	{
		return m_host.c_str();
	}

	const char* BufferRequest::GetTypeName()
	{
		return m_type_name.c_str();
	}

	void BufferRequest::SetTypeName(const char* typeName)
	{
		if(typeName==NULL)
		{
			m_type_name.clear();
		}
		else
		{
			m_type_name = typeName;
		}
	}

	const char* BufferRequest::GetSourceName()
	{
		return m_source_name.empty() ? NULL : m_source_name.c_str();
	}

	void BufferRequest::SetSourceName(const char* sourceName)
	{
		if(sourceName==NULL)
		{
			m_source_name.clear();
		}
		else
		{
			m_source_name = sourceName;
		}
	}

	void BufferRequest::SetOutputFormat(const char* format)
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

	const char* BufferRequest::GetOutputFormat()
	{
		return m_output_format.c_str();
	}

	GFilter* BufferRequest::GetFilter()
	{
		return m_pFilter;
	}

	void BufferRequest::SetFilter(XElement* pxFilter)
	{

	}

	void BufferRequest::Info()
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
