#ifndef __AUGE_GPS_CAPABILITIES_REQUEST_H__
#define __AUGE_GPS_CAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GeoProcessingCapabilitiesRequest : public WebRequest
	{
	public:
		GeoProcessingCapabilitiesRequest();
		virtual ~GeoProcessingCapabilitiesRequest();

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
		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		
		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;

		std::string m_name;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_CAPABILITIES_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wps&version=1.0.0&request=GetCapabilities
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=GetCapabilities
http://192.168.111.151:8088/ows/admin/mgr?service=wps&version=1.0.0&request=GetCapabilities
http://123.57.207.198/ows/admin/mgr?service=wps&version=1.0.0&request=GetCapabilities

************************************************************************/
