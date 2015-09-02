#include "GetMapScaleRequest.h"

namespace auge
{
	std::string GetMapScaleRequest::DEFAULT_HOST   = "127.0.0.1";
	std::string GetMapScaleRequest::DEFAULT_METHOD = "wms"; 

	GetMapScaleRequest::GetMapScaleRequest():
	m_version("1.0.0"),
		m_mime_type("text/xml"),
		m_encoding(AUGE_DEFAULT_ENCODING),
		m_map_width(0.0f),
		m_screen_width(0),
		m_srid(AUGE_DEFAULT_SRID),
		m_host(DEFAULT_HOST),
		m_request_method(DEFAULT_METHOD)
	{

	}

	GetMapScaleRequest::~GetMapScaleRequest()
	{

	}

	const char*	GetMapScaleRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetMapScaleRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetMapScaleRequest::GetRequest()
	{
		return "GetMapScale";
	}

	const char* GetMapScaleRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetMapScaleRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetMapScaleRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	bool GetMapScaleRequest::Create(rude::CGI& cgi)
	{
		const char* val = NULL;
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetSRID(cgi["srid"]);
		SetMapWidth(cgi["mapWidth"]);
		SetScreenWidth(cgi["screenWidth"]);
		return true;
	}

	void GetMapScaleRequest::SetUser(const char* user)
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

	const char* GetMapScaleRequest::GetUser()
	{
		return m_user.c_str();
	}

	void GetMapScaleRequest::SetRequestMethod(const char* method)
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

	void GetMapScaleRequest::SetHost(const char* host)
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

	void GetMapScaleRequest::SetSRID(const char* srid)
	{
		if(srid==NULL)
		{
			m_srid = -1;
		}
		else
		{
			if(isdigit(srid[0]))
			{
				m_srid = atoi(srid);
			}
			else
			{
				m_srid = -1;
			}			
		}
	}

	void GetMapScaleRequest::SetMapWidth(const char* mapWidth)
	{
		if(mapWidth==NULL)
		{
			m_map_width = -1;
		}
		else
		{
			if(isdigit(mapWidth[0]))
			{
				m_map_width = atof(mapWidth);
			}
			else
			{
				m_map_width = 0.0f;
			}			
		}
	}

	void GetMapScaleRequest::SetScreenWidth(const char* screenWidth)
	{
		if(screenWidth==NULL)
		{
			m_screen_width = 0;
		}
		else
		{
			if(isdigit(screenWidth[0]))
			{
				m_screen_width = atoi(screenWidth);
			}
			else
			{
				m_screen_width = 0;
			}			
		}
	}

	g_int GetMapScaleRequest::GetSRID()
	{
		return m_srid;
	}

	g_uint GetMapScaleRequest::GetScreenWidth()
	{
		return m_screen_width;
	}

	float GetMapScaleRequest::GetMapWidth()
	{
		return m_map_width;
	}

	const char* GetMapScaleRequest::GetRequestMethod()
	{
		return m_request_method.c_str();
	}

	const char* GetMapScaleRequest::GetHost()
	{
		return m_host.c_str();
	}

	void GetMapScaleRequest::Info()
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
