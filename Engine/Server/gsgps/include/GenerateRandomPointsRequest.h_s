#ifndef __AUGE_GPS_GENERATE_RANDOM_POINTS_REQUEST_H__
#define __AUGE_GPS_GENERATE_RANDOM_POINTS_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class GenerateRandomPointsRequest : public WebRequest
	{
	public:
		GenerateRandomPointsRequest();
		virtual ~GenerateRandomPointsRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char* GetOutputFormat();
		const char* GetOutputSourceName();
		const char* GetOutputTypeName();
		g_uint		GetCount();
		g_uint		GetSRID();
		GEnvelope&	GetExtent();

	public:
		bool		Create(rude::CGI& cgi);
		bool		Create(XDocument* pxDoc);

		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		void		SetOutputFormat(const char* format);
				
		void		SetOutputSourceName(const char* sourceName);
		void		SetOutputTypeName(const char* typeName);

		void		SetSRID(const char* srid);
		void		SetExtent(const char* value);
		void		SetCount(const char* count);

		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;
		std::string m_output_format;

		std::string m_out_source_name;
		std::string m_out_type_name;

		GEnvelope	m_extent;
		g_uint		m_srid;
		g_uint		m_count;

		XDocument	*m_pxDoc;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_GENERATE_RANDOM_POINTS_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=GenerateRandomPoints&SourceName=test&TypeName=random_points_1000&Srid=900913&count=1000
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=GenerateRandomPoints&SourceName=test&TypeName=random_points_1000&Srid=900913&count=1000
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=GenerateRandomPoints&SourceName=test&TypeName=random_points_1000&Srid=900913&count=1000

service=gps&version=1.0.0&request=GenerateRandomPoints&inputSourceName=test&inputTypeName=rivers&SourceName=test&TypeName=rivers_900913&Srid=900913&count=1000
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=GenerateRandomPoints&inputSourceName=test&inputTypeName=rivers&SourceName=test&TypeName=rivers_900913&Srid=900913&count=1000

service=gps&version=1.0.0&request=GenerateRandomPoints&inputSourceName=test&inputTypeName=country&SourceName=test&TypeName=country_900913&Srid=900913&count=1000
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=GenerateRandomPoints&inputSourceName=test&inputTypeName=country&SourceName=test&TypeName=country_900913&Srid=900913&count=1000

//service=gps&version=1.0.0&request=GenerateRandomPoints&sourceName=db1&typeName=cities&distancefeild=id
//http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=GenerateRandomPoints&sourceName=db1&typeName=cities&distancefeild=id
//http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=GenerateRandomPoints&sourceName=db1&typeName=cities&distancefeild=id

2)
service=gps&version=1.0.0&request=GenerateRandomPoints&featureset=
	<FeatureCollection>
		<FeatureMember>
		</FeatureMember>
	</FeatureCollection>


[ HTTP Post ]
-------------------------------------------------------------------------
1)
<GenerateRandomPoints service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<TypeName>cities</TypeName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<TypeName>cities_900913_post</TypeName>
	<SRID>900913</SRID>
  </Output>
</GenerateRandomPoints>
************************************************************************/
