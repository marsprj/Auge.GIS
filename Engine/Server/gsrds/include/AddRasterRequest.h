#ifndef __AUGE_RDS_ADD_RASTERREQUEST_H__
#define __AUGE_RDS_ADD_RASTERREQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class AddRasterRequest : public WebRequest
	{
	public:
		AddRasterRequest();
		virtual ~AddRasterRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char* GetName();
		const char*	GetPath();
		const char* GetSourceName();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* version);
		void		SetPath(const char* path);
		void		SetName(const char* name);
		void		SetSourceName(const char* sourceName);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_name;
		std::string m_path;
		std::string m_source_name;
	};
}

#endif //__AUGE_RDS_ADD_RASTERREQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=rds&version=1.0.0&request=AddRaster&name=Koala.png&Path=/Koala.png
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=AddRaster&name=Koala.png&Path=/Koala.png

************************************************************************/