#include "JoinTaskRequest.h"
#include "AugeWebCore.h"

namespace auge
{
	JoinTaskRequest::JoinTaskRequest()
	{
		m_version = "1.0.0";
		m_mime_type = "text/xml";
		m_encoding = AUGE_DEFAULT_ENCODING;
	}

	JoinTaskRequest::~JoinTaskRequest()
	{

	}

	bool JoinTaskRequest::Create(rude::CGI& cgi)
	{
		SetVersion(cgi["version"]);
		SetTaskID(cgi["taskid"]);

		return true;
	}

	const char*	JoinTaskRequest::GetMimeType()
	{
		return m_mime_type.c_str();
	}

	const char*	JoinTaskRequest::GetEngine()
	{
		return "csm";
	}

	const char*	JoinTaskRequest::GetVersion()
	{
		return m_version.c_str();
	}

	const char*	JoinTaskRequest::GetRequest()
	{
		return "JoinTask";
	}

	const char* JoinTaskRequest::GetEncoding()
	{
		return m_encoding.c_str();
	}

	void JoinTaskRequest::SetVersion(const char* value)
	{
		if(value==NULL)
		{
			return;
		}
		m_version = value;
	}

	void JoinTaskRequest::SetTaskID(const char* taskid)
	{
		if(taskid==NULL)
		{
			m_tid = -1;
		}
		else if(strlen(taskid)==0)
		{
			m_tid = -1;
		}
		else
		{
			m_tid = atoi(taskid);
		}

	}

	g_uint JoinTaskRequest::GetTaskID()
	{
		return m_tid;
	}

}