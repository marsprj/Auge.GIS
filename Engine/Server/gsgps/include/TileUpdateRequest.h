#ifndef __AUGE_GPS_UPATE_TILE_REQUEST_H__
#define __AUGE_GPS_UPATE_TILE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class UpdateTileRequest : public WebRequest
	{
	public:
		UpdateTileRequest();
		virtual ~UpdateTileRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();


		const char* GetTypeName();
		const char* GetMapName();
		const char*	GetSourceName();
		const char* GetTileStoreName();
		
		g_uint		GetLevel();
		g_uint		GetRow();
		g_uint		GetCol();

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

		void		SetOutputFormat(const char* format);
		void		SetLevel(const char* level);
		void		SetRow(const char* row);
		void		SetCol(const char* col);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;

		std::string m_type_name;

		std::string m_map_name;
		std::string m_source_name;
		
		std::string m_tile_store_name;


		std::string m_output_format;

		g_int		m_level;
		g_int		m_row;
		g_int		m_col;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_UPATE_TILE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=UpdateTile&mapName=world&sourceName=tfs&tileStore=store1&level=1&row=0&col=1
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=UpdateTile&mapName=world&sourceName=tfs&tileStore=store1&level=1&row=0&col=1
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=UpdateTile&mapName=world&sourceName=tfs&tileStore=store1&level=1&row=0&col=1

************************************************************************/
