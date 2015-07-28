#ifndef __AUGE_RDS_REMOVE_RASTERREQUEST_H__
#define __AUGE_RDS_REMOVE_RASTERREQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class RemoveRasterRequest : public WebRequest
	{
	public:
		RemoveRasterRequest();
		virtual ~RemoveRasterRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char* GetRasterName();
		const char*	GetRasterPath();
		const char* GetSourceName();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* version);
		void		SetRasterPath(const char* path);
		void		SetRasterName(const char* name);
		void		SetSourceName(const char* sourceName);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_raster_name;
		std::string m_raster_path;
		std::string m_source_name;
	};
}

#endif //__AUGE_RDS_REMOVE_RASTERREQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=rds&version=1.0.0&request=RemoveRaster&sourceName=rsdb&rasterName=Koala.png&rasterPath=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=RemoveRaster&sourceName=rsdb&rasterName=1.png&rasterPath=/

http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=RemoveRaster&sourceName=rsdb&rasterName=1.png&rasterPath=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=RemoveRaster&sourceName=rsdb&rasterName=Koala.png&rasterPath=/ccc

************************************************************************/