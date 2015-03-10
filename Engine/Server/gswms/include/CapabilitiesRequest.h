#ifndef __AUGE_WMS_CAPABILITIES_REQUEST_H__
#define __AUGE_WMS_CAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CapabilitiesRequest : public WebRequest
	{
	public:
		CapabilitiesRequest();
		virtual ~CapabilitiesRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetUser();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetServiceName();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();
		
	public:
		bool		Create(rude::CGI& cgi);
		void		SetVersion(const char* value);
		void		SetUser(const char* user);
		void		SetServiceName(const char* serviceName);

		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_service_name;

		std::string m_name;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_WMS_CAPABILITIES_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wms&version=1.0.0&request=GetCapabilities
user=user1&servicename=world&service=wms&version=1.0.0&request=GetCapabilities
http://127.0.0.1:8088/user1/world/ims?service=wms&version=1.0.0&request=GetCapabilities

************************************************************************/