#ifndef __AUGE_IMS_JOB_STATISTICS_REQUEST_H__
#define __AUGE_IMS_JOB_STATISTICS_REQUEST_H__

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

	class JobStatisticsRequest : public WebRequest
	{
	public:
		JobStatisticsRequest();
		virtual ~JobStatisticsRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char*		GetUser();
		const char*		GetField();
		const char*		GetStartTime();
		const char*		GetEndTime();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetUser(const char* user);
		void		SetField(const char* field);
		void		SetStartTime(const char* startTime);
		void		SetEndTime(const char* endTime);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_user;
		std::string m_field;
		std::string m_encoding;
		std::string m_start_time;
		std::string m_end_time;
	};
}

#endif //__AUGE_IMS_JOB_STATISTICS_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ims&version=1.0.0&request=JobStatistics&fields=client
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=JobStatistics&field=operation

user=user1&service=ims&version=1.0.0&request=JobStatistics&fields=client&start_time=2015-01-01 00:00:00&end_time=2016-1-01 00:00:00
http://192.168.111.152:8088/ows/user1/mgr?service=ims&version=1.0.0&request=JobStatistics&state=Running&maxJobs=20&offset=0

************************************************************************/