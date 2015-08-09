#ifndef __AUGE_RDS_LIST_REQUEST_H__
#define __AUGE_RDS_LIST_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class ListRequest : public WebRequest
	{
	public:
		ListRequest();
		virtual ~ListRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char*	GetPath();
		const char* GetSourceName();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* value);
		void		SetSourceName(const char* sourceName);
		void		SetPath(const char* path);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_path;
		std::string m_source_name;
	};
}

#endif //__AUGE_RDS_LIST_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
user=user1&service=rds&version=1.0.0&request=List&sourceName=rsdb2&Path=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=List&sourceName=rsdb&Path=/aaaa

http://182.92.114.80:8088/ows/user1/mgr?service=rds&version=1.0.0&request=List&sourceName=rsdb2&Path=/

************************************************************************/