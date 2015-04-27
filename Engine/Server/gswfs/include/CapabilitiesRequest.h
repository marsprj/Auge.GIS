#ifndef __AUGE_WFS_GETCAPABILITIES_REQUEST_H__
#define __AUGE_WFS_GETCAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include "WFeatureRequest.h"

namespace auge
{
	class CapabilitiesRequest : public WFeatureRequest
	{
	public:
		CapabilitiesRequest();
		virtual ~CapabilitiesRequest();
	public:
		//virtual const char*		GetEngine();
		//virtual const char*		GetUser();
		//virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		//virtual const char*		GetMimeType();
		//virtual const char*		GetServiceName();

		//virtual const char*		GetHost();
		//virtual const char*		GetRequestMethod();

	public:
		bool		Create(rude::CGI& cgi);
		//void		SetVersion(const char* value);
		//void		SetUser(const char* user);
		//void		SetServiceName(const char* serviceName);

		//void		SetHost(const char* host);
		//void		SetRequestMethod(const char* method);

	private:
		//std::string m_user;
		//std::string m_version;
		//std::string m_mime_type;
		//std::string m_host;
		//std::string m_request_method;
		//std::string m_service_name;

		std::string m_name;
	};
}

#endif //__AUGE_WFS_GETCAPABILITIES_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wfs&version=1.0.0&request=GetCapabilities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCapabilities
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCapabilities&sourceName=world
user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCapabilities&mapName=world
http://127.0.0.1:8088/ows/user1/mgr?user=user1&servicename=world&service=wfs&version=1.0.0&request=GetCapabilities&mapName=world

http://127.0.0.1:8088/ows/user1/world/ims?service=wfs&version=1.0.0&request=GetCapabilities

************************************************************************/