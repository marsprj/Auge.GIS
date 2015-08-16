#ifndef __AUGE_GPS_GET_SPATIAL_REFERENCE_REQUEST_H__
#define __AUGE_GPS_GET_SPATIAL_REFERENCE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetSpatialReferenceRequest : public WebRequest
	{
	public:
		GetSpatialReferenceRequest();
		virtual ~GetSpatialReferenceRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

	public:
		g_int		GetSRID();
		g_uint		GetCount();
		g_uint		GetOffset();

		void		SetSRID(const char* srid);
		void		SetCount(const char* count);
		void		SetOffset(const char* offset);
		void		SetEncoding(const char* encoding);
		void		SetVersion(const char* version);
		void		SetUser(const char* user);

		bool		Create(rude::CGI& cgi);

	private:
		g_int		m_srid;
		g_uint		m_count;
		g_uint		m_offset;

		std::string	m_user;
		std::string	m_encoding;
		std::string	m_version;
		std::string m_mime_type;

		std::string m_host;
		std::string m_request_method;
	};
}

#endif //__AUGE_GPS_GET_SPATIAL_REFERENCE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=gps&version=1.0.0&request=GetSpatialReference&srid=4326
http://127.0.0.1:8088/ows/user1/mgr?service=gps&version=1.0.0&request=GetSpatialReference&srid=4326


************************************************************************/