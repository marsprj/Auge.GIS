#ifndef __AUGE_GPS_POLYGON_TO_LINE_REQUEST_H__
#define __AUGE_GPS_POLYGON_TO_LINE_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class PolygonToLineRequest : public WebRequest
	{
	public:
		PolygonToLineRequest();
		virtual ~PolygonToLineRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char* GetInputTypeName();
		const char*	GetInputSourceName();

		const char* GetOutputFormat();
		const char* GetOutputSourceName();
		const char* GetOutputTypeName();

	public:
		bool		Create(rude::CGI& cgi);
		bool		Create(XDocument* pxDoc);

		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		void		SetOutputFormat(const char* format);

		void		SetInputSourceName(const char* sourceName);
		void		SetInputTypeName(const char* typeName);
		
		void		SetOutputSourceName(const char* sourceName);
		void		SetOutputTypeName(const char* typeName);

		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;
		std::string m_output_format;

		std::string m_in_source_name;
		std::string m_in_type_name;

		std::string m_out_source_name;
		std::string m_out_type_name;

		XDocument	*m_pxDoc;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_POLYGON_TO_LINE_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=cities&outputSourceName=test&outputTypeName=cities_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=cities&outputSourceName=test&outputTypeName=cities_900913&outputSrid=900913
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=cities&outputSourceName=test&outputTypeName=cities_900913&outputSrid=900913

service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=rivers&outputSourceName=test&outputTypeName=rivers_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=rivers&outputSourceName=test&outputTypeName=rivers_900913&outputSrid=900913

service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=country&outputSourceName=test&outputTypeName=country_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=PolygonToLine&inputSourceName=test&inputTypeName=country&outputSourceName=test&outputTypeName=country_900913&outputSrid=900913

//service=gps&version=1.0.0&request=PolygonToLine&sourceName=db1&typeName=cities&distancefeild=id
//http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=PolygonToLine&sourceName=db1&typeName=cities&distancefeild=id
//http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=PolygonToLine&sourceName=db1&typeName=cities&distancefeild=id

2)
service=gps&version=1.0.0&request=PolygonToLine&featureset=
	<FeatureCollection>
		<FeatureMember>
		</FeatureMember>
	</FeatureCollection>


[ HTTP Post ]
-------------------------------------------------------------------------
1)
<PolygonToLine service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<TypeName>cities</TypeName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<TypeName>cities_900913_post</TypeName>
	<SRID>900913</SRID>
  </Output>
</PolygonToLine>
************************************************************************/
