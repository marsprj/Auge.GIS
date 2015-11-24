#ifndef __AUGE_POI_GET_POI_REQUEST_H__
#define __AUGE_POI_GET_POI_REQUEST_H__

#include "AugeCore.h"
#include "AugeGeometry.h"
#include "AugeWebEngine.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GetPoiRequest : public WebRequest
	{
		friend class GetPoiHandler;
	public:
		GetPoiRequest();
		virtual ~GetPoiRequest();

	public:
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

	public:
		const char* GetName();
		const char* GetFormat();
		GEnvelope&	GetExtent();
		g_uint		GetLimit();
		g_uint		GetOffset();

	public:
		bool		Create(rude::CGI& cgi);

		void		SetVersion(const char* version);
		void		SetName(const char* name);
		void		SetNames(const char* name);
		void		SetExtent(const char* extent);
		void		SetLimit(const char* limit);
		void		SetOffset(const char* offset);
		
	private:
		std::string m_version;
		std::string m_mime_type;
		std::string m_encoding;

		std::string m_name;
		std::vector<std::string> m_names;
		std::string m_format;
		GEnvelope	m_extent;
		g_uint		m_limit;
		g_uint		m_offset;
	};
}

#endif //__AUGE_POI_GET_POI_REQUEST_H__

/************************************************************************

[ HTTP Post ]
-------------------------------------------------------------------------
service=poi&version=1.0.0&request=GetPoi&name=¿¾´®
user=user1&service=poi&version=1.0.0&request=GetPoi&sourceName=rsdb&rasterName=Penguins.jpg&Path=/aaa

http://127.0.0.1:8088/ows/user1/mgr?service=poi&version=1.0.0&request=GetPoi&name=¿¾´®
http://127.0.0.1:8088/ows/user1/mgr?service=poi&version=1.0.0&request=GetPoi&sourceName=rsdb&rasterName=Koala.png&Path=/ccc

http://127.0.0.1:8088/ows/user1/mgr?service=poi&version=1.0.0&request=GetPoi&sourceName=rsdb&rasterName=Koala.png&Path=/ds1&extent=0,0,500,500&bands=3,2,1


service=poi&version=1.0.0&request=GetPoi&sourceName=rsdb&rasterName=Koala.png&Path=/&bands=3,2,1
http://127.0.0.1:8088/ows/user1/mgr?service=poi&version=1.0.0&request=GetPoi&sourceName=rsdb&rasterName=Koala.png&Path=/&bands=3,2,1

************************************************************************/