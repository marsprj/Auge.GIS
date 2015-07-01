#ifndef __AUGE_IMS_REGISTER_MAP_REQUEST_H__
#define __AUGE_IMS_REGISTER_MAP_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class RegisterMapRequest : public WebRequest
	{
	public:
		RegisterMapRequest();
		virtual ~RegisterMapRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetName();
		const char*	GetServiceName();
		const char*	GetMapName();

		void		SetVersion(const char* value);
		void		SetServiceName(const char* name);
		void		SetMapName(const char* name);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_servcie_name;
		std::string m_map_name;
		std::string m_encoding;
	};
}

#endif //__AUGE_IMS_REGISTER_MAP_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=RegisterMap&serviceName=world&mapName=world
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=RegisterMap&serviceName=world&mapName=world
http://192.168.111.160:8088/ows/user1/mgr?service=ims&version=1.0.0&request=RegisterMap&serviceName=world&mapName=world

[ HTP Post ]
-------------------------------------------------------------------------
//<?xml version="1.0"?>
//<RegisterMap>
//	<ServiceName>world</ServiceName>
//	<Handlers>
//		<Handler name="wms"/>
//		<Handler name="wfs"/>
//	</Handlers>
//</RegisterMap>

************************************************************************/