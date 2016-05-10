#include "JobStatisticsRequest.h"

namespace auge
{
	JobStatisticsRequest::JobStatisticsRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	JobStatisticsRequest::~JobStatisticsRequest()
	{

	}

	bool JobStatisticsRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetField(cgi["field"]);
		SetServer(cgi["server"]);
		SetStartTime(cgi["startTime"]);
		SetEndTime(cgi["endTime"]);
		return true;
	}

	const char*	JobStatisticsRequest::GetEngine()
	{
		return "ims";
	}

	const char*	JobStatisticsRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	JobStatisticsRequest::GetRequest()
	{
		return "JobStatistics";
	}

	const char* JobStatisticsRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void JobStatisticsRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	JobStatisticsRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool JobStatisticsRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void JobStatisticsRequest::SetUser(const char* user)
	{
		if(user==NULL)
		{
			m_user.clear();
		}
		else if(strlen(user)==0)
		{
			m_user.clear();
		}
		else
		{
			m_user = user;
		}
	}

	const char*	JobStatisticsRequest::GetUser()
	{
		if(m_user.empty())
		{
			return NULL;
		}
		return m_user.c_str();
	}

	void JobStatisticsRequest::SetField(const char* field)
	{
		if(field==NULL)
		{
			m_field.clear();
		}
		else if(strlen(field)==0)
		{
			m_field.clear();
		}
		else
		{
			m_field = field;
		}
	}

	const char*	JobStatisticsRequest::GetField()
	{
		if(m_field.empty())
		{
			return NULL;
		}
		return m_field.c_str();
	}
	
	const char*	JobStatisticsRequest::GetStartTime()
	{
		if(m_start_time.empty())
		{
			return NULL;
		}
		return m_start_time.c_str();
	}

	const char*	JobStatisticsRequest::GetEndTime()
	{
		if(m_end_time.empty())
		{
			return NULL;
		}
		return m_end_time.c_str();
	}

	void JobStatisticsRequest::SetStartTime(const char* startTime)
	{
		if(startTime==NULL)
		{
			m_start_time.clear();
		}
		else if(strlen(startTime)==0)
		{
			m_start_time.clear();
		}
		else
		{
			m_start_time = startTime;
		}
	}

	void JobStatisticsRequest::SetEndTime(const char* endTime)
	{
		if(endTime==NULL)
		{
			m_end_time.clear();
		}
		else if(strlen(endTime)==0)
		{
			m_end_time.clear();
		}
		else
		{
			m_end_time = endTime;
		}
	}

	void JobStatisticsRequest::SetServer(const char* server)
	{
		if(server==NULL)
		{
			m_server.clear();
		}
		else if(strlen(server)==0)
		{
			m_server.clear();
		}
		else
		{
			m_server = server;
		}
	}

	const char*	JobStatisticsRequest::GetServer()
	{
		return m_server.c_str();
	}

}