#ifndef __AUGE_CSM_JOIN_TASK_REQUEST_H__
#define __AUGE_CSM_JOIN_TASK_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class JoinTaskRequest : public WebRequest
	{
	public:
		JoinTaskRequest();
		virtual ~JoinTaskRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		g_uint		GetTaskID();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetTaskID(const char* task);
	private:
		std::string m_version;
		std::string m_mime_type;
		g_uint		m_tid;
		std::string m_encoding;
	};
}

#endif //__AUGE_CSM_JOIN_TASK_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user2&service=csm&version=1.0.0&request=JoinTask&taskid=2


http://127.0.0.1:8088/ows/user2/mgr?service=csm&version=1.0.0&request=JoinTask&taskid=2

************************************************************************/