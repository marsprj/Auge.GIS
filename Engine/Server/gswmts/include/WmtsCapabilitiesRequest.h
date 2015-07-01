#ifndef __AUGE_WMTS_CAPABILITIES_REQUEST_H__
#define __AUGE_WMTS_CAPABILITIES_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <rude/cgi.h>
#include "WTileRequest.h"

namespace auge
{
	class WmtsCapabilitiesRequest : public WTileRequest
	{
	public:
		WmtsCapabilitiesRequest();
		virtual ~WmtsCapabilitiesRequest();

	public:
		virtual const char*		GetRequest();	
	public:
		bool		Create(rude::CGI& cgi);
	private:
	};

}

#endif //__AUGE_WMTS_CAPABILITIES_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wmts&version=1.0.0&request=GetCapabilities
service=wmts&version=1.0.0&request=GetCapabilities&mapName=world
http://127.0.0.1:8088/ows/user1/world/mgr?service=wmts&version=1.0.0&request=GetCapabilities
http://127.0.0.1:8088/ows/user1/mgr?service=wmts&version=1.0.0&request=GetCapabilities&mapName=world
user=user1&servicename=world&service=wmts&version=1.0.0&request=GetCapabilities 

http://127.0.0.1:8088/ows/user1/world/ims?service=wmts&version=1.0.0&request=GetCapabilities
http://192.168.111.160:8088/ows/user1/world/ims?service=wmts&version=1.0.0&request=GetCapabilities

service=wmts&version=1.0.0&request=GetCapabilities&mapName=tfs
http://127.0.0.1:8088/ows/user1/tfs/wmts?service=wmts&version=1.0.0&request=GetCapabilities

************************************************************************/