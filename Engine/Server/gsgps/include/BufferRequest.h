#ifndef __AUGE_GPS_BUFFER_REQUEST_H__
#define __AUGE_GPS_BUFFER_REQUEST_H__

#include "AugeCore.h"
#include "AugeWebEngine.h"
#include "AugeGeometry.h"
#include <string>
#include <vector>
#include <rude/cgi.h>

namespace auge
{
	class GFilter;

	class BufferRequest : public WebRequest
	{
	public:
		BufferRequest();
		virtual ~BufferRequest();

	public:
		virtual const char*		GetUser();
		virtual const char*		GetEngine();
		virtual const char*		GetVersion();
		virtual const char*		GetRequest();
		virtual const char*		GetMimeType();
		virtual const char*		GetEncoding();

		virtual const char*		GetHost();
		virtual const char*		GetRequestMethod();

		const char* GetTypeName();
		const char*	GetSourceName();

		const char* GetDistanceField();
		double		GetDistance();
		GFilter*	GetFilter();

		const char* GetOutputFormat();
		const char* GetOutSourceName();
		const char* SetOutTypeName();
				
	public:
		bool		Create(rude::CGI& cgi);
		void		SetUser(const char* user);
		void		SetVersion(const char* value);
		
		void		SetHost(const char* host);
		void		SetRequestMethod(const char* method);
		void		SetOutputFormat(const char* format);

		void		SetInTypeName(const char* typeName);
		void		SetInSourceName(const char* sourceName);		

		void		SetOutSourceName(const char* sourceName);
		void		SetOutTypeName(const char* typeName);

		void		SetDistance(const char* distance);
		void		SetDistanceField(const char* distance_field);

		void		SetFilter(XElement* pxFilter);
		
		void		Info();

	private:
		std::string m_user;
		std::string m_version;
		std::string m_mime_type;
		std::string m_host;
		std::string m_request_method;
		std::string m_encoding;

		std::string m_in_source_name;
		std::string m_in_type_name;

		std::string m_out_source_name;
		std::string m_out_type_name;

		std::string m_output_format;
		std::string m_distance_field;
		double		m_distance;


		GFilter*	m_pFilter;

	private:
		static std::string	DEFAULT_HOST;
		static std::string	DEFAULT_METHOD;
	};

}

#endif //__AUGE_GPS_BUFFER_REQUEST_H__

/************************************************************************

[ HTTP Get ]
-------------------------------------------------------------------------
1)
service=gps&version=1.0.0&request=Buffer&sourceName=db1&typeName=cities&distance=1
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=Buffer&sourceName=db1&typeName=cities&distance=1
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=Buffer&sourceName=db1&typeName=cities&distance=1

service=gps&version=1.0.0&request=Buffer&sourceName=db1&typeName=cities&distancefeild=id
http://127.0.0.1:8088/ows/admin/mgr?service=gps&version=1.0.0&request=Buffer&sourceName=db1&typeName=cities&distancefeild=id
http://123.57.207.198/ows/admin/mgr?service=gps&version=1.0.0&request=Buffer&sourceName=db1&typeName=cities&distancefeild=id

2)
service=gps&version=1.0.0&request=Buffer&featureset=
	<FeatureCollection>
		<FeatureMember>
		</FeatureMember>
	</FeatureCollection>


[ HTTP Post ]
-------------------------------------------------------------------------
1)
<Buffer service="gps" version="1.0.0"
		  typeName="country"
		  outputFormat="GML2"
		  distance="1.0"
		  distancefield="id"
		  xmlns:ogc="http://www.opengis.net/ogc">
<Buffer>

2)
<Buffer service="gps" version="1.0.0"
		  typeName="country"
		  outputFormat="GML2"
		  distance="1.0"
		  xmlns:ogc="http://www.opengis.net/ogc">
	<Filter>
		<ogc:FeatureId fid="world.3"/>
	</Filter>
<Buffer>

3)
<Buffer service="gps" version="1.0.0"
		  typeName="country"
		  outputFormat="GML2"
		  xmlns:ogc="http://www.opengis.net/ogc">
	<Filter>
		<ogc:BBOX>
			<ogc:PropertyName>shape</ogc:PropertyName>
			<gml:Envelope xmlns:gml="http://www.opengis.net/gml">
				<gml:lowerCorner>0.0 90.0</gml:lowerCorner>
				<gml:upperCorner>0.0 180.0</gml:upperCorner>
			</gml:Envelope>
		</ogc:BBOX>
	</Filter>
<Buffer>
************************************************************************/
