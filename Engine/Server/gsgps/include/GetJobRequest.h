#ifndef __AUGE_IMS_GET_JOB_REQUEST_H__
#define __AUGE_IMS_GET_JOB_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeWebCore.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class Job;
	class EnumJob;

	class GetJobRequest : public WebRequest
	{
	public:
		GetJobRequest();
		virtual ~GetJobRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char*		GetUser();
		augeProcssState	GetState();
		g_uint			GetMaxJobs();
		g_uint			GetOffset();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetUser(const char* user);
		void		SetState(const char* state);
		void		SetMaxJobs(const char* maxJobs);
		void		SetOffset(const char* offset);
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_user;
		augeProcssState m_state;
		g_uint		m_maxJobs;
		g_uint		m_offset;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_GET_JOB_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=gps&version=1.0.0&request=GetJob&state=Running&maxJobs=20&offset=0
http://127.0.0.1:8088/ows/user1/mgr?service=gps&version=1.0.0&request=GetJob&name=world

************************************************************************/