#ifndef __AUGE_GPS_BUILD_PYRAMID_REQUEST_H__
#define __AUGE_GPS_BUILD_PYRAMID_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class BuildPyramidRequest : public WebRequest
	{
	public:
		BuildPyramidRequest();
		virtual ~BuildPyramidRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char* GetMapName();
		const char*	GetSourceName();
		const char* GetTileStoreName();

		g_uint		GetStartLevel();
		g_uint		GetEndLevel();
		
		const char* GetOutputFormat();

	public:
		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);

		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);

		void		SetTypeName(const char* typeName);
		void		SetMapName(const char* mapName);
		void		SetSourceName(const char* sourceName);
		void		SetTileStoreName(const char* tileStoreName);

		void		SetStartLevel(const char* level);
		void		SetEndLevel(const char* level);

		void		SetOutputFormat(const char* format);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_output_format;
		std::string m_encoding;

		std::string m_type_name;

		std::string m_map_name;
		std::string m_source_name;

		std::string m_tile_store_name;

		g_int		m_start_level;
		g_int		m_end_level;
		
	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_BUILD_PYRAMID_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=BuildPyramid&mapName=world&sourceName=tfs&tileStore=store1&start=1&end=5
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=BuildPyramid&mapName=world&sourceName=tfs&tileStore=store1&start=1&end=5
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=BuildPyramid&mapName=world&sourceName=tfs&tileStore=store1&level=1&row=0&col=1

************************************************************************/
