#ifndef __AUGE_IMS_GET_USER_COUNT_REQUEST_H__
#define __AUGE_IMS_GET_USER_COUNT_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetUserCountRequest : public WebRequest
	{
	public:
		GetUserCountRequest();
		virtual ~GetUserCountRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();
		virtual void			Release();

	public:
		bool		Create(rude::CGI& cgi);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_GET_USER_COUNT_REQUEST_H__

/************************************************************************
[ HTTP Get ]
service=was&version=1.0.0&request=GetUserCount
http://127.0.0.1:8088/ows/admin/mgr?service=was&version=1.0.0&request=GetUserCount


************************************************************************/