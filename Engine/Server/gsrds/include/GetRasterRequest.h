#ifndef __AUGE_RDS_GET_RASTER_REQUEST_H__
#define __AUGE_RDS_GET_RASTER_REQUEST_H__

#include "AugeCore.h"
#include "AugeGeometry.h"
#include "AugeWebEngine.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetRasterRequest : public WebRequest
	{
		friend class GetRasterHandler;
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
		const char* GetSourceName();
		const char*	GetPath();
		const char* GetFormat();
		GEnvelope&	GetExtent();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* version);
		void		SetPath(const char* path);
		void		SetRasterName(const char* rasterName);
		void		SetSourceName(const char* sourceName);
		void		SetFormat(const char* format);
		void		SetExtent(const char* extent);
		void		SetBands(const char* bands);

	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_raster_name;
		std::string m_sourc_name;
		std::string m_path;
		std::string m_format;

		std::vector<int>	m_bands;
		GEnvelope	m_extent;
	};
}

#endif //__AUGE_RDS_GET_RASTER_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=rds&version=1.0.0&request=GetRaster&sourceName=rsdb&rasterName=Koala.png&Path=/
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetRaster&sourceName=rsdb&rasterName=Koala.png&Path=/

service=rds&version=1.0.0&request=GetRaster&sourceName=rsdb&rasterName=Koala.png&Path=/&bands=3,2,1
http://127.0.0.1:8088/ows/user1/mgr?service=rds&version=1.0.0&request=GetRaster&sourceName=rsdb&rasterName=Koala.png&Path=/&bands=3,2,1

************************************************************************/