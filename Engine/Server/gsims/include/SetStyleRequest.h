#ifndef __AUGE_IMS_LAYER_SETSTYLE_REQUEST_H__
#define __AUGE_IMS_LAYER_SETSTYLE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class SetStyleRequest : public WebRequest
	{
	public:
		SetStyleRequest();
		virtual ~SetStyleRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();

	public:
		bool		Create(rude::CGI& cgi);

		const char*	GetName();
		const char*	GetMapName();
		const char* GetLayerName();
		const char* GetStyleName();

		void		SetVersion(const char* value);
		void		SetMapName(const char* mapName);
		void		SetLayerName(const char* layerName);
		void		SetStyleName(const char* styleName);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_map_name;
		std::string m_layer_name;
		std::string m_style_name;
	};
}

#endif //__AUGE_IMS_LAYER_SETSTYLE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=ims&version=1.0.0&request=SetStyle&map=world&&layer=cities2222&style=line
http://127.0.0.1:8088/ows/user1/mgr?service=ims&version=1.0.0&request=SetStyle&map=world&&layer=cities&style=point
http://192.168.111.160:8088/ows/user1/mgr?service=ims&version=1.0.0&request=SetStyle&map=world&&layer=cities&style=point_label
http://123.57207.198/ows/user1/mgr?service=ims&version=1.0.0&request=SetStyle&map=cities_label&&layer=cities&style=point_label

[ HTP Post ]
-------------------------------------------------------------------------
//<?xml version="1.0"?>
//<SetStyle>
//	<ServiceName>world</ServiceName>
//	<Handlers>
//		<Handler name="wms"/>
//		<Handler name="wfs"/>
//	</Handlers>
//</SetStyle>

************************************************************************/