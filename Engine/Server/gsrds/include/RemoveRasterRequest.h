#ifndef __AUGE_RDS_GET_RASTER_REQUEST_H__
#define __AUGE_RDS_GET_RASTER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class GetRasterRequest : public WebRequest
	{
	public:
		GetRasterRequest();
		virtual ~GetRasterRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char* GetName();
		const char*	GetPath();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* version);
		void		SetPath(const char* path);
		void		SetName(const char* name);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_name;
		std::string m_path;
	};
}

#endif //__AUGE_RDS_GET_RASTER_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=rds&version=1.0.0&request=GetRaster&name=1.png&Path=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetRaster&name=1.png&Path=/

************************************************************************/