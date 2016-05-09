#include "GetJobRequest.h"

namespace auge
{
	GetJobRequest::GetJobRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	GetJobRequest::~GetJobRequest()
	{

	}

	bool GetJobRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetUser(cgi["user"]);
		SetState(cgi["state"]);
		SetMaxJobs(cgi["maxJobs"]);
		SetOffset(cgi["offset"]);
		SetUUID(cgi["uuid"]);
		return true;
	}

	const char*	GetJobRequest::GetEngine()
	{
		return "ims";
	}

	const char*	GetJobRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	GetJobRequest::GetRequest()
	{
		return "GetJob";
	}

	const char* GetJobRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void GetJobRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	const char*	GetJobRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	//bool GetJobRequest::Create(GPropertySet& props)
	//{
	//	const char* val = NULL;
	//	val = props.GetValue("version");
	//	SetVersion(val);
	//	return true;
	//}

	void GetJobRequest::SetUser(const char* user)
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

	const char*	GetJobRequest::GetUser()
	{
		if(m_user.empty())
		{
			return NULL;
		}
		return m_user.c_str();
	}

	void GetJobRequest::SetState(const char* state)
	{
		if(state==NULL)
		{
			m_state = augeStateUnknown;
		}
		else
		{
			if(g_stricmp(state,"Running")==0)
			{
				m_state = augeStateRunning;
			}
			else if(g_stricmp(state,"Finished")==0)
			{
				m_state = augeStateFinished;
			}
			if(g_stricmp(state,"Canceled")==0)
			{
				m_state = augeStateCanceled;
			}
			
		}
	}

	void GetJobRequest::SetMaxJobs(const char* maxJobs)
	{
		if(maxJobs==NULL)
		{
			m_maxJobs = 20;
		}
		else
		{
			m_maxJobs = atoi(maxJobs);
		}
	}

	void GetJobRequest::SetOffset(const char* offset)
	{
		if(offset==NULL)
		{
			m_offset = 0;
		}
		else
		{
			m_offset = atoi(offset);
		}
	}

	augeProcssState	GetJobRequest::GetState()
	{
		return m_state;
	}

	g_uint GetJobRequest::GetMaxJobs()
	{
		return m_maxJobs;
	}

	g_uint GetJobRequest::GetOffset()
	{
		return m_offset;
	}

	void GetJobRequest::SetUUID(const char* uuid)
	{
		if(uuid!=NULL)
		{
			m_uuid = uuid;
		}
	}

	const char* GetJobRequest::GetUUID()
	{
		return m_uuid.empty() ? NULL : m_uuid.c_str();
	}
}