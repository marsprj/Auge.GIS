#ifndef __AUGE_GPS_CLUSTER_KMEAN_REQUEST_H__
#define __AUGE_GPS_CLUSTER_KMEAN_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include "AugeXML.h"
#include <string>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class KMeanRequest : public WebRequest
	{
	public:
		KMeanRequest();
		virtual ~KMeanRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		g_int		GetClusters();
		const char*	GetInputDataSource();
		const char*	GetInputFeatureClass();

		const char*	GetOutputDataSource();
		const char*	GetOutputFeatureClass();
		
	public:
		bool		Create(rude::CGI& cgi);
		bool		Create(XDocument* pxDoc);

		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		void		SetOutputFormat(const char* format);

		//////////////////////////////////////////////////////////////////////////
		void		SetClusters(const char* clusters);

		void		SetInputDataSource(const char* sourceName);
		void		SetInputFeatureClass(const char* typeName);

		void		SetOutputDataSource(const char* sourceName);
		void		SetOutputFeatureClass(const char* typeName);
		
		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;
		std::string m_output_format;

		//////////////////////////////////////////////////////////////////////////
		g_int	m_clusters;

		std::string	m_in_source_name;
		std::string	m_in_class_name;

		std::string	m_out_source_name;
		std::string	m_out_class_name;
		//////////////////////////////////////////////////////////////////////////

		XDocument	*m_pxDoc;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_CLUSTER_KMEAN_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=cities&outputSourceName=test&outputTypeName=citie_xxx&clusters=10

http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=cities&outputSourceName=test&outputTypeName=citie_xxx&clusters=10

http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=cities&outputSourceName=test&outputTypeName=cities_900913&outputSrid=900913

service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=rivers&outputSourceName=test&outputTypeName=rivers_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=rivers&outputSourceName=test&outputTypeName=rivers_900913&outputSrid=900913

service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=country&outputSourceName=test&outputTypeName=country_900913&outputSrid=900913
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=KMean&inputSourceName=test&inputTypeName=country&outputSourceName=test&outputTypeName=country_900913&outputSrid=900913

//service=gps&version=1.0.0&request=KMean&sourceName=db1&TypeName=cities&distancefeild=id
//http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=KMean&sourceName=db1&TypeName=cities&distancefeild=id
//http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=KMean&sourceName=db1&TypeName=cities&distancefeild=id

2)
service=gps&version=1.0.0&request=KMean&featureset=
	<FeatureCollection>
		<FeatureMember>
		</FeatureMember>
	</FeatureCollection>


[ HTTP Post ]
-------------------------------------------------------------------------
1)
<KMean service="gps" version="1.0.0">
  <Input>
	<SourceName>test</SourceName>
	<TypeName>cities</TypeName>
  </Input>
  <Output>
	<SourceName>test</SourceName>
	<TypeName>cities_900913_post</TypeName>
	<SRID>900913</SRID>
  </Output>
</KMean>
************************************************************************/
