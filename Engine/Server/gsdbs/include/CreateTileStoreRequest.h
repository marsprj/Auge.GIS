#ifndef __AUGE_IMS_CREATE_TILESTORE_REQUEST_H__
#define __AUGE_IMS_CREATE_TILESTORE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeTile.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class CreateTileStoreRequest : public WebRequest
	{
	public:
		CreateTileStoreRequest();
		virtual ~CreateTileStoreRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetSourceName();
		virtual const char*		GetStoreName();
		augeTileType			GetType();
	public:
		bool		Create(rude::CGI& cgi);
		
		void		SetVersion(const char* value);
		void		SetSourceName(const char* name);
		void		SetStoreName(const char* name);
		void		SetType(const char* type);
		void		SetExtent(const char* extent);
		void		SetStartLevel(const char* level);
		void		SetEndLevel(const char* level);

		GEnvelope&	GetExtent();
		g_uint		GetStartLevel();
		g_uint		GetEndLevel();

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_source_name;
		std::string m_store_name;
		std::string m_encoding;
		g_uint		m_start_level;
		g_uint		m_end_level;
		augeTileType m_type;
		GEnvelope	m_extent;
	};
}

#endif //__AUGE_IMS_CREATE_TILESTORE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
user=user1&service=dbs&version=1.0.0&request=CreateTileStore&sourceName=tfs&storeName=google&type=GoogleCRS84Quad
http://127.0.0.1:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=CreateTileStore&sourceName=tfs&storeName=google&type=GoogleCRS84Quad
http://127.0.0.1:8088/ows/user1/mgr?service=dbs&version=1.0.0&request=CreateTileStore&sourceName=tfs&storeName=google&type=GoogleCRS84Quad&extent=0,0,180,90&startlevel=3&endlevel=5

************************************************************************/