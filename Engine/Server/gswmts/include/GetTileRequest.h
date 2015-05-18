#ifndef __AUGE_WMTS_GET_TILE_REQUEST_H__
#define __AUGE_WMTS_GET_TILE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>
#include "WTileRequest.h"

namespace auge
{
	class GetTileRequest : public WTileRequest
	{
	public:
		GetTileRequest();
		virtual ~GetTileRequest();

	public:
		virtual const char*		GetRequest();

		const char* GetStoreName();
		const char*	GetStyle();
		const char*	GetFormat();
		const char*	GetMatrixSet();
		g_int		GetLevel();
		g_int		GetRow();
		g_int		GetCol();

	public:
		bool		Create(rude::CGI& cgi);

	private:
		void		SetStoreName(const char* name);
		void		SetStyle(const char* style);
		void		SetFormat(const char* format);
		void		SetMatrixSet(const char* matrixset);
		void		SetLevel(const char* level);
		void		SetRow(const char* row);
		void		SetCol(const char* col);

	private:
		std::string m_store_name;
		std::string	m_style;
		//std::string m_format;
		std::string m_matrix_set;
		g_int		m_level;
		g_int		m_row;
		g_int		m_col;
	};

}

#endif //__AUGE_WMTS_GET_TILE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1

service=wmts&request=GetTile&version=1.0.0&layer=world&style=Default&format=image/png&tilematrixset=pgis_tile_store&tilematrix=1&tilerow=0&tilecol=1
http://127.0.0.1:8088/ows/user1/world/mgr?service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1
http://127.0.0.1:8088/ows/user1/world/mgr?service=wmts&request=GetTile&version=1.0.0&layer=tmgo&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1

http://127.0.0.1:8088/ows/user1/mgr?service=wmts&request=GetTile&version=1.0.0&layer=tfs&style=Default&format=image/png&tilematrixset=store1&tilematrix=1&tilerow=0&tilecol=1

************************************************************************/