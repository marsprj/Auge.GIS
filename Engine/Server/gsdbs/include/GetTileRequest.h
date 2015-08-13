#ifndef __AUGE_DBS_GET_TILE_REQUEST_H__
#define __AUGE_DBS_GET_TILE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetTileRequest : public WebRequest
	{
	public:
		GetTileRequest();
		virtual ~GetTileRequest();

	public:
		
		virtual const char*		GetEngine();
		virtual const char*		GetUser();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetSourceName();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char* GetStoreName();
		const char*	GetStyle();
		const char*	GetFormat();
		const char*	GetMatrixSet();
		g_int		GetLevel();
		g_int		GetRow();
		g_int		GetCol();

	public:
		bool		Create(rude::CGI& cgi);


		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);

		void		SetVersion(const char* value);
		void		SetUser(const char* user);
		void		SetSourceName(const char* SourceName);


		void		SetStoreName(const char* name);
		void		SetStyle(const char* style);
		void		SetFormat(const char* format);
		void		SetMatrixSet(const char* matrixset);
		void		SetLevel(const char* level);
		void		SetRow(const char* row);
		void		SetCol(const char* col);

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_source_name;
		std::string m_encoding;

		std::string m_store_name;
		std::string	m_style;
		//std::string m_format;
		std::string m_matrix_set;
		g_int		m_level;
		g_int		m_row;
		g_int		m_col;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_DBS_GET_TILE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1

service=wmts&request=GetTile&version=1.0.0&layer=world&style=Default&format=image/png&tilematrixset=pgis_tile_store&tilematrix=1&tilerow=0&tilecol=1
http://127.0.0.1:8088/ows/user1/world/mgr?service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1
http://127.0.0.1:8088/ows/user1/world/mgr?service=wmts&request=GetTile&version=1.0.0&layer=tmgo&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1

http://127.0.0.1:8088/ows/user1/mgr?service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1

************************************************************************/