#ifndef __AUGE_IMS_GET_ONELINE_USER_REQUEST_H__
#define __AUGE_IMS_GET_ONELINE_USER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetOnlineUserRequest : public WebRequest
	{
	public:
		GetOnlineUserRequest();
		virtual ~GetOnlineUserRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();
		virtual void			Release();

	public:
		bool		Create(rude::CGI& cgi);
		void		SetCount(const char* count);
		void		SetOffset(const char* offset);
		g_uint		GetCount();
		g_uint		GetOffset();

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;
		g_uint		m_count;
		g_uint		m_offset;
	};
}

#endif //__AUGE_IMS_GET_ONELINE_USER_REQUEST_H__

/************************************************************************
[ HTTP Get ]
service=was&version=1.0.0&request=GetOnlineUser&count=10&offset=0
http://127.0.0.1:8088/ows/admin/mgr?service=was&version=1.0.0&request=GetOnlineUser&count=10&offset=0


************************************************************************/