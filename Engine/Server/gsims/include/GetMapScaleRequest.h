#ifndef __AUGE_IMS_GET_MAPSCALE_REQUEST_H__
#define __AUGE_IMS_GET_MAPSCALE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetMapScaleRequest : public WebRequest
	{
	public:
		GetMapScaleRequest();
		virtual ~GetMapScaleRequest();

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
		g_uint		GetScreenWidth();
		float		GetMapWidth();

		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		void		SetSRID(const char* srid);
		void		SetMapWidth(const char* mapWidth);
		void		SetScreenWidth(const char* screenHeight);

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
		g_int m_srid;
		float m_map_width;
		g_uint m_screen_width;

		std::string m_name;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_IMS_GET_MAPSCALE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=ims&version=1.0.0&request=GetMapScale&srid=4326&mapWidth=80&screenWidth=1280
http://127.0.0.1:8088/ows/admin/mgr?service=ims&version=1.0.0&request=GetMapScale&srid=4326&mapWidth=80&screenWidth=1280
http://192.168.111.160:8088/ows/admin/mgr?service=ims&version=1.0.0&request=GetMapScale
http://123.57.207.198/ows/admin/mgr?service=ims&version=1.0.0&request=GetMapScale

************************************************************************/
