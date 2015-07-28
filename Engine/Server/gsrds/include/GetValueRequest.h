#ifndef __AUGE_RDS_GET_VALUE_REQUEST_H__
#define __AUGE_RDS_GET_VALUE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class GetValueRequest : public WebRequest
	{
	public:
		GetValueRequest();
		virtual ~GetValueRequest();

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
		g_int		GetIX();
		g_int		GetIY();
		double		GetX();
		double		GetY();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* version);
		void		SetPath(const char* path);
		void		SetRasterName(const char* name);
		void		SetSourceName(const char* sourceName);
		void		SetIX(const char* ix);
		void		SetIY(const char* iy);
		void		SetX(const char* x);
		void		SetY(const char* y);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_raster_name;
		std::string m_raster_path;
		std::string m_source_name;

		double	m_x;
		double	m_y;
		g_int	m_image_x;
		g_int	m_image_y;
	};
}

#endif //__AUGE_RDS_GET_VALUE_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=rds&version=1.0.0&request=GetValue&sourceName=rsdb&rasterName=1.png&rasterPath=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetValue&sourceName=rsdb&rasterName=1.png&rasterPath=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetValue&sourceName=rsdb&rasterName=Koala.png&rasterPath=/&x=116&y=39

service=rds&version=1.0.0&request=GetValue&sourceName=rsdb&rasterName=16km.tif&sx=1&sy=1&rasterPath=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetValue&rasterName=16km.tif&sourceName=rsdb&sx=1&sy=1&rasterPath=/

service=rds&version=1.0.0&request=GetValue&sourceName=rsdb&rasterName=16km.tif&x=116&y=39&rasterPath=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetValue&sourceName=rsdb&rasterName=16km.tif&x=116&y=39&rasterPath=/

************************************************************************/