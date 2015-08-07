#ifndef __AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_REQUEST_H__
#define __AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetSpatialReferenceCountRequest : public WebRequest
	{
	public:
		GetSpatialReferenceCountRequest();
		virtual ~GetSpatialReferenceCountRequest();

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
		void		SetEncoding(const char* encoding);
		void		SetVersion(const char* version);
		void		SetUser(const char* user);

		bool		Create(rude::CGI& cgi);

	private:
		std::string	m_user;
		std::string	m_encoding;
		std::string	m_version;
		std::string m_mime_type;

		std::string m_host;
		std::string m_request_method;
	};
}

#endif //__AUGE_GPS_GET_SPATIAL_REFERENCE_COUNT_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=gps&version=1.0.0&request=GetSpatialReferenceCount
http://127.0.0.1:8088/ows/user1/mgr?service=gps&version=1.0.0&request=GetSpatialReferenceCount


************************************************************************/