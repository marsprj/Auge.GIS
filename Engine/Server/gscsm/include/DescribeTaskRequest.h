#ifndef __AUGE_CSM_DESCRIBE_TASK_REQUEST_H__
#define __AUGE_CSM_DESCRIBE_TASK_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class DescribeTaskRequest : public WebRequest
	{
	public:
		DescribeTaskRequest();
		virtual ~DescribeTaskRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetName();
	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetName(const char* name);
		void		SetRole(const char* role);

		g_int		GetRole();
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_name;
		std::string m_encoding;
		g_int		m_role;	//0	owner
							//1 joiner
							//-1 all(default)
	};
}

#endif //__AUGE_CSM_DESCRIBE_TASK_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ims&version=1.0.0&request=DescribeTask&name=world
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=DescribeTask&name=world
http://127.0.0.1:8088/ows/user1/mgr?service=csm&version=1.0.0&request=DescribeTask
http://127.0.0.1:8088/ows/user1/mgr?service=csm&version=1.0.0&request=DescribeTask&role=owner
http://127.0.0.1:8088/ows/user1/mgr?service=csm&version=1.0.0&request=DescribeTask&role=joiner

************************************************************************/