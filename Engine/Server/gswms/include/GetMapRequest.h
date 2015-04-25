#ifndef __AUGE_WMS_GETMAP_REQUEST_H__
#define __AUGE_WMS_GETMAP_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>
#include "WMapRequest.h"

namespace auge
{
	class GetMapRequest : public WMapRequest
	{
	public:
		GetMapRequest();
		virtual ~GetMapRequest();

	public:
		//virtual const char*		GetEngine();
		//virtual const char*		GetVersion();
		virtual const char*		GetRequest();

	public:

		bool		Create(rude::CGI& cgi);

		GEnvelope&	GetExtent();
		g_int		GetWidth();
		g_int		GetHeight();
		const char* GetMimeType();
		GColor&		GetBgColor();
		bool		IsTransparent();
		g_uint		GetLayerCount();
		const char*	GetLayer(g_uint i);
		const char* GetStyle(g_uint i);

		void		SetVersion(const char* value);
		void		SetLayers(const char* value);
		void		SetStyles(const char* value);
		void		SetBBox(const char* value);
		void		SetWidth(const char* value);
		void		SetHeight(const char* value);
		void		SetBgColor(const char* value);
		void		SetTransparent(const char* value);

	private:
		std::string m_version;
		GEnvelope	m_extent;
		g_int		m_width;
		g_int		m_height;
		std::string m_mime_type;
		GColor		m_bgColor;
		bool		m_transparent;

		std::vector<std::string> m_layers;
		std::vector<std::string> m_styles;
	};
}


#endif //__AUGE_WMS_GETMAP_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true
user=user1&servicename=world&service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true

service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true&mapName=world
http://127.0.0.1:8088/ows/user1/mgr?service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true&mapName=world

http://127.0.0.1:8088/ows/user1/world/ims?service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true
http://192.168.111.160:8088//ows/user1/world/ims?service=wms&version=1.3.0&request=GetMap&layers=cities&bbox=-180,-90,180,90&width=800&height=600&transparent=true

************************************************************************/
