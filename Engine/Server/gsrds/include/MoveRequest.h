#ifndef __AUGE_RDS_MOVE_REQUEST_H__
#define __AUGE_RDS_MOVE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class MoveRequest : public WebRequest
	{
	public:
		MoveRequest();
		virtual ~MoveRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char*	GetSrc();
		const char*	GetDes();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetSrc(const char* path);
		void		SetDes(const char* path);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_src_path;
		std::string m_des_path;
	};
}

#endif //__AUGE_RDS_MOVE_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=rds&version=1.0.0&request=Move&src=/aa/aqi_ranking.txt&des=/bb.txt
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=Move&src=/aa/aqi_ranking.txt&des=/bb.txt

service=rds&version=1.0.0&request=Move&src=/aa/aqi_ranking.txt&des=/bb.txt
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=Move&src=/aa/&des=/cc/addd

************************************************************************/